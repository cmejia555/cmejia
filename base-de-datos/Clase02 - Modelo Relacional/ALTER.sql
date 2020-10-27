CREATE TABLE Country (
	CountryID INT NOT NULL AUTO_INCREMENT,
	Name VARCHAR(255) NOT NULL DEFAULT '',
	PRIMARY KEY (CountryID)
);

SELECT DISTINCT BillingCountry FROM Invoice ORDER BY BillingCountry;

INSERT INTO Country (Name)
SELECT DISTINCT BillingCountry FROM Invoice ORDER BY BillingCountry;

ALTER TABLE Invoice
ADD CountryID INT NOT NULL DEFAULT 0 AFTER BillingState;

SELECT InvoiceId, BillingCountry, CountryID FROM Invoice LIMIT 10;

UPDATE Invoice
JOIN Country ON (Invoice.BillingCountry = Country.Name)
SET Invoice.CountryID = Country.CountryID;

ALTER TABLE Invoice
DROP BillingCountry;

ALTER TABLE Invoice
CHANGE CountryID CountryID INT NOT NULL;

SELECT InvoiceId, Invoice.CountryID, Country.CountryID, Country.Name, Invoice.Total
FROM Invoice
JOIN Country ON (Invoice.CountryID = Country.CountryID)
LIMIT 10;


SELECT
Country.Name,
AVG(Total) AS 'Promedio',
MIN(Total) AS 'Minimo',
MAX(Total) AS 'Maximo',
SUM(Total) AS 'Suma',
COUNT(Total) AS 'Facturas'
FROM Invoice
JOIN Country ON (Invoice.CountryID = Country.CountryID)
GROUP BY Invoice.CountryId
ORDER BY Country.Name;