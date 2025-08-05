DELETE FROM `conditions` WHERE (`SourceTypeOrReferenceId` = 17) AND (`SourceGroup` = 0) AND (`SourceEntry` = 47938) AND (`SourceId` = 0) AND (`ElseGroup` = 0) AND (`ConditionTypeOrReference` = 32) AND (`ConditionTarget` = 1) AND (`ConditionValue1` = 16) AND (`ConditionValue2` = 0) AND (`ConditionValue3` = 0);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(17, 0, 47938, 0, 0, 32, 1, 16, 0, 0, 1, 0, 0, '', '');


DELETE FROM `spell_script_names` WHERE `spell_id` = 25371 AND `ScriptName` = 'spell_consume_aq20';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (25371, 'spell_consume_aq20');
DELETE FROM `spell_script_names` WHERE `spell_id` = 25373 AND `ScriptName` = 'spell_gen_10pct_count_pct_from_max_hp';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (25373, 'spell_gen_10pct_count_pct_from_max_hp');

DELETE FROM `spell_script_names` WHERE `spell_id` = 25185 AND `ScriptName` = 'spell_itch_aq20';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (25185, 'spell_itch_aq20');

DELETE FROM `spell_script_names` WHERE `spell_id` = 26077 AND `ScriptName` = 'spell_itch_aq40';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (26077, 'spell_itch_aq40');