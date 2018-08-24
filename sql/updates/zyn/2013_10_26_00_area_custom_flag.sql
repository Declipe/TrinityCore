DROP TABLE IF EXISTS `area_custom_flag`;
CREATE TABLE `area_custom_flag` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `flag` tinyint(3) unsigned NOT NULL,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `z` float NOT NULL,
  `radius` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


-- if you want to set an specific, not client based, area to a sanctuary zone you can use the area_custom_flag table in zyn database.
-- just set the coords xyz, the flag id (0 = sanctuary, 1 = ffa, 2 = pvp) and the radius of this area eg 50. the result of this will be a circle in map which has his own flag
-- this system is perfect for malls which can be entered by horde and alliance
