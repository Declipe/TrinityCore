INSERT INTO `rbac_permissions` VALUES
('1114', 'Command: AddCoin');
DELETE FROM `rbac_linked_permissions` WHERE `linkedId` IN (1114);
INSERT INTO `rbac_linked_permissions` VALUE
('192','1114');