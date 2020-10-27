-- Agregamos algunos países a la tabla.
INSERT INTO Country (Name) VALUES ('Uganda'), ('Afghanistan'), ('Albania'), 
('Algeria'), ('Andorra'), ('Angola'), ('Antigua and Barbuda'), ('Armenia'), ('Aruba'), 
('Azerbaijan');


-- JOIN, INNER JOIN, NATURAL INNER JOIN
SELECT Country.CountryID, Country.Name,
COUNT(Invoice.InvoiceId) AS 'Invoices'
FROM Country
INNER JOIN Invoice ON (Invoice.CountryId = Country.CountryId)
GROUP BY Country.CountryId
ORDER BY Country.Name;


-- LEFT JOIN, LEFT OUTER JOIN
SELECT Country.CountryID, Country.Name, COUNT(Invoice.InvoiceId) AS 'Invoices'
FROM Country
LEFT JOIN Invoice ON (Invoice.CountryId = Country.CountryId)
GROUP BY Country.CountryId
ORDER BY Country.Name;


-- RIGHT JOIN, RIGHT OUTER JOIN
SELECT Country.CountryID, Country.Name, COUNT(Invoice.InvoiceId) AS 'Invoices'
FROM Invoice
RIGHT JOIN Country ON (Invoice.CountryId = Country.CountryId)
GROUP BY Country.CountryId
ORDER BY Country.Name;


-- Multiple JOINS
SELECT Country.CountryID, Country.Name,
COUNT(Invoice.InvoiceId) AS 'Invoices',
COUNT(DISTINCT Customer.CustomerID) AS 'Customers'
FROM Invoice
INNER JOIN Customer ON (Invoice.CustomerId = Customer.CustomerId)
RIGHT JOIN Country ON (Invoice.CountryId = Country.CountryId)
GROUP BY Country.CountryId
ORDER BY Country.Name;


-- El orden de los joins es muy importante
SELECT Country.CountryID, Country.Name,
COUNT(Invoice.InvoiceId) AS 'Invoices',
COUNT(DISTINCT Customer.CustomerID) AS 'Customers'
FROM Invoice
RIGHT JOIN Country ON (Invoice.CountryId = Country.CountryId)
JOIN Customer ON (Invoice.CustomerId = Customer.CustomerId)
GROUP BY Country.CountryId
ORDER BY Country.Name;