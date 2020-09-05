INSERT INTO `creature_template` (`entry`, `modelid1`, `name`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `InhabitType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RegenHealth`, `flags_extra`, `ScriptName`) VALUES
(60769, 26502, 'TeleNPC2', 83, 83, 35, 1, 1, 1.14286, 2, 1, 1, 1, 1, 2, 2048, 3, 1, 1, 1, 1, 1, 1, 1, 2, 'npc_teleport');

SET
@ENTRY          := 60769;

INSERT INTO creature (id, map, spawnMask, phaseMask, modelid, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, curhealth, curmana) VALUES
(@ENTRY, 0, 1, 1, 0, -13180.5, 342.503, 43.1936, 4.32977, 25, 0, 13700, 6540), 
(@ENTRY, 530, 1, 1, 0, -3862.69, -11645.8, -137.629, 2.38273, 25, 0, 13700, 6540), 
(@ENTRY, 0, 1, 1, 0, -4898.37, -965.118, 501.447, 2.25986, 25, 0, 13700, 6540), 
(@ENTRY, 0, 1, 1, 0, -8845.09, 624.828, 94.2999, 0.44062, 25, 0, 13700, 6540), 
(@ENTRY, 1, 1, 1, 0, 1599.25, -4375.85, 10.0872, 5.23641, 25, 0, 13700, 6540), 
(@ENTRY, 1, 1, 1, 0, -1277.65, 72.9751, 128.742, 5.95567, 25, 0, 13700, 6540), 
(@ENTRY, 0, 1, 1, 0, 1637.21, 240.132, -43.1034, 3.13147, 25, 0, 13700, 6540), 
(@ENTRY, 530, 1, 1, 0, 9741.67, -7454.19, 13.5572, 3.14231, 25, 0, 13700, 6540), 
(@ENTRY, 571, 1, 1, 0, 5807.06, 506.244, 657.576, 5.54461, 25, 0, 13700, 6540), 
(@ENTRY, 1, 1, 1, 0, 9866.83, 2494.66, 1315.88, 5.9462, 25, 0, 13700, 6540), 
(@ENTRY, 0, 1, 1, 0, -14279.8, 555.014, 8.90011, 3.97606, 25, 0, 13700, 6540), 
(@ENTRY, 530, 1, 1, 0, -1888.65, 5355.88, -12.4279, 1.25883, 25, 0, 13700, 6540);