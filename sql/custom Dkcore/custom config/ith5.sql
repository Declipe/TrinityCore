
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Battleground.Alterac.Reinforcements', 'int', '600', '600', NULL);
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Battleground.Alterac.ReputationOnBossDeath', 'int', '350', '350', NULL);
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Battleground.Arathi.CapturePoints', 'int', '1600', '1600', NULL);
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Battleground.IC', 'int', '300', '300', NULL);
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Battleground.Warsong.Flags', 'int', '3', '3', NULL);
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Battleground.EyeOfTheStorm.CapturePoints', 'int', '1600', '1600', NULL);
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('rate_talent_pet', 'int', '1', '2', NULL);

INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('config_chat_mute_first_login', 'bool', '0', '1', 'Description: Speaking is allowed after playing for Chat.MuteTimeFirstLogin minutes. You may use party and guild chat');
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('config_chat_time_mute_first_login', 'int', '120', '1', 'Description: The time after which the player will be able to speak.');
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('QuestPOI.Enabled', 'int', '1', '1', NULL);

DELETE FROM trinity_string2 WHERE entry = 40177;
INSERT INTO trinity_string2(`entry`,`content_default`,`content_esES`,`content_esMX`,`content_ruRU`) VALUES 
(40177, 'Speaking is allowed after playing for at least %d. You may use party and guild chat.','Podrás hablar cuando hayas jugado durante al menos %d. Puedes usar el chat de grupo y de hermandad.','Podrás hablar cuando hayas jugado durante al menos %d. Puedes usar el chat de grupo y de hermandad','Вы сможете говорить после %d проведенных минут в игре, но можете использовать чат гильдии и группы.');
