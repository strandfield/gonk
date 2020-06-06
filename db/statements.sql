BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "statements" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"content"	TEXT NOT NULL
);
INSERT INTO "statements" VALUES (1,'namespace\n{\nscript::Value bytearray_string_ctor(script::FunctionCall* c)\n{\n  c->thisObject().init<QByteArray>(script::get<std::string>(c->arg(1)).c_str());\n  return c->thisObject();\n}\n}\n');
COMMIT;
