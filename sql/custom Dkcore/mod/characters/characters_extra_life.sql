CREATE TABLE IF NOT EXISTS `hardcore_extra_lives` (
    `player_guid` INT(11) UNSIGNED NOT NULL,
    `has_extra_life` TINYINT(1) NOT NULL DEFAULT 0,
    PRIMARY KEY (`player_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;