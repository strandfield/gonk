// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_SYNTAXHIGHLIGHTING_H
#define GONKDBG_SYNTAXHIGHLIGHTING_H

#include <QSyntaxHighlighter> 

#include <set>

class QStringView;

class SyntaxHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT
public:
  SyntaxHighlighter(QTextDocument* doc);

  enum Format
  {
    Default = 0,
    Keyword,
    Literal,
    Preproc,
    String,
    Function,
    Macro,
    Include,
    Typename,
    MemberName,
    NamespaceName, 
    Comment,
  };

  enum State
  {
    ST_Comment = 1,
  };

  struct Hint
  {
    int line;
    int col;
    Format type;
  };

protected:
  void highlightBlock(const QString& text) override;

protected:
  void initFormat(Format fmt, const QTextCharFormat& value);
  void setFormat(int start, int count, Format fmt);
  Format guessFormat(int line, int col, const QStringView& tok) const;

private:
  std::vector<QTextCharFormat> m_formats;
  std::string m_text;
  std::vector<int> m_offsets;
  QTimer* m_rehighlight_timer;
  std::vector<Hint> m_hints;
};

#endif // GONKDBG_SYNTAXHIGHLIGHTING_H
