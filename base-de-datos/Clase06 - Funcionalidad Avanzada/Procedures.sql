### Store procedures
-- Crea un procedure que es como un view, pero con un filtro obligatorio
DELIMITER //
CREATE PROCEDURE albums_by_artist (IN artist_name VARCHAR(50))
BEGIN
	SELECT Title
	FROM Album
	WHERE ArtistId = (SELECT ArtistId FROM Artist WHERE Name = artist_name);
END	//
DELIMITER ;

-- Crea un procedure para facilitar el insert
DELIMITER //
CREATE PROCEDURE add_new_album (IN artist_name VARCHAR(50), IN album_title VARCHAR(50))
BEGIN
	SET @ArtistId = (SELECT ArtistId FROM Artist WHERE Name = artist_name);
	INSERT INTO Album(Title, ArtistId) VALUES (album_title, @ArtistId);
END	//
DELIMITER ;

-- Usa ambos procedures
CALL add_new_album('AC/DC', 'Ballbreaker');
CALL add_new_album('AC/DC', 'Otro album #2');
CALL add_new_album('AC/DC', 'Otro album #3');
CALL add_new_album('AC/DC', 'Otro album #4');

CALL albums_by_artist('AC/DC');

### Procedures que devuelven valores

-- Consulta para obtener el artista con mayores ventas
SELECT Artist.ArtistId, Artist.Name, 
SUM(InvoiceLine.UnitPrice*InvoiceLine.Quantity) AS 'Total'
FROM InvoiceLine
JOIN Track ON (InvoiceLine.TrackId = Track.TrackId)
JOIN Album ON (Track.AlbumId = Album.AlbumId)
JOIN Artist ON (Artist.ArtistId = Album.ArtistId)
GROUP BY Artist.ArtistId
ORDER BY Total DESC;

-- Crea un procedure con esa misma consulta
DELIMITER //
CREATE PROCEDURE get_best_selling_artist (OUT ArtistId INT)
BEGIN
SET ArtistId = (SELECT Artist.ArtistId FROM InvoiceLine
JOIN Track ON (InvoiceLine.TrackId = Track.TrackId)
JOIN Album ON (Track.AlbumId = Album.AlbumId)
JOIN Artist ON (Artist.ArtistId = Album.ArtistId)
GROUP BY Artist.ArtistId
ORDER BY SUM(InvoiceLine.UnitPrice*InvoiceLine.Quantity) DESC
LIMIT 1);
END	//
DELIMITER ;

-- Usa el procedure con variables tipo "OUT"
CALL get_best_selling_artist(@id);
SELECT @id;
SELECT * FROM Artist WHERE ArtistId = @id;

-- Status
SHOW PROCEDURE STATUS WHERE db = 'Chinook';
-- DROP PROCEDURE borra un store procedure

-- Tarea para el hogar: investigar CREATE FUNCTION