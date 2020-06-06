BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "types" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"typeid"	TEXT,
	"header"	TEXT,
	"is_enum"	INTEGER DEFAULT 0,
	"is_class"	INTEGER DEFAULT 0,
	"condition"	TEXT
);
INSERT INTO "types" VALUES (1,'void','Void',NULL,0,0,NULL);
INSERT INTO "types" VALUES (2,'bool','Boolean',NULL,0,0,NULL);
INSERT INTO "types" VALUES (3,'char','Char',NULL,0,0,NULL);
INSERT INTO "types" VALUES (4,'int','Int',NULL,0,0,NULL);
INSERT INTO "types" VALUES (5,'float','Float',NULL,0,0,NULL);
INSERT INTO "types" VALUES (6,'double','Double',NULL,0,0,NULL);
INSERT INTO "types" VALUES (7,'std::string','String',NULL,0,1,NULL);
COMMIT;
