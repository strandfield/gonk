BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "metadata" (
	"id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"entity_id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"value"	TEXT,
	FOREIGN KEY("entity_id") REFERENCES "entities"("id")
);
INSERT INTO "metadata" VALUES (2,95,'genwrapper','true');
INSERT INTO "metadata" VALUES (3,94,'genwrapper','true');
INSERT INTO "metadata" VALUES (4,96,'genwrapper','true');
INSERT INTO "metadata" VALUES (7,989,'genwrapper','true');
INSERT INTO "metadata" VALUES (8,990,'genwrapper','true');
INSERT INTO "metadata" VALUES (9,991,'genwrapper','true');
INSERT INTO "metadata" VALUES (10,917,'genwrapper','true');
INSERT INTO "metadata" VALUES (11,918,'genwrapper','true');
INSERT INTO "metadata" VALUES (12,919,'genwrapper','true');
INSERT INTO "metadata" VALUES (13,920,'genwrapper','true');
INSERT INTO "metadata" VALUES (14,921,'genwrapper','true');
INSERT INTO "metadata" VALUES (15,1576,'implementation','"bytearray_string_ctor"');
INSERT INTO "metadata" VALUES (16,97,'genwrapper','true');
INSERT INTO "metadata" VALUES (17,1610,'genwrapper','true');
INSERT INTO "metadata" VALUES (18,1611,'genwrapper','true');
COMMIT;
