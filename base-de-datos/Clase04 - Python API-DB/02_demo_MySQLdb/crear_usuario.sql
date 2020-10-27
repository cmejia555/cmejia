CREATE USER 'guest'@'localhost' IDENTIFIED BY 'guest';
GRANT ALL PRIVILEGES ON Chinook.* TO 'guest'@'localhost';
FLUSH PRIVILEGES;
