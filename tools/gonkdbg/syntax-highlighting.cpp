// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "syntax-highlighting.h"

#include <QTextDocument>

#include <QJsonObject>
#include <QJsonArray>

#include <QTimer>

#include <QDebug>


enum class TokenType
{
  Invalid = 0,
  NumericLiteral,
  StringLiteral,
  Keyword,
  Punctuator,
  Identifier,
  Comment,
};

class Token
{
public:
  TokenType type = TokenType::Invalid;
  QStringView str;

public:
  Token();
  Token(const Token&) = default;
  ~Token() = default;

  Token(TokenType t, QStringView strv)
    : type(t), str(strv)
  {

  }

  bool isValid() const { return type != TokenType::Invalid; }

  bool operator==(const Token& other) const { return type == other.type && other.str == str; }
  bool operator!=(const Token& other) const { return (*this) != other; }
  bool operator==(TokenType tok) const { return type == tok; }
  bool operator!=(TokenType tok) const { return !operator==(tok); }

  Token& operator=(const Token&) = default;
};

inline Token::Token()
  : type(TokenType::Invalid), str()
{

}


class Tokenizer
{
public:
  std::vector<Token> output;

public:
  Tokenizer() = default;

  void tokenize(const QStringView& str);

  enum CharacterType {
    Invalid,
    Space,
    Letter,
    Digit,
    Dot,
    SingleQuote,
    DoubleQuote,
    LeftPar,
    RightPar,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    Punctuator,
    Underscore,
    Semicolon,
    Colon,
    QuestionMark,
    Comma,
    Tabulation,
    LineBreak,
    CarriageReturn,
    Other,
  };

  static CharacterType ctype(QChar c);
  inline static bool isLetter(QChar c) { return ctype(c) == Letter; }
  inline static bool isDigit(QChar c) { return ctype(c) == Digit; }
  inline static bool isIdentifier(QChar c) { return isLetter(c) || c == '_'; }
  inline static bool isIdentifierOrDigit(QChar c) { return isIdentifier(c) || isDigit(c); }
  inline static bool isBinary(QChar c) { return c == '0' || c == '1'; }
  inline static bool isOctal(QChar c) { return '0' <= c && c <= '7'; }
  inline static bool isDecimal(QChar c) { return isDigit(c); }
  inline static bool isHexa(QChar c) { return isDecimal(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'); }
  static bool isDiscardable(QChar c);
  inline static bool isSpace(QChar c) { return ctype(c) == Space; }

protected:
  void read();
  void write(const Token& tok);
  void write(TokenType type);
  bool atEnd() const;
  size_t pos() const;
  QChar readChar();
  void discardChar() noexcept;
  QChar charAt(size_t pos);
  QChar currentChar() const;
  inline QChar peekChar() const { return currentChar(); }
  void consumeDiscardable();
  QStringView currentText() const;
  void readNumericLiteral();
  void readHexa();
  void readOctal();
  void readBinary();
  void readDecimal();
  void readIdentifier();
  TokenType identifierType(size_t begin, size_t end) const;
  void readStringLiteral();
  void readCharLiteral();
  void readFromSlash();
  void readSingleLineComment();
  void createLongComment();
  void readMultiLineComment();
  bool tryReadLiteralSuffix();

private:
  const QChar* m_chars = nullptr;
  size_t m_len = 0;
  size_t m_pos = 0;
  size_t m_start = 0;
};


void Tokenizer::tokenize(const QStringView& str)
{
  m_chars = str.data();
  m_len = str.size();
  m_pos = 0;
  m_start = 0;

  while (!atEnd())
    read();
}

void Tokenizer::read()
{
  consumeDiscardable();

  if (atEnd())
    return;

  m_start = pos();
  size_t p = m_start;

  QChar c = readChar();
  CharacterType ct = ctype(c);

  switch (ct)
  {
  case Digit:
    return readNumericLiteral();
  case DoubleQuote:
    return readStringLiteral();
  case SingleQuote:
    return readCharLiteral();
  case Letter:
  case Underscore:
    return readIdentifier();
  case LeftPar:
  case RightPar:
  case LeftBrace:
  case RightBrace:
  case LeftBracket:
  case RightBracket:
  case Semicolon:
  case Colon:
  case QuestionMark:
  case Comma:
  case Dot:
  case Punctuator:
  {
    if (c == '/')
      return readFromSlash();

    return write(TokenType::Punctuator);
  }
  default:
    return write(TokenType::Invalid);
  }
}

void Tokenizer::write(const Token& tok)
{
  this->output.push_back(tok);
}

void Tokenizer::write(TokenType type)
{
  write(Token(type, currentText()));
}

bool Tokenizer::atEnd() const
{
  return m_pos == m_len;
}

size_t Tokenizer::pos() const
{
  return m_pos;
}

QChar Tokenizer::readChar()
{
  return *(m_chars + m_pos++);
}

void Tokenizer::discardChar() noexcept
{
  ++m_pos;
}

QChar Tokenizer::charAt(size_t pos)
{
  return m_chars[pos];
}

QChar Tokenizer::currentChar() const
{
  return *(m_chars + m_pos);
}

void Tokenizer::consumeDiscardable()
{
  while (!atEnd() && isDiscardable(peekChar()))
    discardChar();
}

QStringView Tokenizer::currentText() const
{
  return QStringView(m_chars + m_start, pos() - m_start);
}

Tokenizer::CharacterType Tokenizer::ctype(QChar c)
{
  static const CharacterType map[] = {
    Invalid, // NUL    (Null char.)
    Invalid, // SOH    (Start of Header)
    Invalid, // STX    (Start of Text)
    Invalid, // ETX    (End of Text)
    Invalid, // EOT    (End of Transmission)
    Invalid, // ENQ    (Enquiry)
    Invalid, // ACK    (Acknowledgment)
    Invalid, // BEL    (Bell)
    Invalid, //  BS    (Backspace)
    Tabulation, //  HT    (Horizontal Tab)
    LineBreak, //  LF    (Line Feed)
    Invalid, //  VT    (Vertical Tab)
    Invalid, //  FF    (Form Feed)
    CarriageReturn, //  CR    (Carriage Return)
    Invalid, //  SO    (Shift Out)
    Invalid, //  SI    (Shift In)
    Invalid, // DLE    (Data Link Escape)
    Invalid, // DC1    (XON)(Device Control 1)
    Invalid, // DC2    (Device Control 2)
    Invalid, // DC3    (XOFF)(Device Control 3)
    Invalid, // DC4    (Device Control 4)
    Invalid, // NAK    (Negative Acknowledgement)
    Invalid, // SYN    (Synchronous Idle)
    Invalid, // ETB    (End of Trans. Block)
    Invalid, // CAN    (Cancel)
    Invalid, //  EM    (End of Medium)
    Invalid, // SUB    (Substitute)
    Invalid, // ESC    (Escape)
    Invalid, //  FS    (File Separator)
    Invalid, //  GS    (Group Separator)
    Invalid, //  RS    (Request to Send)(Record Separator)
    Invalid, //  US    (Unit Separator)
    Space, //  SP    (Space)
    Punctuator, //   !    (exclamation mark)
    DoubleQuote, //   "    (double quote)
    Punctuator, //   #    (number sign)
    Punctuator, //   $    (dollar sign)
    Punctuator, //   %    (percent)
    Punctuator, //   &    (ampersand)
    SingleQuote, //   '    (single quote)
    LeftPar, //   (    (left opening parenthesis)
    RightPar, //   )    (right closing parenthesis)
    Punctuator, //   *    (asterisk)
    Punctuator, //   +    (plus)
    Comma, //   ,    (comma)
    Punctuator, //   -    (minus or dash)
    Dot, //   .    (dot)
    Punctuator, //   /    (forward slash)
    Digit, //   0
    Digit, //   1
    Digit, //   2
    Digit, //   3
    Digit, //   4
    Digit, //   5
    Digit, //   6
    Digit, //   7
    Digit, //   8
    Digit, //   9
    Colon, //   :    (colon)
    Semicolon, //   ;    (semi-colon)
    Punctuator, //   <    (less than sign)
    Punctuator, //   =    (equal sign)
    Punctuator, //   >    (greater than sign)
    QuestionMark, //   ?    (question mark)
    Punctuator, //   @    (AT symbol)
    Letter, //   A
    Letter, //   B
    Letter, //   C
    Letter, //   D
    Letter, //   E
    Letter, //   F
    Letter, //   G
    Letter, //   H
    Letter, //   I
    Letter, //   J
    Letter, //   K
    Letter, //   L
    Letter, //   M
    Letter, //   N
    Letter, //   O
    Letter, //   P
    Letter, //   Q
    Letter, //   R
    Letter, //   S
    Letter, //   T
    Letter, //   U
    Letter, //   V
    Letter, //   W
    Letter, //   X
    Letter, //   Y
    Letter, //   Z
    LeftBracket, //   [    (left opening bracket)
    Punctuator, //   \    (back slash)
    RightBracket, //   ]    (right closing bracket)
    Punctuator, //   ^    (caret cirumflex)
    Underscore, //   _    (underscore)
    Punctuator, //   `
    Letter, //   a
    Letter, //   b
    Letter, //   c
    Letter, //   d
    Letter, //   e
    Letter, //   f
    Letter, //   g
    Letter, //   h
    Letter, //   i
    Letter, //   j
    Letter, //   k
    Letter, //   l
    Letter, //   m
    Letter, //   n
    Letter, //   o
    Letter, //   p
    Letter, //   q
    Letter, //   r
    Letter, //   s
    Letter, //   t
    Letter, //   u
    Letter, //   v
    Letter, //   w
    Letter, //   x
    Letter, //   y
    Letter, //   z
    LeftBrace, //   {    (left opening brace)
    Punctuator, //   |    (vertical bar)
    RightBrace, //   }    (right closing brace)
    Punctuator, //   ~    (tilde)
    Invalid, // DEL    (delete)
  };

  if (c <= 127)
    return map[c.toLatin1()];
  return Other;
}

bool Tokenizer::isDiscardable(QChar c)
{
  return ctype(c) == Space || ctype(c) == LineBreak || ctype(c) == CarriageReturn || ctype(c) == Tabulation;
}

void Tokenizer::readNumericLiteral()
{
  if (atEnd()) {
    return write(TokenType::NumericLiteral);
  }

  QChar c = peekChar();

  // Reading binary, octal or hexadecimal number
  // eg. : 0b00110111
  //       018
  //       0xACDBE
  if (charAt(m_start) == '0' && c != '.')
  {
    if (c == 'x') // hexadecimal
      return readHexa();
    else if (c == 'b') // binary
      return readBinary();
    else if (Tokenizer::isDigit(c))// octal
      return readOctal();
    else // it is zero
      return write(TokenType::NumericLiteral);
  }

  return readDecimal();
}

void Tokenizer::readHexa()
{
  const QChar x = readChar();
  assert(x == 'x');

  if (atEnd())  // input ends with '0x' -> error
    return write(TokenType::Invalid);

  while (!atEnd() && Tokenizer::isHexa(peekChar()))
    readChar();

  if (pos() - m_start == 2) // e.g. 0x+
    return write(TokenType::Invalid);

  return write(TokenType::NumericLiteral);
}

void Tokenizer::readOctal()
{
  while (!atEnd() && Tokenizer::isOctal(peekChar()))
    readChar();

  return write(TokenType::NumericLiteral);
}

void Tokenizer::readBinary()
{
  const QChar b = readChar();
  assert(b == 'b');

  if (atEnd())  // input ends with '0b' -> error
    return write(TokenType::Invalid);

  while (!atEnd() && Tokenizer::isBinary(peekChar()))
    readChar();

  return write(TokenType::NumericLiteral);
}

void Tokenizer::readDecimal()
{
  // Reading decimal numbers
  // eg. : 25
  //       3.14
  //       3.14f
  //       100e100
  //       6.02e23
  //       6.67e-11

  while (!atEnd() && Tokenizer::isDigit(peekChar()))
    readChar();

  if (atEnd())
    return write(TokenType::NumericLiteral);

  if (peekChar() == '.')
  {
    readChar();

    while (!atEnd() && Tokenizer::isDigit(peekChar()))
      readChar();

    if (atEnd())
      return write(TokenType::NumericLiteral);
  }

  if (peekChar() == 'e')
  {
    readChar();

    if (atEnd())
      return write(TokenType::Invalid);

    if (peekChar() == '+' || peekChar() == '-')
    {
      readChar();
      if (atEnd())
        return write(TokenType::Invalid);
    }

    while (!atEnd() && Tokenizer::isDigit(peekChar()))
      readChar();

    if (atEnd())
      return write(TokenType::NumericLiteral);
  }


  if (peekChar() == 'f') // eg. 125.f
  {
    readChar();
  }
  else
  {
    if (tryReadLiteralSuffix())
      return write(TokenType::NumericLiteral);
  }

  return write(TokenType::NumericLiteral);
}

bool Tokenizer::tryReadLiteralSuffix()
{
  auto cpos = pos();

  if (!this->atEnd() && (Tokenizer::isLetter(peekChar()) || peekChar() == '_'))
    readChar();
  else
    return false;

  while (!this->atEnd() && (Tokenizer::isLetter(peekChar()) || Tokenizer::isDigit(peekChar()) || peekChar() == '_'))
    readChar();

  const bool read = (cpos != pos());
  return read;
}

void Tokenizer::readIdentifier()
{
  while (!this->atEnd() && (Tokenizer::isLetter(peekChar()) || Tokenizer::isDigit(peekChar()) || peekChar() == '_'))
    readChar();

  return write(identifierType(m_start, pos()));
}

TokenType Tokenizer::identifierType(size_t begin, size_t end) const
{
  static const std::set<QString, std::less<>> keywords = {
    "do", 
    "if", 
    "for", 
    "int", 
    "try",
    "auto",
    "bool",
    "case",
    "char",
    "else",
    "enum",
    "this",
    "true",
    "void",
    "break",
    "catch",
    "class",
    "const",
    "false",
    "final",
    "float",
    "throw",
    "using",
    "while",
    "delete",
    "double",
    "export",
    "friend",
    "import",
    "inline",
    "public",
    "return",
    "static",
    "struct",
    "typeid",
    "default",
    "mutable",
    "private",
    "typedef",
    "virtual",
    "continue",
    "explicit",
    "noexcept",
    "operator",
    "override",
    "template",
    "typename",
    "constexpr",
    "namespace",
    "protected",
  };

  auto it = keywords.find(QStringView(m_chars + begin, m_chars + end));
  return it != keywords.end() ? TokenType::Keyword : TokenType::Identifier;
}

void Tokenizer::readStringLiteral()
{
  while (!atEnd() && peekChar() != '"')
  {
    if (peekChar() == '\\')
    {
      readChar();
      if (!atEnd())
        readChar();
    }
    else if (peekChar() == '\n')
    {
      return write(TokenType::Invalid);
    }
    else
    {
      readChar();
    }
  }

  if (atEnd())
    return write(TokenType::Invalid);

  assert(peekChar() == '"');
  readChar();

  if (tryReadLiteralSuffix())
    return write(TokenType::StringLiteral);

  return write(TokenType::StringLiteral);
}

void Tokenizer::readCharLiteral()
{
  if (atEnd())
    return write(TokenType::Invalid);

  readChar();

  if (atEnd())
    return write(TokenType::Invalid);

  if (ctype(readChar()) != SingleQuote)
    return write(TokenType::Invalid);

  return write(TokenType::StringLiteral);
}

void Tokenizer::readFromSlash()
{
  if (atEnd())
    return write(TokenType::Punctuator);
  if (peekChar() == '/')
    return readSingleLineComment();
  else if (peekChar() == '*')
    return readMultiLineComment();
  else
    return write(TokenType::Punctuator);
}

void Tokenizer::readSingleLineComment()
{
  readChar(); // reads the second '/'

  while (!atEnd() && peekChar() != '\n')
    readChar();

  return write(TokenType::Comment);
}

void Tokenizer::createLongComment()
{
  return write(TokenType::Comment);
}

void Tokenizer::readMultiLineComment()
{
  readChar(); // reads the '*' after opening '/'

  do {
    while (!atEnd() && peekChar() != '*')
      readChar();

    if (atEnd())
      return createLongComment();

    assert(peekChar() == '*');
    readChar(); // reads the '*'

    if (atEnd())
      return createLongComment();

  } while (peekChar() != '/');

  readChar(); // reads the closing '/'
  return write(TokenType::Comment);
}


SyntaxHighlighter::SyntaxHighlighter(QTextDocument* doc)
  : QSyntaxHighlighter(doc)
{
  m_formats.resize(Comment + 1);

  QTextCharFormat fmt;
  //fmt.setFont(document()->defaultFont());

  fmt.setForeground(QColor("#808000"));
  initFormat(Format::Keyword, fmt);

  fmt.setForeground(QColor("#000080"));
  initFormat(Format::Literal, fmt);

  fmt.setForeground(QColor("#000080"));
  initFormat(Format::Preproc, fmt);

  fmt.setForeground(QColor("#008000"));
  initFormat(Format::String, fmt);

  fmt.setForeground(QColor("#00677c"));
  initFormat(Format::Function, fmt);

  fmt.setForeground(QColor("#000080"));
  initFormat(Format::Macro, fmt);

  fmt.setForeground(QColor("#000080"));
  initFormat(Format::Macro, fmt);

  fmt.setForeground(QColor("#008000"));
  initFormat(Format::Include, fmt);

  fmt.setForeground(QColor("#800080"));
  initFormat(Format::Typename, fmt);

  fmt.setForeground(QColor("#860d0d"));
  initFormat(Format::MemberName, fmt);

  fmt.setForeground(QColor("#800080"));
  initFormat(Format::NamespaceName, fmt);

  fmt.setForeground(QColor("#008000"));
  initFormat(Format::Comment, fmt);

  m_rehighlight_timer = new QTimer(this);
  m_rehighlight_timer->setSingleShot(true);
  m_rehighlight_timer->setInterval(1000);

  connect(m_rehighlight_timer, &QTimer::timeout, this, &SyntaxHighlighter::rehighlight);
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
  int prevstate = previousBlockState();
  int blocknum = currentBlock().blockNumber();

  int begin = 0;

  if (prevstate == ST_Comment)
  {
    begin = text.indexOf("*/");

    if (begin != -1)
    {
      begin = begin + 2;
      setFormat(0, begin, Comment);
      setCurrentBlockState(0);
    }
    else
    {
      setFormat(0, text.length(), Comment);
      setCurrentBlockState(prevstate);
      return;
    }
  }

  Tokenizer lexer;
  lexer.tokenize(QStringView(text.begin() + begin, text.end()));

  for (size_t i(0); i < lexer.output.size(); ++i)
  {
    const Token& tok = lexer.output.at(i);

    int offset = tok.str.data() - text.data();

    if (tok.type == TokenType::StringLiteral)
    {
      setFormat(offset, tok.str.length(), Format::String);
    }
    else if (tok.type == TokenType::NumericLiteral)
    {
      setFormat(offset, tok.str.length(), Format::Literal);
    }
    else if (tok.type == TokenType::Keyword)
    {
      setFormat(offset, tok.str.length(), Format::Keyword);
    }
    else if (tok.type == TokenType::Punctuator)
    {
      // no-op
    }
    else if (tok.type == TokenType::Identifier)
    {
      if (i + 1 < lexer.output.size())
      {
        if(lexer.output.at(i+1).str == '(')
          setFormat(offset, tok.str.length(), Format::Function);
        else if(lexer.output.at(i + 1).str == QLatin1String("::"))
          setFormat(offset, tok.str.length(), Format::Typename);
        else
          setFormat(offset, tok.str.length(), guessFormat(blocknum, offset, tok.str));
      }
      else
      {
        setFormat(offset, tok.str.length(), guessFormat(blocknum, offset, tok.str));
      }
    }
    else if (tok.type == TokenType::Comment)
    {
      setFormat(offset, tok.str.length(), Format::Comment);
    }
  }

  if (!lexer.output.empty())
  {
    const Token& tok = lexer.output.back();
    
    bool starts_multi_comment = tok.type == TokenType::Comment && tok.str.startsWith(QLatin1String("/*")) && !tok.str.endsWith(QLatin1String("*/"));
    setCurrentBlockState(starts_multi_comment ? ST_Comment : 0);
  }
  else
  {
    setCurrentBlockState(0);
  }
}

void SyntaxHighlighter::initFormat(Format fmt, const QTextCharFormat& value)
{
  m_formats[static_cast<size_t>(fmt)] = value;
}

void SyntaxHighlighter::setFormat(int start, int count, Format fmt)
{
  QSyntaxHighlighter::setFormat(start, count, m_formats.at(fmt));
}

SyntaxHighlighter::Format SyntaxHighlighter::guessFormat(int line, int col, const QStringView& tok) const
{
  // @TODO: use "m_hints"
  return Default;
}
