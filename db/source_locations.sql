BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "source_locations" (
	"entity_id"	INTEGER NOT NULL,
	"file_id"	INTEGER NOT NULL,
	"line"	INTEGER,
	"column"	INTEGER,
	PRIMARY KEY("entity_id"),
	FOREIGN KEY("entity_id") REFERENCES "entities"("id"),
	FOREIGN KEY("file_id") REFERENCES "files"("id")
);
INSERT INTO "source_locations" VALUES (1567,1,237,1);
INSERT INTO "source_locations" VALUES (1568,1,238,1);
INSERT INTO "source_locations" VALUES (1569,1,238,1);
INSERT INTO "source_locations" VALUES (1570,1,238,1);
INSERT INTO "source_locations" VALUES (1571,1,239,1);
INSERT INTO "source_locations" VALUES (1572,1,239,1);
INSERT INTO "source_locations" VALUES (1573,1,239,1);
INSERT INTO "source_locations" VALUES (1574,1,243,22);
INSERT INTO "source_locations" VALUES (1575,1,244,22);
COMMIT;
