// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_FORMAT_H
#define METAGONK_FORMAT_H

#include <QString>

inline QString format(const char* str)
{
  return QString(str);
}


struct Formatter
{
  static QString toString(QString str)
  {
    return str;
  }

  static QString toString(std::string str)
  {
    return QString::fromStdString(str);
  }

  static QString toString(int n)
  {
    return QString::number(n);
  }
};

template<typename T>
QString format(QString fmt, const T& arg)
{
  return fmt.arg(Formatter::toString(arg));
}

template<typename T1, typename T2>
QString format(QString fmt, const T1& a1, const T2& a2)
{
  return fmt.arg(Formatter::toString(a1), Formatter::toString(a2));
}

template<typename T1, typename T2, typename T3>
QString format(QString fmt, const T1& a1, const T2& a2, const T3& a3)
{
  return fmt.arg(Formatter::toString(a1), Formatter::toString(a2), Formatter::toString(a3));
}

template<typename T1, typename T2, typename T3, typename T4>
QString format(QString fmt, const T1& a1, const T2& a2, const T3& a3, const T4& a4)
{
  return fmt.arg(Formatter::toString(a1), Formatter::toString(a2), Formatter::toString(a3), Formatter::toString(a4));
}

#endif // METAGONK_FORMAT_H
