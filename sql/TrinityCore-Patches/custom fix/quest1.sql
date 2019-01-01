
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `spell_school_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(90032, 0, 0, 0, 0, 0, 16464, 16465, 16466, 16467, 'Phantom Guest', NULL, NULL, 0, 70, 70, 1, 35, 3, 1, 1.42857, 1, 0, 0, 1667, 2000, 1, 1, 2, 32768, 2048, 0, 0, 0, 0, 0, 0, 6, 8, 16409, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 290, 384, '', 1, 1, 1.4, 5, 1, 1, 1, 0, 144, 1, 8388624, 0, 0, '', 12340);


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