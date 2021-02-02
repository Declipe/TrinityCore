CREATE TABLE IF NOT EXISTS `auth3`.`account_premium` (
  `id` int(11) NOT NULL DEFAULT '0' COMMENT 'Account id',
  `setdate` int(4) NOT NULL DEFAULT '0',
  `unsetdate` int(4) NOT NULL DEFAULT '0',
  `premium_type` tinyint(4) unsigned NOT NULL DEFAULT '1',
  `active` tinyint(4) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `active` (`active`),
  KEY `setdate` (`setdate`),
  KEY `unsetdate` (`unsetdate`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DELETE FROM `rbac_permissions` WHERE `Id` IN (1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020);
INSERT INTO `rbac_permissions` VALUES
('1000', 'vip'),
('1001', 'vip debuff'),
('1002', 'vip bank'),
('1003', 'vip repair'),
('1004', 'vip resettalents'),
('1005', 'vip taxa'),
('1006', 'vip home'),
('1007', 'vip capital'),
('1008', 'vip mail'),
('1009', 'vip changerace'),
('1010', 'vip customize'),
('1011', 'vip appear'),
('1012', 'vip map'),
('1013', 'vip buffs'),
('1014', 'vip arena'),
('1015', 'vip warsong'),
('1016', 'vip arathi'),
('1017', 'vip eye'),
('1018', 'vip alterac'),
('1019', 'vip teles'),
('1020', 'vip qcomplete');

DELETE FROM `rbac_linked_permissions` WHERE `linkedId` IN (1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1017, 1018, 1019, 1020);
INSERT INTO `rbac_linked_permissions` VALUES
('195','1000'),
('195','1001'),
('195','1002'),
('195','1003'),
('195','1004'),
('195','1005'),
('195','1006'),
('195','1007'),
('195','1008'),
('195','1009'),
('195','1010'),
('195','1011'),
('195','1012'),
('195','1013'),
('195','1014'),
('195','1015'),
('195','1016'),
('195','1017'),
('195','1018'),
('195','1019'),
('195','1020');