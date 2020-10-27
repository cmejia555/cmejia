-- Creamos la tabla country, nos aseguramos que el engine sea InnoDB
CREATE TABLE Country (
	CountryId INT NOT NULL AUTO_INCREMENT,
	Name VARCHAR(255) NOT NULL DEFAULT '',
	PRIMARY KEY (CountryId)
) ENGINE = InnoDB;

-- Nos aseguramos que el motor de invoice sea InnoDB
-- ALTER TABLE Invoice ENGINE = InnoDB;

-- Insertamos todos los paises
INSERT INTO Country (Name)
SELECT DISTINCT BillingCountry FROM Invoice ORDER BY BillingCountry;

-- Agregamos la columna en invoice
ALTER TABLE Invoice
	ADD CountryId INT NOT NULL DEFAULT 0;

-- Actualizamos el CountryId en invoice
UPDATE Invoice
JOIN Country ON (Invoice.BillingCountry = Country.Name)
SET Invoice.CountryId = Country.CountryId;

-- Eliminamos redundancia y nos ponemos estrictos con el campo CountryId
ALTER TABLE Invoice
DROP BillingCountry;
CHANGE CountryId CountryId INT NOT NULL;

-- Declaro que CountryId es un FOREIGN KEY de la tabla Country
ALTER TABLE Invoice
    ADD CONSTRAINT invoice_country_id
    FOREIGN KEY(CountryId) REFERENCES Country(CountryId)
    ON UPDATE CASCADE
    ON DELETE CASCADE;

-- tambien puede ser SET NULL o NO ACTION
-- suele definirse en un alter para evitar tener que crear las tablas en cierto orden.

-- De ahora en adelante, la integridad referencial está garantizada
-- Esto falla
DROP TABLE Country;

-- No me deja insertar valores
INSERT INTO Invoice (CustomerId, CountryId) VALUE (1, 999999999999);

-- Elimina en cascada
DELETE FROM Country WHERE CountryId = 1;
