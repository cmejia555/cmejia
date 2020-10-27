-- Comienza una transacción
BEGIN;

SELECT * FROM Artist;

INSERT INTO Artist (Name) VALUES ('Uno'),('Dos'),('Tres'),('Cuatro');

DELETE FROM Artist;

SELECT * FROM Artist;

-- Vuelve todo para atrás
ROLLBACK;

-- !!!! TODO lo que esté fuera de una transacción es irreversible

-- Comienza una nueva transacción
BEGIN;

INSERT INTO Artist (Name) VALUES ('Uno'),('Dos'),('Tres'),('Cuatro');

COMMIT;



INSERT INTO Artist (Name) VALUES ('El primero');