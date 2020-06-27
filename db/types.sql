BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "types" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"typeid"	TEXT,
	"is_enum"	INTEGER DEFAULT 0,
	"is_class"	INTEGER DEFAULT 0
);
INSERT INTO "types" VALUES (1,'void','Void',0,0);
INSERT INTO "types" VALUES (2,'bool','Boolean',0,0);
INSERT INTO "types" VALUES (3,'char','Char',0,0);
INSERT INTO "types" VALUES (4,'int','Int',0,0);
INSERT INTO "types" VALUES (5,'float','Float',0,0);
INSERT INTO "types" VALUES (6,'double','Double',0,0);
INSERT INTO "types" VALUES (7,'std::string','String',0,1);
COMMIT;
