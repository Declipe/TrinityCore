DELETE FROM `rbac_permissions` WHERE `id`=1021;
INSERT INTO `rbac_permissions` (`id`,`name`) VALUES
(1021, 'Command: reload full');

DELETE FROM `rbac_linked_permissions` WHERE `linkedId`=881;
INSERT INTO `rbac_linked_permissions` (`id`,`linkedId`) VALUES
(196,1021);
