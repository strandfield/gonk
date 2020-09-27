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
INSERT INTO "types" VALUES (8,'QByteArray','QByteArray',0,1);
INSERT INTO "types" VALUES (9,'QByteArray::Base64Option','QByteArrayBase64Option',1,0);
INSERT INTO "types" VALUES (10,'QByteArray::Base64DecodingStatus','QByteArrayBase64DecodingStatus',1,0);
INSERT INTO "types" VALUES (11,'QByteArray::FromBase64Result','QByteArrayFromBase64Result',0,1);
INSERT INTO "types" VALUES (12,'QByteRef','QByteRef',0,1);
INSERT INTO "types" VALUES (13,'FromBase64Result','FromBase64Result',0,1);
INSERT INTO "types" VALUES (14,'QLatin1Char','QLatin1Char',0,1);
INSERT INTO "types" VALUES (15,'QChar::Direction','QCharDirection',1,0);
INSERT INTO "types" VALUES (16,'QChar','QChar',0,1);
INSERT INTO "types" VALUES (17,'QChar::SpecialCharacter','QCharSpecialCharacter',1,0);
INSERT INTO "types" VALUES (18,'QChar::Category','QCharCategory',1,0);
INSERT INTO "types" VALUES (19,'QChar::Script','QCharScript',1,0);
INSERT INTO "types" VALUES (20,'QChar::Decomposition','QCharDecomposition',1,0);
INSERT INTO "types" VALUES (21,'QChar::JoiningType','QCharJoiningType',1,0);
INSERT INTO "types" VALUES (22,'QChar::Joining','QCharJoining',1,0);
INSERT INTO "types" VALUES (23,'QChar::CombiningClass','QCharCombiningClass',1,0);
INSERT INTO "types" VALUES (24,'QChar::UnicodeVersion','QCharUnicodeVersion',1,0);
INSERT INTO "types" VALUES (25,'QString','QString',0,1);
INSERT INTO "types" VALUES (26,'QString::is_convertible_to_view_or_qstring_helper','QStringis_convertible_to_view_or_qstring_helper',0,1);
INSERT INTO "types" VALUES (27,'QLatin1String','QLatin1String',0,1);
INSERT INTO "types" VALUES (28,'QString::SectionFlag','QStringSectionFlag',1,0);
INSERT INTO "types" VALUES (29,'QString::NormalizationForm','QStringNormalizationForm',1,0);
INSERT INTO "types" VALUES (30,'QString::is_convertible_to_view_or_qstring','QStringis_convertible_to_view_or_qstring',0,1);
INSERT INTO "types" VALUES (31,'QString::Null','QStringNull',0,1);
INSERT INTO "types" VALUES (32,'QString::SplitBehavior','QStringSplitBehavior',1,0);
INSERT INTO "types" VALUES (33,'QCharRef','QCharRef',0,1);
INSERT INTO "types" VALUES (34,'QStringRef','QStringRef',0,1);
INSERT INTO "types" VALUES (35,'QDir::SortFlag','QDirSortFlag',1,0);
INSERT INTO "types" VALUES (36,'QDir','QDir',0,1);
INSERT INTO "types" VALUES (37,'QDir::Filter','QDirFilter',1,0);
INSERT INTO "types" VALUES (38,'QObject','QObject',0,1);
INSERT INTO "types" VALUES (39,'QSignalBlocker','QSignalBlocker',0,1);
COMMIT;
