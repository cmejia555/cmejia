CREATE OR REPLACE VIEW `view_all_albums` AS
SELECT LEFT(Artist.Name,30) AS 'Artist',
LEFT(Album.Title,30) AS 'Album',
COUNT(DISTINCT Track.TrackId) AS 'Tracks'
FROM Artist
INNER JOIN Album ON (Artist.ArtistId = Album.ArtistId)
LEFT JOIN Track ON (Album.AlbumId = Track.AlbumId)
GROUP BY Album.AlbumId
ORDER BY Artist.Name ASC, Album.Title ASC;

# Mostrar todas las vistas en la base
SHOW FULL TABLES IN Chinook WHERE TABLE_TYPE LIKE 'VIEW';

# Describir una vista
EXPLAIN view_all_albums;