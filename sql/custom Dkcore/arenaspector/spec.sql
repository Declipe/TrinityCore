--
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES
('1030', 'spectate'),
('1031', 'spectate player'),
('1032', 'spectate view'),
('1033', 'spectate reset'),
('1034', 'spectate leave');

DELETE FROM `rbac_linked_permissions` WHERE `linkedId` IN ("1003","1004","1005","1006","1007");

INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES
('195','1030'),
('195','1031'),
('195','1032'),
('195','1033'),
('195','1034');