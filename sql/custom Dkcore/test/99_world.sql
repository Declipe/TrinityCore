
INSERT INTO `creature_template2_difficulty` (`CreatureId`, `DifficultyCreatureId1`, `DifficultyCreatureId2`, `DifficultyCreatureId3`) 
SELECT `entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3` FROM `creature_template2`;
