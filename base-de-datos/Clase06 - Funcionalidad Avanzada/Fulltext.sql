# SOUNDEX
SELECT * FROM Artist WHERE SOUNDEX(Name) = SOUNDEX('Block Sebbat');
SELECT * FROM Artist WHERE Name SOUNDS LIKE 'Block Sebbat';

# Fulltext
ALTER TABLE Artist ADD FULLTEXT(Name);
ALTER TABLE Album ADD FULLTEXT(Title);

# Natural language (como LIKE '%%')
SELECT Artist.Name, Album.Title
FROM Artist
JOIN Album ON (Album.ArtistId = Artist.ArtistId)
WHERE MATCH(Artist.Name) AGAINST ('Black' IN NATURAL LANGUAGE MODE)
OR MATCH(Album.Title) AGAINST ('Black' IN NATURAL LANGUAGE MODE);

# Boolean mode, o "match exacto"
SELECT Artist.Name, Album.Title
FROM Artist
LEFT JOIN Album ON (Album.ArtistId = Artist.ArtistId)
WHERE
MATCH(Artist.Name) AGAINST ('Black White* System' IN BOOLEAN MODE);

# Boolean mode inclusivo/exclusivo
SELECT Artist.Name, Album.Title
FROM Artist
LEFT JOIN Album ON (Album.ArtistId = Artist.ArtistId)
WHERE
MATCH(Artist.Name) AGAINST ('Black White* System -Label' IN BOOLEAN MODE);

# Boolean mode operators
SELECT Artist.Name, Album.Title
FROM Artist
LEFT JOIN Album ON (Album.ArtistId = Artist.ArtistId)
WHERE
MATCH(Artist.Name) AGAINST ('Black White* System -Label +Sabbath' IN BOOLEAN MODE);

# Buscando discos en vivo
SELECT Artist.Name, Album.AlbumId, Album.Title
FROM Artist
JOIN Album ON (Album.ArtistId = Artist.ArtistId)
WHERE MATCH(Album.Title) AGAINST('Live' IN NATURAL LANGUAGE MODE);

# Buscando discos en vivo expandiendo a similares
SELECT Artist.Name, Album.AlbumId, Album.Title
FROM Artist
JOIN Album ON (Album.ArtistId = Artist.ArtistId)
WHERE MATCH(Album.Title)
	AGAINST('Live' IN NATURAL LANGUAGE MODE WITH QUERY EXPANSION);

# Lo mismo, ordenando resultados por "mejores"
SELECT Artist.Name, Album.AlbumId, Album.Title,
	MATCH(Album.Title)
	AGAINST('Live' IN NATURAL LANGUAGE MODE WITH QUERY EXPANSION) AS score
FROM Artist
JOIN Album ON (Album.ArtistId = Artist.ArtistId)
WHERE MATCH(Album.Title)
	AGAINST('Live' IN NATURAL LANGUAGE MODE WITH QUERY EXPANSION)
ORDER BY score DESC;

# Lo mismo, pero filtrando resultados menos relevantes
SELECT Artist.Name, Album.AlbumId, Album.Title,
	MATCH(Album.Title)
	AGAINST('Live' IN NATURAL LANGUAGE MODE WITH QUERY EXPANSION) AS score
FROM Artist
JOIN Album ON (Album.ArtistId = Artist.ArtistId)
WHERE MATCH(Album.Title)
	AGAINST('Live' IN NATURAL LANGUAGE MODE WITH QUERY EXPANSION)
AND MATCH(Album.Title)
	AGAINST('Live' IN NATURAL LANGUAGE MODE WITH QUERY EXPANSION) >= 5.0
ORDER BY score DESC;