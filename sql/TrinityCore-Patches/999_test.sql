UPDATE `creature_template` SET `npcflag` = 3 WHERE (entry = 30997);
UPDATE `item_template` SET `class` = 0, `BagFamily` = 0 WHERE (entry = 38644);
UPDATE `item_template` SET `maxcount` = 0 WHERE (entry = 38644);

UPDATE `item_template` SET `maxcount` = 0, `stackable` = 1000 WHERE (entry = 6265);




-- Table creature_template
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 29856;

-- Table smart_scripts
DELETE FROM `smart_scripts` WHERE (source_type = 0 AND entryorguid = 29856);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(29856, 0, 0, 0, 8, 0, 100, 512, 55363, 0, 0, 0, 0, 41, 3000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Gooey Ghoul Drool - On Spellhit \'Create Ghoul Drool Cover\' - Despawn Instant');

UPDATE `item_template` SET `spellid_1` = 71993 WHERE (entry = 39290);

INSERT INTO `creature_template` VALUES ('60783', '0', '0', '0', '0', '0', '21503', '0', '0', '0', 'Kilaniari', 'Avatar von Sathal', '', '0', '82', '82', '2', '16', '0', '1', '1.5', '2', '3', '0', '2000', '2000', '1', '1', '1', '0', '2048', '8', '0', '0', '0', '0', '0', '4', '0', '0', '0', '0', '50', '50', '50', '50', '50', '50', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '6000000', '6000000', '', '0', '1', '1', '380', '50', '5', '6', '1', '0', '0', '3', '618348543', '0', 'npc_archavon_warder1', '0');

UPDATE `creature_template` SET `faction` = 14, `scale` = 0.4, `unit_class` = 8, `type` = 3, `resistance1` = 0, `resistance2` = 0, `resistance3` = 0, `resistance4` = 0, `resistance5` = 0, `resistance6` = 0, `ArmorModifier` = 5, `ExperienceModifier` = 1, `RacialLeader` = 0, `RegenHealth` = 3, `mechanic_immune_mask` = 618348415 WHERE (entry = 60783);

UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80 WHERE (entry = 60783);