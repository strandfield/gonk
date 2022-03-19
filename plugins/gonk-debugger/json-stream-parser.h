// Copyright (C) 2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_DEBUGGER_JSSTREAMPARSER_H
#define GONK_DEBUGGER_JSSTREAMPARSER_H

#include <json-toolkit/parsing.h>

namespace script
{
class Class;
} // namespace script

namespace gonk
{

class JsonStreamParser
{
public:
  std::vector<json::Object> objects;

  void write(const std::string& data);

private:
  json::Tokenizer<json::DefaultTokenizerBackend> m_tokenizer;
  json::ParserMachine<json::DefaultParserBackend> m_parser;
};

inline void JsonStreamParser::write(const std::string& data)
{
  m_tokenizer.write(data);

  json::ParserMachine<json::DefaultParserBackend> m_parser;

  for (const auto& tok : m_tokenizer.backend().token_buffer)
  {
    m_parser.write(tok);

    if (m_parser.state() == json::ParserState::Idle)
    {
      objects.push_back(m_parser.backend().stack.front().toObject());
      m_parser.backend().stack.clear();
    }
  }

  m_tokenizer.backend().token_buffer.clear();
}

} // namespace gonk

#endif // GONK_DEBUGGER_JSSTREAMPARSER_H
