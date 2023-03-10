DELETE FROM `trinity_string` WHERE `entry` IN (40131, 40132);
INSERT INTO `trinity_string` VALUES ('40131', 'Guild Level: %u.', null, null, null, null, null, null, null, null);
INSERT INTO `trinity_string` VALUES ('40132', 'Guild Experience: %u/%u.', null, null, null, null, null, null, null, null);
DELETE FROM `command` WHERE `permission` IN (639558, 639559, 639560);
INSERT INTO `command` VALUES ('guild linfo', '639558', 'Syntax: .guild linfo');
INSERT INTO `command` VALUES ('guild setlevel', '639559', 'Syntax: .guild setlevel $GuildName $Level');
INSERT INTO `command` VALUES ('guild givexp', '639560', 'Syntax: .guild givexp $GuildName $Xp');
