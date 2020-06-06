BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "enums" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"type"	INTEGER,
	FOREIGN KEY("type") REFERENCES "types"("id")
);
INSERT INTO "enums" VALUES (1,'Base64Option',9);
INSERT INTO "enums" VALUES (2,'SectionFlag',15);
INSERT INTO "enums" VALUES (3,'SplitBehavior',12);
INSERT INTO "enums" VALUES (4,'NormalizationForm',11);
INSERT INTO "enums" VALUES (5,'SpecialCharacter',24);
INSERT INTO "enums" VALUES (6,'Category',20);
INSERT INTO "enums" VALUES (7,'Script',23);
INSERT INTO "enums" VALUES (8,'Direction',27);
INSERT INTO "enums" VALUES (9,'Decomposition',25);
INSERT INTO "enums" VALUES (10,'JoiningType',22);
INSERT INTO "enums" VALUES (11,'Joining',28);
INSERT INTO "enums" VALUES (12,'CombiningClass',29);
INSERT INTO "enums" VALUES (13,'UnicodeVersion',26);
COMMIT;
