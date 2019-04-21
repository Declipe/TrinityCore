INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('930', '1', '0', '-1', '1283', '18', '7', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('855', '1', '0', '-1', '1281', '18', '8', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('965', '1', '0', '-1', '981', '18', '7', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('1100', '1', '0', '-1', '21586', '18', '8', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('1724', '1', '0', '-1', '2592', '18', '8', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('2115', '1', '0', '-1', '3237', '18', '8', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('3298', '1', '0', '-1', '3410', '18', '8', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('3568', '1', '0', '-1', '1244', '18', '8', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('4501', '1', '0', '-1', '6430', '18', '8', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('13330', '1', '0', '-1', '23991', '18', '-1', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('22233', '1', '0', '-1', '12333', '18', '1', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('30806', '1', '0', '-1', '3565', '0', '-1', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('38059', '1', '0', '-1', '50457', '18', '8', '0');
INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`,`displayid`,`inventorytype`,`material`,`sheath`)VALUES ('184', '1', '0', '-1', '362', '18', '8', '0');

UPDATE `item_template` SET `name` = 930, `ContainerSlots` = 20 WHERE (entry = 930);
UPDATE `item_template` SET `name` = 855, `ContainerSlots` = 20 WHERE (entry = 855);
UPDATE `item_template` SET `name` = 965, `ContainerSlots` = 20 WHERE (entry = 965);
UPDATE `item_template` SET `name` = 1100, `ContainerSlots` = 20 WHERE (entry = 1100);
UPDATE `item_template` SET `name` = 1724, `ContainerSlots` = 20 WHERE (entry = 1724);
UPDATE `item_template` SET `name` = 2115, `ContainerSlots` = 20 WHERE (entry = 2115);
UPDATE `item_template` SET `name` = 3568, `ContainerSlots` = 20 WHERE (entry = 3568);
UPDATE `item_template` SET `name` = 3298, `ContainerSlots` = 20 WHERE (entry = 3298);
UPDATE `item_template` SET `name` = 4501, `ContainerSlots` = 20 WHERE (entry = 4501);
UPDATE `item_template` SET `name` = 13330, `ContainerSlots` = 20 WHERE (entry = 13330);
UPDATE `item_template` SET `name` = 22233, `ContainerSlots` = 20 WHERE (entry = 22233);
UPDATE `item_template` SET `name` = 30806, `ContainerSlots` = 20 WHERE (entry = 30806);
UPDATE `item_template` SET `name` = 38059, `ContainerSlots` = 20 WHERE (entry = 38059);
UPDATE `item_template` SET `name` = 184, `ContainerSlots` = 20  WHERE (entry = 184);

INSERT INTO creature_template VALUES (60784,0,0,0,0,0,20988,0,0,0,'E','Trade','',0,80,80,0,35,4225,1,1.14286,1,3,0,2000,2000,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,1,1,1,1,0,0,1,0,0,'',0);

INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `VerifiedBuild`) VALUES
(60784, 0, 930, 0, 0, 0, 0),
(60784, 0, 855, 0, 0, 0, 0),
(60784, 0, 965, 0, 0, 0, 0),
(60784, 0, 1100, 0, 0, 0, 0),
(60784, 0, 1724, 0, 0, 0, 0),
(60784, 0, 2115, 0, 0, 0, 0),
(60784, 0, 3568, 0, 0, 0, 0),
(60784, 0, 3298, 0, 0, 0, 0),
(60784, 0, 4501, 0, 0, 0, 0),
(60784, 0, 13330, 0, 0, 0, 0),
(60784, 0, 22233, 0, 0, 0, 0),
(60784, 0, 30806, 0, 0, 0, 0),
(60784, 0, 38059, 0, 0, 0, 0),
(60784, 0, 184, 0, 0, 0, 0);
