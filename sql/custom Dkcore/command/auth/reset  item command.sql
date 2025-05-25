
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (4000, 'Command: reset items equipped');
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (4001, 'Command: reset items bags');
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (4002, 'Command: reset items bank');
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (4003, 'Command: reset items keyring');
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (4004, 'Command: reset items currency');
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (4005, 'Command: reset items vendor_buyback');
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (4006, 'Command: reset items all');
INSERT INTO `rbac_permissions` (`id`, `name`) VALUES (4007, 'Command: reset items allbags');

INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (192, 4000);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (192, 4001);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (192, 4002);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (192, 4003);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (192, 4004);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (192, 4005);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (192, 4006);
INSERT INTO `rbac_linked_permissions` (`id`, `linkedId`) VALUES (192, 4007);
