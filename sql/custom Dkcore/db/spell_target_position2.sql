
DROP TABLE IF EXISTS `spell_target_position2`;

CREATE TABLE `spell_target_position2` (
  `ID` int unsigned NOT NULL DEFAULT '0' COMMENT 'Identifier',
  `EffectIndex` tinyint unsigned NOT NULL DEFAULT '0',
  `MapID` smallint unsigned NOT NULL DEFAULT '0',
  `PositionX` float NOT NULL DEFAULT '0',
  `PositionY` float NOT NULL DEFAULT '0',
  `PositionZ` float NOT NULL DEFAULT '0',
  `Orientation` float NOT NULL DEFAULT '0',
  `VerifiedBuild` smallint DEFAULT '0',
  PRIMARY KEY (`ID`,`EffectIndex`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci COMMENT='Spell System';

/*Data for the table `spell_target_position2` */

insert  into `spell_target_position2`(`ID`,`EffectIndex`,`MapID`,`PositionX`,`PositionY`,`PositionZ`,`Orientation`,`VerifiedBuild`) values 
(0,0,0,0,0,0,0,0),
(90016,0,530,12354,-6894.78,18.1203,4.17834,0),
(442555,0,530,12354,-6894.78,18.1203,4.17834,0),
(778893,0,530,-4039.98,5844.04,267.049,2.29853,0),
(778894,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778895,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778896,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778897,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778898,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778899,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778900,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778901,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778902,0,0,3616.98,-2820.75,177.172,1.86647,0),
(778903,0,0,3616.98,-2820.75,177.172,1.86647,0);
