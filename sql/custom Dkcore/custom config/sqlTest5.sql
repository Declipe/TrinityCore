DELETE FROM `custom_config` WHERE `OptionName` = ('DontCacheRandomMovementPaths');
DELETE FROM `custom_config` WHERE `OptionName` = ('Rate.Reputation.Gain.WSG');
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Rate.Reputation.Gain.WSG', 'rate', '1', '1', 'This is applied IN ADDITION to the global Rate.Reputation.Gain.');
DELETE FROM `custom_config` WHERE `OptionName` = ('Rate.Reputation.Gain.AB');
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Rate.Reputation.Gain.AB', 'rate', '1', '1', 'This is applied IN ADDITION to the global Rate.Reputation.Gain.');
DELETE FROM `custom_config` WHERE `OptionName` = ('Rate.Reputation.Gain.AV');
INSERT INTO `custom_config` (`OptionName`, `Type`, `DefaultValue`, `CustomValue`, `Description`) VALUES ('Rate.Reputation.Gain.AV', 'rate', '1', '1', 'This is applied IN ADDITION to the global Rate.Reputation.Gain.');
