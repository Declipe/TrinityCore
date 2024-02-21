DELETE FROM `rbac_permissions` WHERE `Id` IN (1116, 1117);
INSERT INTO `rbac_permissions` VALUES
('1116', 'Command: Vip set'),
('1117', 'Command: Vip del');
DELETE FROM `rbac_linked_permissions` WHERE `linkedId` IN (1116, 1117);
INSERT INTO `rbac_linked_permissions` VALUES
('192','1116'),
('192','1117');