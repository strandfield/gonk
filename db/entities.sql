BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "entities" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"parent"	INTEGER,
	"module_id"	INTEGER,
	"namespace_id"	INTEGER,
	"class_id"	INTEGER,
	"function_id"	INTEGER,
	"enum_id"	INTEGER,
	"enumerator_id"	INTEGER,
	FOREIGN KEY("function_id") REFERENCES "functions"("id"),
	FOREIGN KEY("class_id") REFERENCES "classes"("id"),
	FOREIGN KEY("enum_id") REFERENCES "enums"("id"),
	FOREIGN KEY("enumerator_id") REFERENCES "enumerators"("id"),
	FOREIGN KEY("module_id") REFERENCES "modules"("id"),
	FOREIGN KEY("namespace_id") REFERENCES "namespaces"("id"),
	FOREIGN KEY("parent") REFERENCES "entities"("id")
);
COMMIT;
