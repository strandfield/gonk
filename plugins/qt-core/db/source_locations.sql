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
INSERT INTO "source_locations" VALUES (1577,2,127,21);
INSERT INTO "source_locations" VALUES (1581,2,129,5);
INSERT INTO "source_locations" VALUES (1582,2,129,5);
INSERT INTO "source_locations" VALUES (1589,2,153,17);
INSERT INTO "source_locations" VALUES (1590,2,154,17);
INSERT INTO "source_locations" VALUES (1591,2,156,17);
INSERT INTO "source_locations" VALUES (1596,2,165,9);
INSERT INTO "source_locations" VALUES (1598,2,216,31);
INSERT INTO "source_locations" VALUES (1607,2,355,17);
INSERT INTO "source_locations" VALUES (1608,2,358,17);
INSERT INTO "source_locations" VALUES (1620,2,425,21);
INSERT INTO "source_locations" VALUES (1621,2,427,17);
INSERT INTO "source_locations" VALUES (1623,2,554,22);
INSERT INTO "source_locations" VALUES (1624,2,557,7);
COMMIT;
