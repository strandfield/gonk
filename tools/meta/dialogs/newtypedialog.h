// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_NEWTYPEDIALOG_H
#define METAGONK_NEWTYPEDIALOG_H

#include <QDialog>

#include "project.h"

class QComboBox;
class QLineEdit;

class NewTypeDialog : public QDialog
{
  Q_OBJECT
public:
  NewTypeDialog(QWidget *parent = nullptr);
  ~NewTypeDialog() = default;

  MGType::Category getCategory() const;
  std::shared_ptr<MGType> getType() const;

private:
  QComboBox *mTypeCategoryComboBox;
  QLineEdit *mNameLineEdit;
  QLineEdit *mIdLineEdit;
};

#endif // METAGONK_NEWTYPEDIALOG_H
