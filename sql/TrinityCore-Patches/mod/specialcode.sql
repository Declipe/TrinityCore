CREATE TABLE `world_coded` (
  `CodeId` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `CodeData` varchar(255) NOT NULL,
  `CodeStart` int(4) NOT NULL DEFAULT '0',
  `CodeEnd` int(4) NOT NULL DEFAULT '0',
  `MailSubject` varchar(200) NOT NULL,
  `MailText` varchar(200) NOT NULL,
  `MailMoney` int(10) NOT NULL DEFAULT '0',
  `MaxCountForCharacter` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxCountForAccount` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxCountTotal` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`CodeId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
