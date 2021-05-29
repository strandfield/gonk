// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "code-viewer.h"

#include "controller.h"
#include "syntax-highlighting.h"

#include <QPainter>
#include <QPainterPath>

#include <QDebug>

CodeViewer::CodeViewer(Controller& con, std::shared_ptr<gonk::debugger::SourceCode> src, QWidget* parent)
  : QPlainTextEdit(parent),
    m_controller(con),
    m_source(src)
{
  setReadOnly(true);

  QFont font{ "Courier" };
  font.setStyleHint(QFont::StyleHint::TypeWriter);
  document()->setDefaultFont(font);

  QFontMetrics metrics{ font };
  m_charwidth = metrics.width('M');
  m_lineheight = metrics.height();

  m_highlighter = new SyntaxHighlighter(document());

  {
    m_linenumberarea = new LineNumberArea(this);

    connect(this, &CodeViewer::blockCountChanged, this, &CodeViewer::updateLineNumberAreaWidth);
    connect(this, &CodeViewer::updateRequest, this, &CodeViewer::updateLineNumberArea);

    updateLineNumberAreaWidth(0);
  }

  m_breakpoint_pixmap = QPixmap(":/breakpoint.png");
  m_cursor_pixmap = QPixmap(":/cursor.png");

  QTextCursor cursor{ document() };
  cursor.insertText(QString::fromStdString(src->source));

  document()->setMetaInformation(QTextDocument::MetaInformation::DocumentUrl, QString::fromStdString(src->path));

  connect(&m_controller, &Controller::currentFrameChanged, this, &CodeViewer::updateMarkers);
  connect(&m_controller, &Controller::callstackUpdated, this, &CodeViewer::updateMarkers);
  connect(&m_controller, &Controller::breakpointsUpdated, this, &CodeViewer::updateMarkers);
  connect(&(m_controller.client()), &gonk::debugger::Client::stateChanged, this, &CodeViewer::updateMarkers);

  updateMarkers();
}

QString CodeViewer::documentPath() const
{
  return document()->metaInformation(QTextDocument::MetaInformation::DocumentUrl);
}

void CodeViewer::clearMarkers()
{
  if (!m_markers.empty())
  {
    m_markers.clear();
    m_linenumberarea->update();
  }
}

void CodeViewer::addMarker(int line, MarkerType m)
{
  auto it = std::find_if(m_markers.begin(), m_markers.end(), [line](const Marker& m) {
    return m.line >= line;
    });

  if (it == m_markers.end() || it->line > line)
  {
    Marker marker;
    marker.line = line;
    marker.markers = static_cast<int>(m);
    m_markers.insert(it, marker);
    m_linenumberarea->update();
  }
  else
  {
    it->markers |= static_cast<int>(m);
    m_linenumberarea->update();
  }
}

void CodeViewer::updateMarkers()
{
  clearMarkers();

  auto callstack = m_controller.lastCallstackMessage();

  if (callstack != nullptr)
  {
    const auto& callstack_top = [&]() -> const gonk::debugger::CallstackEntry& {
      return m_controller.currentFrame() == -1 ? callstack->entries.back() : callstack->entries.at(m_controller.currentFrame());
    }();

    std::shared_ptr<gonk::debugger::SourceCode> src = m_controller.getSource(callstack_top.path);

    if (src == m_source)
    {
      addMarker(callstack_top.line, CodeViewer::BreakpositionMarker);
    }
  }

  auto breakpoints = m_controller.lastBreakpointListMessage();

  if (breakpoints)
  {
    std::string my_path = documentPath().toStdString();

    for (const gonk::debugger::BreakpointData& bp : breakpoints->list)
    {
      if (bp.script_path == my_path)
      {
        addMarker(bp.line, CodeViewer::BreakpointMarker);
      }
    }
  }
}

void CodeViewer::toggleBreakpoint(int line)
{
  if (m_controller.hasBreakpoint(documentPath().toStdString(), line))
  {
    m_controller.client().removeBreakpoint(documentPath().toStdString(), line);
  }
  else
  {
    m_controller.client().addBreakpoint(documentPath().toStdString(), line);
  }

  m_controller.client().getBreakpoints();
}

void CodeViewer::resizeEvent(QResizeEvent* ev)
{
  QPlainTextEdit::resizeEvent(ev);

  QRect cr = contentsRect();
  m_linenumberarea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeViewer::lineNumberAreaPaintEvent(QPaintEvent* ev)
{
  QPainter painter{ m_linenumberarea };
  painter.fillRect(ev->rect(), Qt::lightGray);

  QFontMetrics metrics{ document()->defaultFont() };
  painter.setFont(document()->defaultFont());
  painter.setPen(Qt::black);

  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());

  std::vector<Marker>::const_iterator marker_it = m_markers.cbegin();

  while (block.isValid() && top <= ev->rect().bottom()) 
  {
    if (block.isVisible() && bottom >= ev->rect().top()) 
    {
      if (find_marker(blockNumber, marker_it))
        drawMarkers(painter, QRect(QPoint(3, top), QSize(bottom-top, bottom-top)), marker_it->markers);

      QString number = QString::number(blockNumber + 1);
      painter.drawText(0, top, m_linenumberarea->width(), metrics.height(), Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

void CodeViewer::lineNumberAreaMousePress(QMouseEvent* ev)
{
  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());

  while (block.isValid() && ev->y() < top || ev->y() > bottom)
  {
    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }

  if (block.isValid())
  {
    toggleBreakpoint(blockNumber);
  }
}

int CodeViewer::columnWidth() const
{
  return QFontMetrics(document()->defaultFont()).horizontalAdvance(QLatin1Char('9'));
}

int CodeViewer::lineNumberAreaWidth()
{
  int digits = 1;
  int max = qMax(1, blockCount());

  while (max >= 10)
  {
    max /= 10;
    ++digits;
  }

  int space = 3 + QFontMetrics(document()->defaultFont()).height() + columnWidth() * digits;
  return space;
}

void CodeViewer::updateLineNumberAreaWidth(int /* newBlockCount */)
{
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeViewer::updateLineNumberArea(const QRect& rect, int dy)
{
  if (dy)
    m_linenumberarea->scroll(0, dy);
  else
    m_linenumberarea->update(0, rect.y(), m_linenumberarea->width(), rect.height());

  if (rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
}

bool CodeViewer::find_marker(int line, std::vector<Marker>::const_iterator& it) const
{
  if (it == m_markers.end())
    return false;

  while (it != m_markers.end() && it->line < line)
    ++it;

  return it != m_markers.end() && it->line == line;
}

void CodeViewer::drawMarkers(QPainter& painter, QRect rect, int markers)
{
  painter.setRenderHint(QPainter::Antialiasing);

  rect = QRect(rect.topLeft() + QPoint(0, rect.height() - rect.width()) / 2, QSize(rect.width(), rect.width()));

  if (markers & static_cast<int>(MarkerType::BreakpointMarker))
  {
    painter.drawPixmap(rect, m_breakpoint_pixmap.scaled(rect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  }

  if (markers & static_cast<int>(MarkerType::BreakpositionMarker))
  {
    painter.drawPixmap(rect, m_cursor_pixmap.scaled(rect.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
  }
}

LineNumberArea::LineNumberArea(CodeViewer* editor)
  : QWidget(editor),
    m_codeeditor(editor)
{

}

QSize LineNumberArea::sizeHint() const
{
  return QSize(m_codeeditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent* event)
{
  m_codeeditor->lineNumberAreaPaintEvent(event);
}

void LineNumberArea::mousePressEvent(QMouseEvent* ev)
{
  m_codeeditor->lineNumberAreaMousePress(ev);
}
