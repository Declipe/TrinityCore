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
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`probability`,`BroadcastTextId`,`TextRange`,`emote`,`comment`) VALUES
(27915, 1, 0, "Come over here time traveller, we need to talk!", 15, 100, 29167, 0, 70, "Chromie - WHISPER_COME_TALK"),
(27915, 1, 1, "Quick, over here!", 15, 100, 29169, 0, 70, "Chromie - WHISPER_COME_TALK"),
(27915, 1, 2, "We meet again, time traveller!", 15, 100, 29168, 0, 70, "Chromie - WHISPER_COME_TALK");

-- Corruptor of Time
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceEntry`=60422;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 60422, 31, 0, 3, 32281, "Corruption of Time - Implicitly target Guardian of Time");

-- Starting inn RP fluff
DELETE FROM `areatrigger_scripts` WHERE `entry`=5291;
INSERT INTO `areatrigger_scripts` (`entry`,`ScriptName`) VALUES (5291,"at_stratholme_inn_stairs_cot");
UPDATE `creature` SET `movementtype`=0 WHERE `id`=30571;
UPDATE `creature_addon` SET `bytes1`=0 WHERE `guid` IN (SELECT guid FROM `creature` WHERE `id`=30571);
UPDATE `creature_template` SET `ScriptName`="",`AIName`="SmartAI" WHERE `entry`=30571;
UPDATE `creature_template` SET `ScriptName`="npc_hearthsinger_forresten_cot",`AIName`="" WHERE `entry`=30551;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 30571 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`action_type`,`action_param1`,`action_param2`,`target_type`,`target_param1`,`target_param2`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(30571, 0, 0, 1, 72, 100, 1, 100,  0, 17,  0, 0,  1,     0,   0,        0,        0,        0,        0, "Michael Belfast - On Action 100 - Set Emote State None (No Repeat)"),
(30571, 0, 1, 2, 61, 100, 1,   0,  0, 59,  0, 0,  0,     0,   0,        0,        0,        0,        0, "Michael Belfast - On Action 100 - Set Run (false) (No Repeat)"),
(30571, 0, 2, 0, 61, 100, 1,   0,  0, 69, 42, 0,  8,     0,   0, 1554.371, 587.4865, 99.77525, 1.107272, "Michael Belfast - On Action 100 - Move Position (ID 42) (No Repeat)"),
(30571, 0, 3, 0, 34, 100, 1,   8, 42, 45,  1, 1, 11, 30551, 100,        0,        0,        0,        0, "Michael Belfast - On Movement Inform (ID 42) - Set Data 1-1 on Hearthsinger Forresten (within 100yd) (No Repeat)");
DELETE FROM `creature_text` WHERE `entry` IN (30551,30552,30553,30561,30571,31017);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`probability`,`BroadcastTextId`,`emote`,`comment`) VALUES
(30571, 0, 0, "What were you doing in my cellar? There's a food scare going on, and the last thing I need is strangers rummaging around in my goods! Shoo!", 12, 100, 31323, 5, "Michael Belfast - SAY_CELLAR"),
(30571, 0, 1, "Hey! Stop rooting around in my cellar! Clear out!", 12, 100, 31322, 5, "Michael Belfast - SAY_CELLAR"),
(30551, 0, 0, "This whole situation seems a bit paranoid, don't you think?", 12, 100, 31324, 1, "Hearthsinger Forresten - LINE_FORREST_1"),
(30553, 0, 0, "Orders are orders. If the Prince says jump...", 12, 100, 31325, 1, "Footman James - LINE_JAMES_1"),
(30552, 0, 0, "It's a strange order, you can't deny. Suspicious food? Under that definition, you should arrest Belfast!", 12, 100, 31326, 1, "Fras Siabi - LINE_FRAS_1"),
(30571, 1, 0, "I HEARD THAT! No more ale for you! Not a drop!", 12, 100, 31327, 22, "Michael Belfast - LINE_BELFAST_1"),
(31017, 0, 0, "Enough, Michael. Business is hurting enough with this scare as it is. We can use every copper.", 12, 100, 32560, 1, "Mal Corricks - LINE_MAL_1"),
(30561, 0, 0, "The soldiers are doing important work. The safety of the people is more important, Mal, if you're interested in your customers living to spend another day.", 12, 100, 32571, 1, "Gryan Stoutmantle - LINE_GRYAN_1"),
(31017, 1, 0, "%s grudgingly nods.", 16, 100, 32569, 273, "Mal Corricks - LINE_MAL_2"),
(31017, 2, 0, "I can't argue with that.", 12, 100, 32570, 1, "Mal Corricks - LINE_MAL_3"),
(30553, 1, 0, "Don't worry too much. By the time I went off duty, we hadn't found a scrap of befouled grain here.", 12, 100, 32572, 1, "Footman James - LINE_JAMES_2"),
(30551, 1, 0, "Thank the Light for that.", 12, 100, 32573, 1, "Hearthsinger Forresten - LINE_FORREST_2"),
(30552, 1, 0, "%s nods.", 16, 100, 32574, 273, "Fras Siabi - LINE_FRAS_2");

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

-- Arthas AI
UPDATE `creature_template` SET `ScriptName`="npc_arthas_stratholme" WHERE `entry`=26499;

-- Missing Arthas lines
DELETE FROM `creature_text` WHERE `entry`=26499 AND `groupid`>38;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`probability`,`BroadcastTextId`,`TextRange`,`comment`) VALUES
(26499,39,0,"I can't afford to spare you.",12,100,31355,0,"Arthas SAY_AGGRO_ZOMBIE"),
(26499,40,0,"All officers should check in with me when their squads are ready. We'll enter Stratholme on my order.",12,100,27517,3,"Arthas RP1_LINE_ARTHAS11");

-- Arthas/Uther RP data
UPDATE `creature_template` SET `ScriptName`="npc_stratholme_rp_dummy",`AIName`="" WHERE `entry` IN (26528,26497);
UPDATE `creature_text` SET `TextRange`=3 WHERE `entry` IN (26499,26528,26497);
UPDATE `creature_text` SET `emote`=396 WHERE
	(`entry` = 26499 AND `groupid` IN (0,3,5)) OR
	(`entry` = 26528 AND `groupid`=3) OR
	(`entry` = 26497 AND `groupid` IN (0,1));
UPDATE `creature_text` SET `emote`=1 WHERE `entry`=26499 AND `groupid`=7;
UPDATE `creature_text` SET `emote`=5 WHERE
	(`entry` = 26499 AND `groupid` IN (2,4)) OR
	(`entry` = 26528 AND `groupid` IN (1,4));
UPDATE `creature_text` SET `emote`=6 WHERE `entry`=26528 AND `groupid`=2;
UPDATE `creature_text` SET `emote`=397 WHERE
	(`entry` = 26528 AND `groupid` IN (0,5));

-- Arthas/Mal'ganis RP data
DELETE FROM `creature_text` WHERE `entry`=28169 AND `groupid`=0 AND `id`=1;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`probability`,`BroadcastTextId`,`TextRange`,`comment`) VALUES
(28169, 0, 1, "On, no...", 12, 100, 27552, 0, "culling SAY_PHASE204");
UPDATE `creature_text` SET `emote`=1 WHERE
	(`entry` = 28167 AND `groupid`=0) OR
	(`entry` = 28169 AND `groupid`=0) OR
	(`entry` = 26499 AND `groupid` IN (12,14)) OR
	(`entry` = 26533 AND `groupid`=0);
UPDATE `creature_text` SET `emote`=397 WHERE
	(`entry` = 26499 AND `groupid`=13);
	
-- Blanket apply a spawn control AI to all "live stratholme" mobs that prevents them respawning after the purge begins
UPDATE `creature_template` SET `ScriptName`="npc_stratholme_fluff_living",`AIName`="" WHERE `entry` IN (28167,31126,31019,28169,31127,31023,31020,31018);
UPDATE `creature_template` SET `ScriptName`="npc_stratholme_smart_living",`AIName`="SmartAI" WHERE `entry` IN (31057);
-- Do the same for undead stratholme mobs, except for the other phases
UPDATE `creature_template` SET `ScriptName`="npc_stratholme_smart_undead",`AIName`="SmartAI" WHERE `entry` IN (28249,27729,28200,27734,27731,28199,27736);

-- SmartAI fixes for wave creatures
UPDATE `smart_scripts` SET `event_param3`=3100, `event_param4`=3400 WHERE `entryorguid`=28200 AND `source_type`=0 AND `id` IN (0,1);

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

-- Town Hall RP event
DELETE FROM `creature_text` WHERE `entry`=28340;
INSERT INTO `creature_text` (`entry`,`groupid`,`text`,`type`,`probability`,`BroadcastTextId`,`TextRange`,`comment`) VALUES
(28340, 0, "Ah, you've finally arrived Prince Arthas. You're here just in the nick of time.", 12, 100, 27811, 3, "Stratholme Citizen (Disguised Infinite) RP3_LINE_CITIZEN1"),
(28340, 1, "There's no need for you to understand, Arthas. All you need to do is die.", 12, 100, 27815, 3, "Stratholme Citizen (Disguised Infinite) RP3_LINE_CITIZEN2");
