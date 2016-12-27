UPDATE `creature` SET `spawndist`=0, `MovementType`=0 WHERE  `guid`=132444;
DELETE FROM `creature_addon` WHERE  `guid`=132444;
UPDATE `creature` SET MovementType=0, spawndist=0 WHERE id IN (27885,30571);
UPDATE `creature` SET position_x=1553.526, position_y=578.1731, position_z=99.8456, orientation=5.707227 WHERE id=30571;

-- Areatrigger SAI
SET @ENTRY := 5291;
DELETE FROM `areatrigger_scripts` WHERE `entry`=@ENTRY;
INSERT INTO `areatrigger_scripts` (`entry`,`ScriptName`) VALUES (@ENTRY,"SmartTrigger");
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=2;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,2,0,0,46,0,100,0,5291,0,0,0,45,1,1,0,0,0,0,10,132444,30571,0,0,0,0,0,"Areatrigger - On Trigger - Set Data 1 1");

-- Michael Belfast SAI
SET @ENTRY := 30571;
UPDATE `creature_template` SET `AIName`="SmartAI" WHERE `entry`=@ENTRY;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,0,0,0,38,0,100,1,1,1,0,0,80,@ENTRY*100+00,2,0,0,0,0,1,0,0,0,0,0,0,0,"Michael Belfast - On Data Set 1 1 - Run Script (No Repeat)");

-- Actionlist SAI
SET @ENTRY := 3057100;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,9,0,0,0,0,100,0,0,0,0,0,59,0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Michael Belfast - On Script - Set Run Off"),
(@ENTRY,9,1,0,0,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,1549.57,575.557,100.052,0.82,"Michael Belfast - On Script - Move To Position"),
(@ENTRY,9,2,0,0,0,100,0,4000,4000,0,0,1,0,5000,0,0,0,0,1,0,0,0,0,0,0,0,"Michael Belfast - On Script - Say Line 0"),
(@ENTRY,9,3,0,0,0,100,0,5000,5000,0,0,1,0,5000,0,0,0,0,19,30551,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 0"),
(@ENTRY,9,4,0,0,0,100,0,5000,5000,0,0,1,0,4000,0,0,0,0,19,30553,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 0"),
(@ENTRY,9,5,0,0,0,100,0,4000,4000,0,0,1,0,4000,0,0,0,0,19,30552,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 0"),
(@ENTRY,9,6,0,0,0,100,0,3000,3000,0,0,5,5,0,0,0,0,0,19,30552,50,0,0,0,0,0,"Michael Belfast - On Script - Play Emote 5"),
(@ENTRY,9,7,0,0,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,1554.85,588.465,99.775,0.88,"Michael Belfast - On Script - Move To Position"),
(@ENTRY,9,8,0,0,0,100,0,5000,5000,0,0,1,1,3000,0,0,0,0,1,0,0,0,0,0,0,0,"Michael Belfast - On Script - Say Line 1"),
(@ENTRY,9,9,0,0,0,100,0,3000,3000,0,0,1,0,5000,0,0,0,0,19,31017,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 0"),
(@ENTRY,9,10,0,0,0,100,0,5000,5000,0,0,1,0,10000,0,0,0,0,19,30561,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 0"),
(@ENTRY,9,11,0,0,0,100,0,10000,10000,0,0,1,1,0,0,0,0,0,19,31017,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 1"),
(@ENTRY,9,12,0,0,0,100,0,0,0,0,0,1,2,3000,0,0,0,0,19,31017,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 2"),
(@ENTRY,9,13,0,0,0,100,0,0,0,0,0,69,0,0,0,0,0,0,8,0,0,0,1549.61,575.544,100.052,4.17,"Michael Belfast - On Script - Move To Position"),
(@ENTRY,9,14,0,0,0,100,0,3000,3000,0,0,1,1,5000,0,0,0,0,19,30553,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 1"),
(@ENTRY,9,15,0,0,0,100,0,5000,5000,0,0,1,1,5000,0,0,0,0,19,30551,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 1"),
(@ENTRY,9,16,0,0,0,100,0,0,0,0,0,1,1,5000,0,0,0,0,19,30552,50,0,0,0,0,0,"Michael Belfast - On Script - Say Line 1");

DELETE FROM `creature_text` WHERE `entry` IN (30571, 30551, 30553, 30552, 31017, 30561);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
-- Michael Belfast
(30571, 0, 0, 'Hey! Stop rooting around in my cellar! Clear out!', 12, 0, 100, 5, 0, 0, 31322, 'Michael Belfast'),
(30571, 0, 1, 'What were you doing in my cellar? There\'s a food scare going on, and the last thing I need is strangers rummaging around in my goods! Shoo!', 12, 0, 100, 5, 0, 0, 31323, 'Michael Belfast'),
(30571, 1, 0, 'I HEARD THAT! No more ale for you! Not a drop!', 12, 0, 100, 5, 0, 0, 31327, 'Michael Belfast'),
-- Hearthsinger Forresten
(30551, 0, 0, 'This whole situation seems a bit paranoid, don\'t you think?', 12, 0, 100, 6, 0, 0, 31324, 'Hearthsinger Forresten'),
(30551, 1, 0, 'Thank the Light for that.', 12, 0, 100, 1, 0, 0, 32573, 'Hearthsinger Forresten'),
-- Footman James
(30553, 0, 0, 'Orders are orders. If the Prince says jump...', 12, 0, 100, 1, 0, 0, 31325, 'Footman James'),
(30553, 1, 0, 'Don\'t worry too much. By the time I went off duty, we hadn\'t found a scrap of befouled grain here.', 12, 0, 100, 1, 0, 0, 32572, 'Footman James'),
-- Fras Siabi
(30552, 0, 0, 'It\'s a strange order, you can\'t deny. Suspicious food? Under that definition, you should arrest Belfast!', 12, 0, 100, 1, 0, 0, 31326, 'Fras Siabi'),
(30552, 1, 0, '%s nods.', 16, 0, 100, 0, 0, 0, 3065, 'Fras Siabi'),
-- Mal Corricks
(31017, 0, 0, 'Enough, Michael. Business is hurting enough with this scare as it is. We can use every copper.', 12, 0, 100, 1, 0, 0, 32560, 'Mal Corricks'),
(31017, 1, 0, '%s grudgingly nods.', 16, 0, 100, 0, 0, 0, 32569, 'Mal Corricks'),
(31017, 2, 0, 'I can\'t argue with that.', 12, 0, 100, 1, 0, 0, 32570, 'Mal Corricks'),
-- Gryan Stoutmantle
(30561, 0, 0, 'The soldiers are doing important work. The safety of the people is more important, Mal, if you\'re interested in your customers living to spend another day.', 12, 0, 100, 1, 0, 0, 32571, 'Gryan Stoutmantle');