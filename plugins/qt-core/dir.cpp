// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "dir.h"

#include "gonk/common/binding/class.h"
#include "gonk/common/binding/enum.h"
#include "gonk/common/binding/namespace.h"
#include "gonk/common/enums.h"

#include <script/classbuilder.h>
#include <script/enumbuilder.h>

#include "qt-core/char_.h"
#include "qt-core/string.h"


static void register_dir__filter(script::Class& parent)
{
  using namespace script;
  
  Enum filter = gonk::bind::enumeration<QDir::Filter>(parent, "Filter").get();

  filter.addValue("Dirs", QDir::Filter::Dirs);
  filter.addValue("Files", QDir::Filter::Files);
  filter.addValue("Drives", QDir::Filter::Drives);
  filter.addValue("NoSymLinks", QDir::Filter::NoSymLinks);
  filter.addValue("AllEntries", QDir::Filter::AllEntries);
  filter.addValue("TypeMask", QDir::Filter::TypeMask);
  filter.addValue("Readable", QDir::Filter::Readable);
  filter.addValue("Writable", QDir::Filter::Writable);
  filter.addValue("Executable", QDir::Filter::Executable);
  filter.addValue("PermissionMask", QDir::Filter::PermissionMask);
  filter.addValue("Modified", QDir::Filter::Modified);
  filter.addValue("Hidden", QDir::Filter::Hidden);
  filter.addValue("System", QDir::Filter::System);
  filter.addValue("AccessMask", QDir::Filter::AccessMask);
  filter.addValue("AllDirs", QDir::Filter::AllDirs);
  filter.addValue("CaseSensitive", QDir::Filter::CaseSensitive);
  filter.addValue("NoDot", QDir::Filter::NoDot);
  filter.addValue("NoDotDot", QDir::Filter::NoDotDot);
  filter.addValue("NoDotAndDotDot", QDir::Filter::NoDotAndDotDot);
  filter.addValue("NoFilter", QDir::Filter::NoFilter);
}
static void register_dir__sort_flag(script::Class& parent)
{
  using namespace script;
  
  Enum sort_flag = gonk::bind::enumeration<QDir::SortFlag>(parent, "SortFlag").get();

  sort_flag.addValue("Name", QDir::SortFlag::Name);
  sort_flag.addValue("Time", QDir::SortFlag::Time);
  sort_flag.addValue("Size", QDir::SortFlag::Size);
  sort_flag.addValue("Unsorted", QDir::SortFlag::Unsorted);
  sort_flag.addValue("SortByMask", QDir::SortFlag::SortByMask);
  sort_flag.addValue("DirsFirst", QDir::SortFlag::DirsFirst);
  sort_flag.addValue("Reversed", QDir::SortFlag::Reversed);
  sort_flag.addValue("IgnoreCase", QDir::SortFlag::IgnoreCase);
  sort_flag.addValue("DirsLast", QDir::SortFlag::DirsLast);
  sort_flag.addValue("LocaleAware", QDir::SortFlag::LocaleAware);
  sort_flag.addValue("Type", QDir::SortFlag::Type);
  sort_flag.addValue("NoSort", QDir::SortFlag::NoSort);
}

static void register_dir(script::Namespace& parent)
{
  using namespace script;

  Engine* e = parent.engine();
  
  Class dir = parent.newClass("QDir").setId(e->makeType<QDir>().data())
    .get();

  Class& c = dir;
  
  register_dir__filter(c);
          register_dir__sort_flag(c);
        
  // QDir(const QDir&);
  gonk::bind::constructor<QDir, const QDir&>(c).create();
  // QDir(const QString& path);
  gonk::bind::constructor<QDir, const QString&>(c).create();
  // QDir(const QString& path, const QString& nameFilter, QDir::SortFlags sort, QDir::Filters filter);
  /// TODO: QDir(const QString& path, const QString& nameFilter, QDir::SortFlags sort, QDir::Filters filter);
  // ~QDir();
  gonk::bind::destructor<QDir>(c).create();
  // QDir& operator=(const QDir&);
  gonk::bind::memop_assign<QDir, const QDir&>(c);
  // QDir& operator=(const QString& path);
  gonk::bind::memop_assign<QDir, const QString&>(c);
  // QDir& operator=(QDir&& other);
  gonk::bind::memop_assign<QDir, QDir&&>(c);
  // void swap(QDir& other);
  gonk::bind::void_member_function<QDir, QDir&, &QDir::swap>(c, "swap").create();
  // void setPath(const QString& path);
  gonk::bind::void_member_function<QDir, const QString&, &QDir::setPath>(c, "setPath").create();
  // QString path() const;
  gonk::bind::member_function<QDir, QString, &QDir::path>(c, "path").create();
  // QString absolutePath() const;
  gonk::bind::member_function<QDir, QString, &QDir::absolutePath>(c, "absolutePath").create();
  // QString canonicalPath() const;
  gonk::bind::member_function<QDir, QString, &QDir::canonicalPath>(c, "canonicalPath").create();
  // static void addResourceSearchPath(const QString& path);
  gonk::bind::static_void_member_function<QDir, const QString&,&QDir::addResourceSearchPath>(c, "addResourceSearchPath").create();
  // static void setSearchPaths(const QString& prefix, const QStringList& searchPaths);
  /// TODO: static void setSearchPaths(const QString& prefix, const QStringList& searchPaths);
  // static void addSearchPath(const QString& prefix, const QString& path);
  gonk::bind::static_void_member_function<QDir, const QString&, const QString&,&QDir::addSearchPath>(c, "addSearchPath").create();
  // static QStringList searchPaths(const QString& prefix);
  /// TODO: static QStringList searchPaths(const QString& prefix);
  // QString dirName() const;
  gonk::bind::member_function<QDir, QString, &QDir::dirName>(c, "dirName").create();
  // QString filePath(const QString& fileName) const;
  gonk::bind::member_function<QDir, QString, const QString&, &QDir::filePath>(c, "filePath").create();
  // QString absoluteFilePath(const QString& fileName) const;
  gonk::bind::member_function<QDir, QString, const QString&, &QDir::absoluteFilePath>(c, "absoluteFilePath").create();
  // QString relativeFilePath(const QString& fileName) const;
  gonk::bind::member_function<QDir, QString, const QString&, &QDir::relativeFilePath>(c, "relativeFilePath").create();
  // static QString toNativeSeparators(const QString& pathName);
  gonk::bind::static_member_function<QDir, QString, const QString&, &QDir::toNativeSeparators>(c, "toNativeSeparators").create();
  // static QString fromNativeSeparators(const QString& pathName);
  gonk::bind::static_member_function<QDir, QString, const QString&, &QDir::fromNativeSeparators>(c, "fromNativeSeparators").create();
  // bool cd(const QString& dirName);
  gonk::bind::member_function<QDir, bool, const QString&, &QDir::cd>(c, "cd").create();
  // bool cdUp();
  gonk::bind::member_function<QDir, bool, &QDir::cdUp>(c, "cdUp").create();
  // QStringList nameFilters() const;
  /// TODO: QStringList nameFilters() const;
  // void setNameFilters(const QStringList& nameFilters);
  /// TODO: void setNameFilters(const QStringList& nameFilters);
  // QDir::Filters filter() const;
  /// TODO: QDir::Filters filter() const;
  // void setFilter(QDir::Filters filter);
  /// TODO: void setFilter(QDir::Filters filter);
  // QDir::SortFlags sorting() const;
  /// TODO: QDir::SortFlags sorting() const;
  // void setSorting(QDir::SortFlags sort);
  /// TODO: void setSorting(QDir::SortFlags sort);
  // uint count() const;
  /// TODO: uint count() const;
  // bool isEmpty(QDir::Filters filters) const;
  /// TODO: bool isEmpty(QDir::Filters filters) const;
  // QString operator[](int) const;
  gonk::bind::memop_const_subscript<QDir, QString, int>(c);
  // static QStringList nameFiltersFromString(const QString& nameFilter);
  /// TODO: static QStringList nameFiltersFromString(const QString& nameFilter);
  // QStringList entryList(QDir::Filters filters, QDir::SortFlags sort) const;
  /// TODO: QStringList entryList(QDir::Filters filters, QDir::SortFlags sort) const;
  // QStringList entryList(const QStringList& nameFilters, QDir::Filters filters, QDir::SortFlags sort) const;
  /// TODO: QStringList entryList(const QStringList& nameFilters, QDir::Filters filters, QDir::SortFlags sort) const;
  // QFileInfoList entryInfoList(QDir::Filters filters, QDir::SortFlags sort) const;
  /// TODO: QFileInfoList entryInfoList(QDir::Filters filters, QDir::SortFlags sort) const;
  // QFileInfoList entryInfoList(const QStringList&, QDir::Filters, QDir::SortFlags) const;
  /// TODO: QFileInfoList entryInfoList(const QStringList&, QDir::Filters, QDir::SortFlags) const;
  // bool mkdir(const QString& dirName) const;
  gonk::bind::member_function<QDir, bool, const QString&, &QDir::mkdir>(c, "mkdir").create();
  // bool rmdir(const QString& dirName) const;
  gonk::bind::member_function<QDir, bool, const QString&, &QDir::rmdir>(c, "rmdir").create();
  // bool mkpath(const QString& dirPath) const;
  gonk::bind::member_function<QDir, bool, const QString&, &QDir::mkpath>(c, "mkpath").create();
  // bool rmpath(const QString& dirPath) const;
  gonk::bind::member_function<QDir, bool, const QString&, &QDir::rmpath>(c, "rmpath").create();
  // bool removeRecursively();
  gonk::bind::member_function<QDir, bool, &QDir::removeRecursively>(c, "removeRecursively").create();
  // bool isReadable() const;
  gonk::bind::member_function<QDir, bool, &QDir::isReadable>(c, "isReadable").create();
  // bool exists() const;
  gonk::bind::member_function<QDir, bool, &QDir::exists>(c, "exists").create();
  // bool isRoot() const;
  gonk::bind::member_function<QDir, bool, &QDir::isRoot>(c, "isRoot").create();
  // static bool isRelativePath(const QString& path);
  gonk::bind::static_member_function<QDir, bool, const QString&, &QDir::isRelativePath>(c, "isRelativePath").create();
  // static bool isAbsolutePath(const QString& path);
  gonk::bind::static_member_function<QDir, bool, const QString&, &QDir::isAbsolutePath>(c, "isAbsolutePath").create();
  // bool isRelative() const;
  gonk::bind::member_function<QDir, bool, &QDir::isRelative>(c, "isRelative").create();
  // bool isAbsolute() const;
  gonk::bind::member_function<QDir, bool, &QDir::isAbsolute>(c, "isAbsolute").create();
  // bool makeAbsolute();
  gonk::bind::member_function<QDir, bool, &QDir::makeAbsolute>(c, "makeAbsolute").create();
  // bool operator==(const QDir& dir) const;
  gonk::bind::memop_eq<QDir, const QDir&>(c);
  // bool operator!=(const QDir& dir) const;
  gonk::bind::memop_neq<QDir, const QDir&>(c);
  // bool remove(const QString& fileName);
  gonk::bind::member_function<QDir, bool, const QString&, &QDir::remove>(c, "remove").create();
  // bool rename(const QString& oldName, const QString& newName);
  gonk::bind::member_function<QDir, bool, const QString&, const QString&, &QDir::rename>(c, "rename").create();
  // bool exists(const QString& name) const;
  gonk::bind::member_function<QDir, bool, const QString&, &QDir::exists>(c, "exists").create();
  // static QFileInfoList drives();
  /// TODO: static QFileInfoList drives();
  // static QChar listSeparator();
  gonk::bind::static_member_function<QDir, QChar, &QDir::listSeparator>(c, "listSeparator").create();
  // static QChar separator();
  gonk::bind::static_member_function<QDir, QChar, &QDir::separator>(c, "separator").create();
  // static bool setCurrent(const QString& path);
  gonk::bind::static_member_function<QDir, bool, const QString&, &QDir::setCurrent>(c, "setCurrent").create();
  // static QDir current();
  gonk::bind::static_member_function<QDir, QDir, &QDir::current>(c, "current").create();
  // static QString currentPath();
  gonk::bind::static_member_function<QDir, QString, &QDir::currentPath>(c, "currentPath").create();
  // static QDir home();
  gonk::bind::static_member_function<QDir, QDir, &QDir::home>(c, "home").create();
  // static QString homePath();
  gonk::bind::static_member_function<QDir, QString, &QDir::homePath>(c, "homePath").create();
  // static QDir root();
  gonk::bind::static_member_function<QDir, QDir, &QDir::root>(c, "root").create();
  // static QString rootPath();
  gonk::bind::static_member_function<QDir, QString, &QDir::rootPath>(c, "rootPath").create();
  // static QDir temp();
  gonk::bind::static_member_function<QDir, QDir, &QDir::temp>(c, "temp").create();
  // static QString tempPath();
  gonk::bind::static_member_function<QDir, QString, &QDir::tempPath>(c, "tempPath").create();
  // static bool match(const QStringList& filters, const QString& fileName);
  /// TODO: static bool match(const QStringList& filters, const QString& fileName);
  // static bool match(const QString& filter, const QString& fileName);
  gonk::bind::static_member_function<QDir, bool, const QString&, const QString&, &QDir::match>(c, "match").create();
  // static QString cleanPath(const QString& path);
  gonk::bind::static_member_function<QDir, QString, const QString&, &QDir::cleanPath>(c, "cleanPath").create();
  // void refresh() const;
  gonk::bind::const_void_member_function<QDir, &QDir::refresh>(c, "refresh").create();
}


void register_dir_file(script::Namespace ns)
{
  using namespace script;

  register_dir(ns);

  // void swap(QDir& value1, QDir& value2);
  gonk::bind::void_function<QDir&, QDir&, &swap>(ns, "swap").create();
  // QFlags<QDir::Filters::enum_type> operator|(QDir::Filters::enum_type f1, QDir::Filters::enum_type f2);
  /// TODO: QFlags<QDir::Filters::enum_type> operator|(QDir::Filters::enum_type f1, QDir::Filters::enum_type f2);
  // QFlags<QDir::Filters::enum_type> operator|(QDir::Filters::enum_type f1, QFlags<QDir::Filters::enum_type> f2);
  /// TODO: QFlags<QDir::Filters::enum_type> operator|(QDir::Filters::enum_type f1, QFlags<QDir::Filters::enum_type> f2);
  // QIncompatibleFlag operator|(QDir::Filters::enum_type f1, int f2);
  /// TODO: QIncompatibleFlag operator|(QDir::Filters::enum_type f1, int f2);
  // QFlags<QDir::SortFlags::enum_type> operator|(QDir::SortFlags::enum_type f1, QDir::SortFlags::enum_type f2);
  /// TODO: QFlags<QDir::SortFlags::enum_type> operator|(QDir::SortFlags::enum_type f1, QDir::SortFlags::enum_type f2);
  // QFlags<QDir::SortFlags::enum_type> operator|(QDir::SortFlags::enum_type f1, QFlags<QDir::SortFlags::enum_type> f2);
  /// TODO: QFlags<QDir::SortFlags::enum_type> operator|(QDir::SortFlags::enum_type f1, QFlags<QDir::SortFlags::enum_type> f2);
  // QIncompatibleFlag operator|(QDir::SortFlags::enum_type f1, int f2);
  /// TODO: QIncompatibleFlag operator|(QDir::SortFlags::enum_type f1, int f2);
  // QDebug operator<<(QDebug debug, QDir::Filters filters);
  /// TODO: QDebug operator<<(QDebug debug, QDir::Filters filters);
  // QDebug operator<<(QDebug debug, const QDir& dir);
  /// TODO: QDebug operator<<(QDebug debug, const QDir& dir);
}
