// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "abstractnodeeditor.h"

AbstractNodeEditor::AbstractNodeEditor(std::shared_ptr<cxx::Entity> n, QWidget *p)
  : QWidget(p)
  , mNode(n)
{

}