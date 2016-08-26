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

-- Fix a random typo in AI name for Grain Crate Helper
UPDATE `creature_template` SET `ScriptName`="npc_crate_helper_cot" WHERE `entry`=27827;

-- Arcane Disruption: Require active grain crate within 8yd, and implicitly target the helper for that crate
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (13,17) AND `SourceEntry`=49590;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ErrorType`, `Comment`) VALUES
(13, 1, 49590, 31, 0, 3,  27827, 11, "Arcane Disruption - Implicitly target Grain Crate Helper"),
(17, 0, 49590, 30, 0, 190094, 8,  0, "Arcane Disruption - Must be within 8yd of Suspicious Grain Crate"),
(17, 0, 49590, 29, 0,  27827, 8,  0, "Arcane Disruption - Must be within 8yd of Grain Crate Helper");

-- Make Suspicious Grain Crate and Grain Crate Helper never respawn
UPDATE `creature` SET `spawntimesecs`=@DAY WHERE `id`=27827;
UPDATE `gameobject` SET `spawntimesecs`=@DAY WHERE `id`=190094;

-- Arthas AI
UPDATE `creature_template` SET `ScriptName`="npc_arthas_stratholme" WHERE `entry`=26499;
