DELETE FROM `spell_script_names` WHERE `spell_id` = 50380;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(50380, 'spell_bloodspore_haze');

DELETE FROM `spell_custom_attr` WHERE `entry` = 50380;
INSERT INTO `spell_custom_attr` (`entry`, `attributes`) VALUES
(50380, 0x00400000);