-- Select: http://dev.mysql.com/doc/refman/5.5/en/select.html

-- Select como cláusula universal de manejo de datos
SELECT 1+1;
SELECT NOW(), UNIX_TIMESTAMP();
SELECT GREATEST(4,5,6,99,8,4,3,12,105);
SELECT 1+1 AS 'Calculo',
    NOW() AS 'Fecha',
    GREATEST(4,5,6,99,8,4,3,12,105) AS 'Maximo';

-- Usando tablas
EXPLAIN Artist;
SELECT * FROM Artist;
SELECT Name AS 'Artista' FROM Artist;
SELECT Artist.Name FROM Artist;
SELECT Artist.Name, LENGTH(Artist.Name) FROM Artist;
SELECT COUNT(*) AS 'Total' FROM Artist;

-- Procesando datos
SELECT Artist.Name, LENGTH(Artist.Name) FROM Artist;

SELECT Artist.Name, LENGTH(Artist.Name) AS 'Length'
FROM Artist
WHERE LENGTH(Artist.Name) <= 5
ORDER BY Artist.Name;

SELECT CONCAT("El artista ", Artist.Name, " tiene como ID el numero ", ArtistId) FROM Artist;

-- Distintos
EXPLAIN Invoice;
SELECT BillingCountry FROM Invoice ORDER BY BillingCountry;
SELECT DISTINCT(BillingCountry) FROM Invoice ORDER BY BillingCountry;
SELECT COUNT(BillingCountry) FROM Invoice;
SELECT COUNT(DISTINCT BillingCountry) FROM Invoice;

-- Filtrado
SELECT * FROM Artist WHERE Name = 'Black Sabbath';
SELECT * FROM Artist WHERE Name LIKE 'Black%';
SELECT * FROM Artist WHERE Name LIKE '%Black%';
SELECT * FROM Artist WHERE Name NOT LIKE 'Black%';
SELECT * FROM Artist WHERE ArtistId IN (1,5,15,23,42,44,55,200,7981765265);
SELECT * FROM Artist WHERE ArtistId NOT IN (1,5,15,23,42,44,55,200,7981765265);
SELECT * FROM Artist WHERE ArtistId BETWEEN 1 AND 20;
SELECT * FROM Artist WHERE Name BETWEEN 'A' AND 'B' ORDER BY Name;
SELECT * FROM Artist WHERE Name BETWEEN 'A' AND 'C' ORDER BY Name;
SELECT * FROM Artist WHERE ArtistId % 2 = 0;
SELECT * FROM Artist WHERE ArtistId % 2 != 0;
SELECT * FROM Artist WHERE ArtistId % 10 = 0;

-- Operadores lógicos
SELECT * FROM Artist WHERE Name LIKE '%black%' OR Name LIKE '%white%';
SELECT * FROM Artist WHERE NOT(Name LIKE '%A%' OR Name LIKE '%E%');
SELECT * FROM Artist WHERE Name NOT LIKE '%A%' AND Name NOT LIKE '%E%';
SELECT * FROM Artist WHERE ArtistId IN (1,5,15,23,42,44,55,200) AND LENGTH(Name) > 10;

SELECT * FROM Artist WHERE ArtistId % 2 = 0 XOR ArtistId % 3 = 0
ORDER BY ArtistId LIMIT 20;

SELECT * FROM Artist WHERE NOT(ArtistId % 2 = 0 XOR ArtistId % 3 = 0)
ORDER BY ArtistId LIMIT 20;


--- Agregadores
EXPLAIN Invoice;
SELECT Total FROM Invoice;

SELECT SUM(Total) FROM Invoice;

SELECT AVG(Total) AS 'Promedio',
MIN(Total) AS 'Minimo',
MAX(Total) AS 'Maximo',
SUM(Total) AS 'Suma'
FROM Invoice;

SELECT AVG(Total) AS 'Promedio',
MIN(Total) AS 'Minimo',
MAX(Total) AS 'Maximo',
SUM(Total) AS 'Suma',
COUNT(Total) AS 'Facturas'
FROM Invoice WHERE BillingCountry = 'Brazil';

-- Multiples tablas

--- Sin relacion
SELECT Name, Title FROM Artist, Album;

--- Columna ambigua
SELECT Name, Title FROM Artist, Album ORDER BY ArtistId;

--- Inner join implícito
SELECT Artist.Name, Album.Title FROM Artist, Album
WHERE Artist.ArtistId = Album.ArtistId
ORDER BY Artist.Name;

--- Inner join explícito
SELECT Artist.Name, Album.Title
FROM Artist
JOIN Album ON (Artist.ArtistId = Album.ArtistId)
ORDER BY Artist.Name;

--- Agrupación
SELECT
BillingCountry,
AVG(Total) AS 'Promedio',
MIN(Total) AS 'Minimo',
MAX(Total) AS 'Maximo',
SUM(Total) AS 'Suma',
COUNT(Total) AS 'Facturas'
FROM Invoice
GROUP BY BillingCountry
ORDER BY BillingCountry;

SELECT Artist.Name, COUNT(Album.AlbumId) AS 'Discos'
FROM Artist
JOIN Album ON (Artist.ArtistId = Album.ArtistId)
GROUP BY Artist.ArtistId
ORDER BY Artist.Name;

--- -- --
--- Insert, update, delete
--- -- --
INSERT INTO Artist (Name) VALUES ('The Ramones');
INSERT INTO Album (ArtistId, Title) VALUES (?, 'Mondo Bizarro');
INSERT INTO Album (ArtistId, Title) VALUES (?, 'Adios Amigos!');

UPDATE Artist SET Name = 'Los Ramones' WHERE ArtistId = ?;

DELETE FROM Artist WHERE Name = 'The Ramones';

