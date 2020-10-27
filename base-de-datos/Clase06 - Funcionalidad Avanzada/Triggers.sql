-- Sólo para MySQL 5.5+

-- Trigger para forzar que los nombres de artistas sean todos en mayúsculas
DELIMITER //
CREATE TRIGGER uppercase_artist BEFORE INSERT ON Artist 
FOR EACH ROW
BEGIN
	SET NEW.Name = UPPER(NEW.Name);
END//
DELIMITER ;

-- Inserta un par en minúsculas
INSERT INTO Artist(Name) VALUES('minuscula uno'),('minuscula dos');

SELECT * FROM Artist;

-- Puede ser BEFORE INSERT, BEFORE UPDATE, BEFORE DELETE
-- o AFTER INSERT, AFTER UPDATE, AFTER DELETE

-- BEFORE se usan para validaciones o transformacion de datos
-- AFTER se usan para propagación de datos (históricos, denormalizaciones)

-- Trigger de validación.
DELIMITER //
CREATE TRIGGER no_cordera BEFORE INSERT ON Artist
FOR EACH ROW
	BEGIN
		IF(NEW.Name = 'Pelado Cordera') THEN
			SIGNAL SQLSTATE '45000';
			-- SET MESSAGE_TEXT = 'Muere Pelado, muere!';
		END IF;
	END//
DELIMITER ;

INSERT INTO Artist(Name) VALUES ('Pelado Cordera');


