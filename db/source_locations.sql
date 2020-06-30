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
COMMIT;
