// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_SYNTAXHIGHLIGHTER_H
#define GONKDBG_SYNTAXHIGHLIGHTER_H

#include <QObject>

#include <debugger/message.h>

#include <typewriter/syntaxhighlighter.h>

class GonkSyntaxHighlighter
{
  typewriter::SyntaxHighlighter m_highlighter;
  std::vector<typewriter::Position> m_pos_mapper;

public:
  explicit GonkSyntaxHighlighter(typewriter::TextView& view);
  ~GonkSyntaxHighlighter() = default;

  enum Format
  {
    Keyword = 1,
    Literal = 2,
    Identifier = 3,
    Operator = 4,
    Punctuator = 5,
    Other = 6,
  };

  void highlight(std::shared_ptr<gonk::debugger::SourceCode> src);

protected:
  void map_positions(const std::string& src);
  void recursive_highlight(const QJsonObject& json);
};

#endif // GONKDBG_SYNTAXHIGHLIGHTER_H
