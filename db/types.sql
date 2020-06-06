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
INSERT INTO "types" VALUES (8,'QByteArray','QByteArray','',0,1,NULL);
INSERT INTO "types" VALUES (9,'QByteArray::Base64Option','QByteArrayBase64Option','',1,0,NULL);
INSERT INTO "types" VALUES (10,'QByteRef','QByteRef','',0,1,NULL);
INSERT INTO "types" VALUES (11,'QString::NormalizationForm','QStringNormalizationForm','',1,0,NULL);
INSERT INTO "types" VALUES (12,'QString::SplitBehavior','QStringSplitBehavior','',1,0,NULL);
INSERT INTO "types" VALUES (13,'QLatin1String','QLatin1String','',0,1,NULL);
INSERT INTO "types" VALUES (14,'QString','QString','',0,1,NULL);
INSERT INTO "types" VALUES (15,'QString::SectionFlag','QStringSectionFlag','',1,0,NULL);
INSERT INTO "types" VALUES (16,'QString::Null','QStringNull','',0,1,NULL);
INSERT INTO "types" VALUES (17,'QCharRef','QCharRef','',0,1,NULL);
INSERT INTO "types" VALUES (18,'QStringRef','QStringRef','',0,1,NULL);
INSERT INTO "types" VALUES (19,'QLatin1Char','QLatin1Char','',0,1,NULL);
INSERT INTO "types" VALUES (20,'QChar::Category','QCharCategory','',1,0,NULL);
INSERT INTO "types" VALUES (21,'QChar','QChar','',0,1,NULL);
INSERT INTO "types" VALUES (22,'QChar::JoiningType','QCharJoiningType','',1,0,NULL);
INSERT INTO "types" VALUES (23,'QChar::Script','QCharScript','',1,0,NULL);
INSERT INTO "types" VALUES (24,'QChar::SpecialCharacter','QCharSpecialCharacter','',1,0,NULL);
INSERT INTO "types" VALUES (25,'QChar::Decomposition','QCharDecomposition','',1,0,NULL);
INSERT INTO "types" VALUES (26,'QChar::UnicodeVersion','QCharUnicodeVersion','',1,0,NULL);
INSERT INTO "types" VALUES (27,'QChar::Direction','QCharDirection','',1,0,NULL);
INSERT INTO "types" VALUES (28,'QChar::Joining','QCharJoining','',1,0,NULL);
INSERT INTO "types" VALUES (29,'QChar::CombiningClass','QCharCombiningClass','',1,0,NULL);
COMMIT;
