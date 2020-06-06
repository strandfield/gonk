// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gen/cppfile.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include <QDebug>

#include <algorithm>

CppFile::CppFile()
{
  this->copyright_message =
    "// Copyright (C) 2020 Vincent Chambrin\n"
    "// This file is part of the 'gonk' project\n"
    "// For conditions of distribution and use, see copyright notice in LICENSE\n";
}

void CppFile::include(const char* group, QString file)
{
  auto it = std::find_if(includes.begin(), includes.end(), [group](const IncludeGroup& ig) {
    return ig.group_name == group;
    });

  if (it == includes.end())
    it = includes.insert(includes.end(), IncludeGroup{ group, {} });
  
  it->files.insert(std::move(file));
}

void CppFile::write(const QFileInfo& fileinfo)
{
  QFile file{ fileinfo.absoluteFilePath() };
  file.open(QIODevice::WriteOnly | QIODevice::Truncate);

  QTextStream stream{ &file };

  if (!copyright_message.isEmpty())
  {
    stream << copyright_message;
    stream << "\n";
  }

  if (!header_guard.isEmpty())
  {
    stream << "#ifndef " << header_guard << "\n";
    stream << "#define " << header_guard << "\n";
    stream << "\n";
  }

  if (!main_include.isEmpty())
  {
    stream << "#include \"" << main_include << "\"\n";
    stream << "\n";
  }

  for (const IncludeGroup inc : includes)
  {
    for (const QString& i : inc.files)
    {
      if (i.startsWith("<"))
        stream << "#include " << i << "\n";
      else
        stream << "#include \"" << i << "\"\n";
    }

    stream << "\n";
  }

  for (const QString& l : lines)
  {
    stream << l << "\n";
  }

  if (!header_guard.isEmpty())
  {
    stream << "#endif // " << header_guard << "\n";
  }

  stream.flush();
}
