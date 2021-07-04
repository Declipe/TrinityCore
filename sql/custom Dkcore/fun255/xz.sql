UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 2, `HealthModifier` = 2 WHERE (entry = 3300);


REPLACE INTO `command` (`name`,`permission`,`help`) VALUES ('vip status', 193, 'Syntax: .vip  status $account - vip Status eines Accounts anzeigen');
