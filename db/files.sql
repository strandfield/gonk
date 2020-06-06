BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "files" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"hincludes"	TEXT,
	"cppincludes"	TEXT
);
INSERT INTO "files" VALUES (1,'bytearray','<QByteArray>','');
INSERT INTO "files" VALUES (2,'string','<QString>','');
INSERT INTO "files" VALUES (3,'char_','<QChar>','');
COMMIT;
