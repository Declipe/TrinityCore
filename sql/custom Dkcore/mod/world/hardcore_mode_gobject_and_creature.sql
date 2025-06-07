
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`) VALUES
(254605, 2, 6925, 'Shrine of Challenge', '', '', '', 1.2, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 'hardcore_activator_ai', 0);

-- NPC для выдачи наград
INSERT INTO `creature_template` (`entry`, `modelid1`, `name`, `subname`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `scale`, `rank`, `type`, `type_flags`, `unit_flags`, `unit_flags2`, `AIName`, `ScriptName`) VALUES
(190001, 25902, 'Хранитель Времени', 'Награды за выживание', 80, 80, 35, 1, 1, 0, 7, 0, 0, 2048, '', 'npc_hardcore_time_rewards');
