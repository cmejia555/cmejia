### SUBSELECT

# Subselect con sólo un elemento
SELECT * FROM Album WHERE ArtistId IN (
	SELECT ArtistId FROM Artist
);

# Subselect con múltiples elementos
SELECT * FROM Album WHERE ArtistId IN (
	SELECT ArtistId FROM Artist WHERE Name LIKE 'A%'
);

# Falla si está mal construido
SELECT * FROM Album WHERE ArtistId IN (
	SELECT ArtistId, Name FROM Artist WHERE Name LIKE 'A%'
);

# Subselect como resultado
SELECT Artist.ArtistId, Artist.Name,
(SELECT COUNT(*) FROM Album WHERE Album.ArtistId = Artist.ArtistId) AS "Albums"
FROM Artist
ORDER BY Artist.Name

### UNIONES

-- La cantidad y el tipo de columnas debe coincidir
-- Luego del where: el group, order, limit, etc se aplica posteriormente a realizarse la unión.
SELECT Artist.ArtistId AS 'Id', Artist.Name AS 'Name', 'Artist' AS 'Type'
FROM Artist
WHERE Artist.Name LIKE 'A%'
UNION
SELECT Album.AlbumId AS 'Id', Album.Title AS 'Name', 'Album' AS 'Type'
FROM Album
WHERE Album.Title LIKE 'A%'
UNION
SELECT Track.TrackId AS 'Id', Track.Name AS 'Name', 'Song' AS 'Type'
FROM Track
WHERE Track.Name LIKE 'A%'
ORDER BY Type, Name;

### HAVING

# Sin Having
SELECT Country.CountryID, Country.Name,
COUNT(Invoice.InvoiceId) AS 'Invoices',
COUNT(DISTINCT Customer.CustomerID) AS 'Customers'
FROM Invoice
JOIN Customer ON (Invoice.CustomerId = Customer.CustomerId)
RIGHT JOIN Country ON (Invoice.CountryId = Country.CountryId)
GROUP BY Country.CountryId
ORDER BY Country.Name;

# Having (o "el where del group")
SELECT Country.CountryID, Country.Name,
COUNT(Invoice.InvoiceId) AS 'Invoices',
COUNT(DISTINCT Customer.CustomerID) AS 'Customers'
FROM Invoice
JOIN Customer ON (Invoice.CustomerId = Customer.CustomerId)
RIGHT JOIN Country ON (Invoice.CountryId = Country.CountryId)
GROUP BY Country.CountryId
HAVING Invoices >= 10
ORDER BY Country.Name;
-- O bien HAVING COUNT(Invoice.InvoiceId) >= 10
-- Sirve para todas las funciones de agregación (count, sum, avg, etc).

# Having y where combinados
SELECT Country.CountryID, Country.Name,
COUNT(Invoice.InvoiceId) AS 'Invoices',
COUNT(DISTINCT Customer.CustomerID) AS 'Customers'
FROM Invoice
JOIN Customer ON (Invoice.CustomerId = Customer.CustomerId)
RIGHT JOIN Country ON (Invoice.CountryId = Country.CountryId)
WHERE Country.Name LIKE 'U%'
GROUP BY Country.CountryId
HAVING Invoices >= 10
ORDER BY Country.Name;