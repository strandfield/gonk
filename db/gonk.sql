BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "types" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"typeid"	TEXT,
	"header"	TEXT,
	"is_enum"	INTEGER DEFAULT 0,
	"is_class"	INTEGER DEFAULT 0
);
CREATE TABLE IF NOT EXISTS "modules" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "enums" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "enumerators" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"enum_id"	INTEGER,
	FOREIGN KEY("enum_id") REFERENCES "enums"("id")
);
CREATE TABLE IF NOT EXISTS "inheritance" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"base"	INTEGER NOT NULL,
	"derived"	INTEGER NOT NULL,
	FOREIGN KEY("base") REFERENCES "classes"("id"),
	FOREIGN KEY("derived") REFERENCES "classes"("id")
);
CREATE TABLE IF NOT EXISTS "classes" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "functions" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"return_type"	TEXT NOT NULL,
	"parameters"	TEXT NOT NULL,
	"specifiers"	TEXT
);
CREATE TABLE IF NOT EXISTS "includes" (
	"id"	INTEGER NOT NULL UNIQUE,
	"header"	TEXT,
	"source"	TEXT,
	PRIMARY KEY("id"),
	FOREIGN KEY("id") REFERENCES "entities"("id")
);
CREATE TABLE IF NOT EXISTS "namespaces" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "statements" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"content"	TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS "files" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"hincludes"	TEXT,
	"cppincludes"	TEXT
);
CREATE TABLE IF NOT EXISTS "entities" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"parent"	INTEGER,
	"module_id"	INTEGER,
	"file_id"	INTEGER,
	"namespace_id"	INTEGER,
	"class_id"	INTEGER,
	"function_id"	INTEGER,
	"enum_id"	INTEGER,
	"enumerator_id"	INTEGER,
	"statement_id"	INTEGER,
	"condition"	TEXT,
	"renaming"	TEXT,
	"order"	INTEGER,
	"binding"	TEXT,
	FOREIGN KEY("statement_id") REFERENCES "statements"("id"),
	FOREIGN KEY("function_id") REFERENCES "functions"("id"),
	FOREIGN KEY("class_id") REFERENCES "classes"("id"),
	FOREIGN KEY("enum_id") REFERENCES "enums"("id"),
	FOREIGN KEY("enumerator_id") REFERENCES "enumerators"("id"),
	FOREIGN KEY("module_id") REFERENCES "modules"("id"),
	FOREIGN KEY("file_id") REFERENCES "files"("id"),
	FOREIGN KEY("namespace_id") REFERENCES "namespaces"("id")
);
INSERT INTO "types" VALUES (1,'bool','Int',NULL,0,0);
INSERT INTO "types" VALUES (2,'char','Char',NULL,0,0);
INSERT INTO "types" VALUES (3,'int','Int',NULL,0,0);
INSERT INTO "types" VALUES (4,'float','Float',NULL,0,0);
INSERT INTO "types" VALUES (5,'double','Double',NULL,0,0);
INSERT INTO "types" VALUES (6,'std::string','String',NULL,0,1);
COMMIT;
