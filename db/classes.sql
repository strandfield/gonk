BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "classes" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"name"	TEXT NOT NULL,
	"type"	INTEGER,
	"base"	TEXT,
	"final"	INTEGER,
	FOREIGN KEY("type") REFERENCES "types"("id")
);
INSERT INTO "classes" VALUES (1,'QByteArray',8,'',NULL);
INSERT INTO "classes" VALUES (2,'QByteRef',10,'',NULL);
INSERT INTO "classes" VALUES (3,'QLatin1String',13,'',NULL);
INSERT INTO "classes" VALUES (4,'QString',14,'',NULL);
INSERT INTO "classes" VALUES (5,'Null',16,'',NULL);
INSERT INTO "classes" VALUES (6,'QCharRef',17,'',NULL);
INSERT INTO "classes" VALUES (7,'QStringRef',18,'',NULL);
INSERT INTO "classes" VALUES (8,'QLatin1Char',19,'',NULL);
INSERT INTO "classes" VALUES (9,'QChar',21,'',NULL);
COMMIT;
