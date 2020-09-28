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
INSERT INTO "classes" VALUES (2,'FromBase64Result',11,'',NULL);
INSERT INTO "classes" VALUES (3,'QByteRef',12,'',NULL);
INSERT INTO "classes" VALUES (4,'FromBase64Result',13,'',NULL);
INSERT INTO "classes" VALUES (5,'QLatin1Char',14,'',NULL);
INSERT INTO "classes" VALUES (6,'QChar',16,'',NULL);
INSERT INTO "classes" VALUES (7,'QLatin1String',27,'',NULL);
INSERT INTO "classes" VALUES (8,'QString',25,'',NULL);
INSERT INTO "classes" VALUES (11,'Null',31,'',NULL);
INSERT INTO "classes" VALUES (12,'QCharRef',33,'',NULL);
INSERT INTO "classes" VALUES (13,'QStringRef',34,'',NULL);
INSERT INTO "classes" VALUES (14,'QDir',36,'',NULL);
INSERT INTO "classes" VALUES (15,'QObject',38,'',NULL);
INSERT INTO "classes" VALUES (16,'QSignalBlocker',39,'',NULL);
COMMIT;
