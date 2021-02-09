

DELETE FROM `command` WHERE `name`='reload full';
INSERT INTO `command` (`name`,`help`) VALUES
('reload full', "Syntax: .reload full

Reloads full template definitions from the database.");
