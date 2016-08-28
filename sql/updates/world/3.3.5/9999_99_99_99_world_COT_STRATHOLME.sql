-- Caverns of Time: Stratholme rework
SET @DAY = 86400;
SET @CGUID = 700300; -- creature GUIDs (1 creature);
DELETE FROM `creature` WHERE `guid` BETWEEN @CGUID+00 AND @CGUID+00;

-- Move additional Chromie spawns to DB
INSERT INTO `creature` (`guid`,`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`movementtype`) VALUES
(@CGUID+00, 27915, 595, 3, 1, 1813.298, 1283.578, 142.3258, 3.878161, @DAY, 0, 0);

-- Chromie AI
UPDATE `creature_template` SET `ScriptName`="npc_chromie_start" WHERE `entry`=26527;
UPDATE `creature_template` SET `ScriptName`="npc_chromie_middle" WHERE `entry`=27915;
-- Chromie whispers
DELETE FROM `creature_text` WHERE `entry`=27915 AND `groupid`=1;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`probability`,`BroadcastTextId`,`TextRange`,`comment`) VALUES
(27915, 1, 0, "Come over here time traveller, we need to talk!", 15, 100, 29167, 0, "Chromie - WHISPER_COME_TALK"),
(27915, 1, 1, "Quick, over here!", 15, 100, 29169, 0, "Chromie - WHISPER_COME_TALK"),
(27915, 1, 2, "We meet again, time traveller!", 15, 100, 29168, 0, "Chromie - WHISPER_COME_TALK");

-- Corruptor of Time
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry`=60422;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 60422, 31, 0, 3, 32281, "Corruption of Time - Implicitly target Guardian of Time");

-- Fix a random typo in AI name for Grain Crate Helper
UPDATE `creature_template` SET `ScriptName`="npc_crate_helper_cot" WHERE `entry`=27827;

-- Arcane Disruption: Require active grain crate within 8yd, and implicitly target the helper for that crate
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13,17) AND `SourceEntry`=49590;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ErrorType`, `Comment`) VALUES
(13, 1, 49590, 31, 0, 3,  27827,  0, "Arcane Disruption - Implicitly target Grain Crate Helper"),
(17, 0, 49590, 30, 0, 190094, 8, 11, "Arcane Disruption - Must be within 8yd of Suspicious Grain Crate"),
(17, 0, 49590, 29, 0,  27827, 8, 11, "Arcane Disruption - Must be within 8yd of Grain Crate Helper");

-- Make Suspicious Grain Crate and Grain Crate Helper never respawn
UPDATE `creature` SET `spawntimesecs`=@DAY WHERE `id`=27827;
UPDATE `gameobject` SET `spawntimesecs`=@DAY WHERE `id`=190094;

-- Blanket apply a spawn control AI to all "live stratholme" mobs that prevents them respawning after the purge begins
UPDATE `creature_template` SET `ScriptName`="npc_stratholme_fluff_living",`AIName`="" WHERE `entry` IN (28167,31126,31019,28169,31127,31023,31020,31018);
UPDATE `creature_template` SET `ScriptName`="npc_stratholme_smart_living",`AIName`="SmartAI" WHERE `entry` IN (31057);
-- Do the same for undead stratholme mobs, except for the other phases
UPDATE `creature_template` SET `ScriptName`="npc_stratholme_smart_undead",`AIName`="SmartAI" WHERE `entry` IN (28249,27729,28200,27734,27731,28199,27736);

-- SmartAI fixes for wave creatures
UPDATE `smart_scripts` SET `event_param3`=3100, `event_param4`=3400 WHERE `entryorguid`=28200 AND `source_type`=0 AND `id` IN (0,1);

-- Arthas AI
UPDATE `creature_template` SET `ScriptName`="npc_arthas_stratholme" WHERE `entry`=26499;

-- Lordaeron Crier yells
DELETE FROM `creature_text` WHERE `entry` = 27913;
INSERT INTO `creature_text` (`entry`,`groupid`,`text`,`type`,`probability`,`BroadcastTextId`,`TextRange`,`comment`) VALUES
(27913, 0, "All soldiers of Lordaeron should immediately report to the entrance of Stratholme, and await further orders from Prince Arthas.", 14, 100, 27263, 3, "Lordaeron Crier CALL_TO_GATES"),
(27913, 1, "Scourge forces have been spotted near the King's Square fountain!", 14, 100, 27642, 3, "Lordaeron Crier KINGS_SQUARE"),
(27913, 2, "Scourge forces have been spotted near the Market Row Gate!", 14, 100, 27643, 3, "Lordaeron Crier MARKET_ROW"),
(27913, 3, "Scourge forces have been spotted near the Festival Lane Gate!", 14, 100, 27644, 3, "Lordaeron Crier FESTIVAL_LANE"),
(27913, 4, "Scourge forces have been spotted near the Elder's Square Gate!", 14, 100, 27645, 3, "Lordaeron Crier ELDERS_SQUARE"),
(27913, 5, "Scourge forces have been spotted near the Town Hall!", 14, 100, 27646, 3, "Lordaeron Crier TOWN_HALL");

-- Clean up some incorrect spawns in wave area
DELETE FROM `creature` WHERE `guid` BETWEEN 143949 AND 143952;

-- Get rid of the colossal mess that is Risen Zombie SmartAI
UPDATE `creature_template` SET `ScriptName`="npc_stratholme_fluff_undead",`AIName`="" WHERE `entry` = 27737;
DELETE FROM `smart_scripts` WHERE -`entryorguid` IN (SELECT `guid` FROM `creature` WHERE `id`=27737 AND `map`=595);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND -`SourceEntry` IN (SELECT `guid` FROM `creature` WHERE `id`=27737 AND `map`=595);
