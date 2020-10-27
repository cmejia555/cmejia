DROP TABLE IF EXISTS SongName;

CREATE TABLE IF NOT EXISTS SongName (
id INT NOT NULL AUTO_INCREMENT,
word1 VARCHAR(50) NOT NULL DEFAULT '',
word2 VARCHAR(50) NOT NULL DEFAULT '',
word3 VARCHAR(50) NOT NULL DEFAULT '',
rating TINYINT NOT NULL DEFAULT 0,
type ENUM ('funny', 'boring', 'vague', 'offensive'),
PRIMARY KEY (id)
) ENGINE = MyISAM;

-- Aca corremos una query de inserción de medio millón de registros.
-- INSERT INTO BandNames (word1, word2, word3, rating, type) VALUES (...);

SHOW TABLE STATUS;

SELECT * FROM SongName
WHERE word1 LIKE 'a%' AND word2 LIKE 'b%';

SELECT s1.*, s2.id, s2.word2, s3.id, s3.word3
FROM SongName AS s1
JOIN SongName AS s2 ON (s1.word3 = s2.word2)
JOIN SongName AS s3 ON (s1.word2 = s3.word3)
WHERE s1.word1 LIKE 'abb%' AND s1.rating = 10
GROUP BY s1.id;

-- Add indexes
ALTER TABLE SongName
ADD INDEX word1_idx (word1(50)),
ADD INDEX word2_idx (word2(50)),
ADD INDEX word3_idx (word3(50));

ALTER TABLE SongName
ADD INDEX rating_idx (rating);

ALTER TABLE SongName
ADD INDEX word1_first_idx (word1(3)),
ADD INDEX word2_first_idx (word2(3)),
ADD INDEX word3_first_idx (word3(3));

ALTER TABLE SongName
ADD UNIQUE INDEX word_dup_idx (word1, word2, word3);

ALTER TABLE SongName
ADD INDEX word12_idx (word1, word2),
ADD INDEX word23_idx (word2, word3),
ADD INDEX word31_idx (word3, word1);

ALTER TABLE SongName ADD INDEX word12_idx (word1, word2);


-- Drop all indexes
ALTER TABLE SongName
DROP INDEX word1_idx,
DROP INDEX word2_idx,
DROP INDEX word3_idx;

ALTER TABLE SongName
DROP INDEX word1_first_idx,
DROP INDEX word2_first_idx,
DROP INDEX word3_first_idx;

ALTER TABLE SongName DROP INDEX word_dup_idx;
ALTER TABLE SongName DROP INDEX rating_idx;

ALTER TABLE SongName DROP INDEX word12_idx, DROP INDEX word23_idx, DROP INDEX word3_idx;
