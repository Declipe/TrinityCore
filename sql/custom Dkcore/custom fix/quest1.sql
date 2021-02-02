INSERT INTO creature_template VALUES (90032,0,0,0,0,0,16464,16465,16466,16467,'Phantom Guest',NULL,NULL,0,70,70,1,35,3,1,1.42857,1,0,0,1667,2000,1,1,2,32768,2048,0,0,6,8,16409,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,290,384,'',1,1,1.4,5,1,1,1,0,144,1,8388624,0,0,'',12340);


INSERT INTO `creature_addon` VALUES (15008635, 0, 0, 0, 1, 400, NULL);
INSERT INTO `creature_addon` VALUES (15008638, 0, 0, 0, 1, 400, NULL);
INSERT INTO `creature_addon` VALUES (15008639, 0, 0, 0, 1, 400, NULL);
INSERT INTO `creature_addon` VALUES (15008640, 0, 0, 0, 1, 400, NULL);

DELETE FROM `creature_queststarter` WHERE (quest = 269991);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(90032, 269991);

DELETE FROM `creature_questender` WHERE (quest = 269991);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(90032, 269991);

DELETE FROM `creature_queststarter` WHERE (quest = 269992);
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(90032, 269992);

DELETE FROM `creature_questender` WHERE (quest = 269992);
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(90032, 269992);

INSERT INTO `quest_template_addon` VALUES (269991, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
INSERT INTO `quest_template_addon` VALUES (269992, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);

INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`) VALUES
(90032, 3, 6477, 'Journal', '', '', '', 1, 57, 90032, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 12340);

INSERT INTO `gameobject_questitem` (`GameObjectEntry`, `Idx`, `ItemId`, `VerifiedBuild`) VALUES
(90032, 0, 24147, 12340);

INSERT INTO `gameobject_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(90032, 24147, 0, 100, 1, 1, 0, 1, 1, NULL);

SET @stack := 20000;

UPDATE `item_template` SET `stackable` = @stack WHERE `entry` = 24147;
