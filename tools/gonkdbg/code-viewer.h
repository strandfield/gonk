// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CPPC_CODEVIEWER_H
#define CPPC_CODEVIEWER_H

#include <QPlainTextEdit>

#include <debugger/message.h>

#include <QPixmap>

class Controller;
class SyntaxHighlighter;
class LineNumberArea;

class CodeViewer : public QPlainTextEdit
{
  Q_OBJECT
public:
  explicit CodeViewer(Controller& con, std::shared_ptr<gonk::debugger::SourceCode> src, QWidget* parent = nullptr);

  enum MarkerType
  {
    BreakpointMarker = 1,
    BreakpositionMarker = 2,
  };

  struct Marker
  {
    int line = -1;
    int markers = 0;
  };

  QString documentPath() const;

  void clearMarkers();
  void addMarker(int line, MarkerType m);
  void updateMarkers();

  void toggleBreakpoint(int line);

protected:
  void resizeEvent(QResizeEvent* ev) override;

protected: 
  friend class LineNumberArea;
  void lineNumberAreaPaintEvent(QPaintEvent* ev);
  void lineNumberAreaMousePress(QMouseEvent* ev);
  int columnWidth() const;
  int lineNumberAreaWidth();

  void updateLineNumberAreaWidth(int newBlockCount);
  void updateLineNumberArea(const QRect& rect, int dy);

  bool find_marker(int line, std::vector<Marker>::const_iterator& it) const;
  void drawMarkers(QPainter& painter, QRect rect, int markers);

private:
  Controller& m_controller;
  SyntaxHighlighter* m_highlighter = nullptr;
  LineNumberArea* m_linenumberarea = nullptr;
  std::shared_ptr<gonk::debugger::SourceCode> m_source;
  std::vector<Marker> m_markers;
  int m_charwidth = -1;
  int m_lineheight = -1;
  QPixmap m_breakpoint_pixmap;
  QPixmap m_cursor_pixmap;
};

class LineNumberArea : public QWidget
{
  Q_OBJECT
public:
  explicit LineNumberArea(CodeViewer* editor);

  QSize sizeHint() const override;

protected:
  void paintEvent(QPaintEvent* event) override;

  void mousePressEvent(QMouseEvent* ev) override;

private:
  CodeViewer* m_codeeditor;
};


#endif // CPPC_CODEVIEWER_H
