// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "database-exporter.h"

#include <QSqlError>

#include <QFile>
#include <QTextStream>

#include <QDebug>

#include <stdexcept>


void DatabaseExporter::exportDatabase()
{
	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"classes\" (\n"
			"	\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"name\"	TEXT NOT NULL,\n"
			"	\"type\"	INTEGER,\n"
			"	\"base\"	TEXT,\n"
			"	\"final\"	INTEGER,\n"
			"	FOREIGN KEY(\"type\") REFERENCES \"types\"(\"id\")\n"
			");";

		TableDescription table{ "classes", {
			INTEGER_NOT_NULL("id"),
			TEXT_NOT_NULL("name"),
			INTEGER("type"),
			TEXT("base"),
			INTEGER("final")
     } };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"entities\" (\n"
			"	\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"parent\"	INTEGER,\n"
			"	\"namespace_id\"	INTEGER,\n"
			"	\"class_id\"	INTEGER,\n"
			"	\"function_id\"	INTEGER,\n"
			"	\"enum_id\"	INTEGER,\n"
			"	\"enumerator_id\"	INTEGER,\n"
			"	FOREIGN KEY(\"parent\") REFERENCES \"entities\"(\"id\"),\n"
			"	FOREIGN KEY(\"class_id\") REFERENCES \"classes\"(\"id\"),\n"
			"	FOREIGN KEY(\"function_id\") REFERENCES \"functions\"(\"id\"),\n"
			"	FOREIGN KEY(\"enum_id\") REFERENCES \"enums\"(\"id\"),\n"
			"	FOREIGN KEY(\"enumerator_id\") REFERENCES \"enumerators\"(\"id\"),\n"
			"	FOREIGN KEY(\"namespace_id\") REFERENCES \"namespaces\"(\"id\")\n"
			");";

		TableDescription table{ "entities", {
			INTEGER_NOT_NULL("id"),
			INTEGER("parent"),
			INTEGER("namespace_id"),
			INTEGER("class_id"),
			INTEGER("function_id"),
			INTEGER("enum_id"),
			INTEGER("enumerator_id")
		 } };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"enumerators\" (\n"
			"	\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"name\"	TEXT NOT NULL,\n"
			"	\"enum_id\"	INTEGER,\n"
			"	FOREIGN KEY(\"enum_id\") REFERENCES \"enums\"(\"id\")\n"
			");";

		TableDescription table{ "enumerators", {
			INTEGER_NOT_NULL("id"),
			TEXT_NOT_NULL("name"),
			INTEGER("enum_id")
		 } };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"enums\" (\n"
			"	\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"name\"	TEXT NOT NULL,\n"
			"	\"enum_class\"	INTEGER DEFAULT 0,\n"
			"	\"type\"	INTEGER,\n"
			"	FOREIGN KEY(\"type\") REFERENCES \"types\"(\"id\")\n"
			");";

		TableDescription table{ "enums", {
			INTEGER_NOT_NULL("id"),
			TEXT_NOT_NULL("name"),
			INTEGER("enum_class"),
			INTEGER("type")
		 } };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"files\" (\n"
			"	\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"path\"	TEXT NOT NULL\n"
			");";

		TableDescription table{ "files", {
			INTEGER_NOT_NULL("id"),
			TEXT_NOT_NULL("path")
		 } };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"functions\" (\n"
			"	\"id\"	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"name\"	TEXT NOT NULL,\n"
			"	\"return_type\"	TEXT NOT NULL,\n"
			"	\"parameters\"	TEXT NOT NULL,\n"
			"	\"specifiers\"	TEXT\n"
			");";

		TableDescription table{ "functions", {
			INTEGER_NOT_NULL("id"),
			TEXT_NOT_NULL("name"),
			TEXT_NOT_NULL("return_type"),
			TEXT_NOT_NULL("parameters"),
			TEXT("specifiers"),
		 } };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"metadata\" (\n"
			"	\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"entity_id\"	INTEGER NOT NULL,\n"
			"	\"name\"	TEXT NOT NULL,\n"
			"	\"value\"	TEXT,\n"
			"	FOREIGN KEY(\"entity_id\") REFERENCES \"entities\"(\"id\")\n"
			");";

		TableDescription table{ "metadata", {
			INTEGER_NOT_NULL("id"),
			INTEGER_NOT_NULL("entity_id"),
			TEXT_NOT_NULL("name"),
			TEXT("value")
		 } };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"namespaces\" (\n"
			"	\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"name\"	TEXT NOT NULL\n"
			");";

		TableDescription table{ "namespaces", {
			INTEGER_NOT_NULL("id"),
			TEXT_NOT_NULL("name")
		 } };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"source_locations\" (\n"
			"	\"entity_id\"	INTEGER NOT NULL,\n"
			"	\"file_id\"	INTEGER NOT NULL,\n"
			"	\"line\"	INTEGER,\n"
			"	\"column\"	INTEGER,\n"
			"	PRIMARY KEY(\"entity_id\"),\n"
			"	FOREIGN KEY(\"entity_id\") REFERENCES \"entities\"(\"id\"),\n"
			"	FOREIGN KEY(\"file_id\") REFERENCES \"files\"(\"id\")\n"
			");";

		TableDescription table{ "source_locations", {
			INTEGER_NOT_NULL("entity_id"),
			INTEGER_NOT_NULL("file_id"),
			INTEGER("line"),
			INTEGER("column")
		} };

		exportTable(table, table_creation);
	}

	{
		const QString table_creation =
			"CREATE TABLE IF NOT EXISTS \"types\" (\n"
			"	\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\n"
			"	\"name\"	TEXT NOT NULL,\n"
			"	\"typeid\"	TEXT,\n"
			"	\"is_enum\"	INTEGER DEFAULT 0,\n"
			"	\"is_class\"	INTEGER DEFAULT 0\n"
			");";

		TableDescription table{ "types", {
			INTEGER_NOT_NULL("id"),
			TEXT_NOT_NULL("name"),
			TEXT("typeid"),
			INTEGER("is_enum"),
			INTEGER("is_class")
		} };

		exportTable(table, table_creation);
	}
}

FieldDescription DatabaseExporter::INTEGER_NOT_NULL(const QString& name)
{
	FieldDescription r;
	r.name = name;
	r.type = DBFieldType::INTEGER;
	r.not_null = true;
	return r;
}

FieldDescription DatabaseExporter::INTEGER(const QString& name)
{
	FieldDescription r;
	r.name = name;
	r.type = DBFieldType::INTEGER;
	r.not_null = false;
	return r;
}

FieldDescription DatabaseExporter::TEXT_NOT_NULL(const QString& name)
{
	FieldDescription r;
	r.name = name;
	r.type = DBFieldType::TEXT;
	r.not_null = true;
	return r;
}

FieldDescription DatabaseExporter::TEXT(const QString& name)
{
	FieldDescription r;
	r.name = name;
	r.type = DBFieldType::TEXT;
	r.not_null = false;
	return r;
}

void DatabaseExporter::exportTable(const TableDescription& table, const QString& createStatement)
{
	QFile output_file{ m_export_dir.absoluteFilePath(table.name + ".sql") };
	output_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

	QTextStream stream{ &output_file };

	const QStringList field_name_list = [&]() {
		QStringList l;
		for (const auto& f : table.fields)
			l.append(f.name);
		return l;
	}();

	const QString field_names = field_name_list.join(", ");
  
	int nb_fields = field_names.count(',') + 1;

	stream << "BEGIN TRANSACTION;" << "\n";
	stream << createStatement << "\n";

	QSqlQuery query = Database::exec("SELECT " + field_names + " FROM " + table.name);

	while (query.next())
	{
		QStringList field_values;


		for (size_t i(0); i < table.fields.size(); ++i)
		{
			const auto& f = table.fields.at(i);

			QVariant field_val = query.value(static_cast<int>(i));

			if (!f.not_null && query.value(static_cast<int>(i)).isNull())
			{
				field_values.push_back("NULL");
			}
			else
			{
				if (f.type == DBFieldType::TEXT)
				{
					field_values.push_back(QString("'") + Database::sqlEscape(field_val.toString()) + QString("'"));
				}
				else if (f.type == DBFieldType::INTEGER)
				{
					field_values.push_back(QString::number(field_val.toInt()));
				}
			}
		}

		stream << "INSERT INTO \"" << table.name << "\" VALUES (" << field_values.join(',') << ");" << "\n";
	}

	stream << "COMMIT;" << "\n";
}
