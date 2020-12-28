// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "syntaxhighlighter.h"

#include <unicode/utf8.h>

#include <QJsonArray>
#include <QJsonObject>

#include <QDebug>

GonkSyntaxHighlighter::GonkSyntaxHighlighter(typewriter::TextView& view)
  : m_highlighter{ view }
{

}

void GonkSyntaxHighlighter::highlight(std::shared_ptr<gonk::debugger::SourceCode> src)
{
  map_positions(src->source);

  QJsonArray nodes = src->syntaxtree.value("nodes").toArray();

  for (int i(0); i < nodes.size(); ++i)
  {
    recursive_highlight(nodes.at(i).toObject());
  }
}

void GonkSyntaxHighlighter::map_positions(const std::string& src)
{
  typewriter::Position pos;
  pos.line = 0;
  pos.column = 0;

  m_pos_mapper.clear();
  m_pos_mapper.resize(src.size() + 1, pos);

  size_t r = 0;

  while (r < src.size())
  {
    m_pos_mapper[r] = pos;

    if (src.at(r) == '\n')
    {
      pos.line += 1;
      pos.column = 0;
    }
    else
    {
      pos.column += 1;
    }

    r += unicode::utf8::codepoint_length(src.data() + r);
  }

  m_pos_mapper.back() = pos;
}

void GonkSyntaxHighlighter::recursive_highlight(const QJsonObject& json)
{
  QJsonArray children = json.value("children").toArray();

  static const QString keyword = "kw";
  static const QString lit = "lit";
  static const QString identifier = "id";
  static const QString op = "op";
  static const QString punctuator = "p";
  static const QString other = "o";

  if (children.isEmpty())
  {
    size_t offset = json.value("offset").toInt();
    size_t length = json.value("length").toInt();
    QString type = json.value("type").toString();

    int line = m_pos_mapper.at(offset).line;
    int col = m_pos_mapper.at(offset).column;
    int width = m_pos_mapper.at(offset + length).column - col;

    if (type == keyword)
    {
      m_highlighter.setFormat(line, col, width, 1);
    }
    else if (type == lit)
    {
      m_highlighter.setFormat(line, col, width, 2);
    }
    else if (type == identifier)
    {
      m_highlighter.setFormat(line, col, width, 3);
    }
    else if (type == op)
    {
      m_highlighter.setFormat(line, col, width, 4);
    }
    else if (type == punctuator)
    {
      m_highlighter.setFormat(line, col, width, 5);
    }
    else
    {
      m_highlighter.setFormat(line, col, width, 6);
    }
  }
  else
  {
    for (int i(0); i < children.size(); ++i)
    {
      recursive_highlight(children.at(i).toObject());
    }
  }
}
