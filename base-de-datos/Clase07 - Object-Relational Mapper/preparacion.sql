DROP DATABASE IF EXISTS ormtest;
DROP USER IF EXISTS 'guest'@'localhost';
CREATE DATABASE ormtest;
CREATE USER 'guest'@'localhost' IDENTIFIED BY 'guest';
GRANT ALL ON ormtest.* TO 'guest'@'localhost';
