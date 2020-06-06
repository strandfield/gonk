BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "functions" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"return_type"	TEXT NOT NULL,
	"parameters"	TEXT NOT NULL,
	"specifiers"	TEXT,
	"binding"	TEXT,
	"implementation"	TEXT,
	"condition"	TEXT
);
COMMIT;
