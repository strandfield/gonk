// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef CPPC_CODEVIEWER_H
#define CPPC_CODEVIEWER_H

#include <QPlainTextEdit>

#include <QPixmap>

#include <debugger/message.h>

class Controller;
class SyntaxHighlighter;
class LineNumberArea;

class CodeViewer : public QPlainTextEdit
{
  Q_OBJECT
public:
  explicit CodeViewer(Controller& con, QWidget* parent = nullptr);

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

  void setSource(std::shared_ptr<gonk::debugger::SourceCode> src);

  void loadDocument(const QString& path);
  QString documentPath() const;

  void clearMarkers();
  void addMarker(int line, MarkerType m);

Q_SIGNALS:
  void gutterLineClicked(int line);

protected:
  void resizeEvent(QResizeEvent* ev) override;

protected: 
  friend class LineNumberArea;
  void lineNumberAreaPaintEvent(QPaintEvent* ev);
  void lineNumberAreaMousePress(QMouseEvent* ev);
  int lineNumberAreaWidth();

  void updateLineNumberAreaWidth(int newBlockCount);
  void updateLineNumberArea(const QRect& rect, int dy);

  bool find_marker(int line, std::vector<Marker>::const_iterator& it) const;
  void drawMarkers(QPainter& painter, QPoint pt, int markers);

private:
  Controller& m_controller;
  SyntaxHighlighter* m_highlighter = nullptr;
  LineNumberArea* m_linenumberarea = nullptr;
  std::shared_ptr<gonk::debugger::SourceCode> m_source;
  std::vector<Marker> m_markers;
  int m_charwidth = -1;
  int m_lineheight = -1;
  QPixmap m_breakpoint_icon;
  QPixmap m_breakpos_icon;
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
