BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "enums" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"enum_class"	INTEGER DEFAULT 0,
	"type"	INTEGER,
	FOREIGN KEY("type") REFERENCES "types"("id")
);
INSERT INTO "enums" VALUES (1,'Base64Option',0,9);
INSERT INTO "enums" VALUES (2,'Base64DecodingStatus',1,10);
INSERT INTO "enums" VALUES (3,'SpecialCharacter',NULL,17);
INSERT INTO "enums" VALUES (4,'Category',NULL,18);
INSERT INTO "enums" VALUES (5,'Script',NULL,19);
INSERT INTO "enums" VALUES (6,'Direction',NULL,15);
INSERT INTO "enums" VALUES (7,'Decomposition',NULL,20);
INSERT INTO "enums" VALUES (8,'JoiningType',NULL,21);
INSERT INTO "enums" VALUES (9,'Joining',NULL,22);
INSERT INTO "enums" VALUES (10,'CombiningClass',NULL,23);
INSERT INTO "enums" VALUES (11,'UnicodeVersion',NULL,24);
INSERT INTO "enums" VALUES (12,'SectionFlag',NULL,28);
INSERT INTO "enums" VALUES (13,'SplitBehavior',NULL,32);
INSERT INTO "enums" VALUES (14,'NormalizationForm',NULL,29);
COMMIT;
