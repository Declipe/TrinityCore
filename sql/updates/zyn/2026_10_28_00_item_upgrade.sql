CREATE TABLE IF NOT EXISTS `character_item_upgrade` (
  `guid` int unsigned NOT NULL,
  `item_guid` int unsigned NOT NULL,
  `stat_id` int unsigned NOT NULL,
  PRIMARY KEY (`guid`, `item_guid`, `stat_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS `character_weapon_upgrade` (
  `guid` int unsigned NOT NULL,
  `item_guid` int unsigned NOT NULL,
  `upgrade_perc` float NOT NULL,
  PRIMARY KEY (`guid`, `item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

CREATE TABLE IF NOT EXISTS `character_weapon_speed_upgrade` (
  `guid` int unsigned NOT NULL,
  `item_guid` int unsigned NOT NULL,
  `upgrade_perc` float NOT NULL,
  PRIMARY KEY (`guid`, `item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci ROW_FORMAT=DYNAMIC;

-- Preserve existing stat upgrades when the character database uses its default name.
INSERT IGNORE INTO `character_item_upgrade` (`guid`, `item_guid`, `stat_id`)
SELECT `guid`, `item_guid`, `stat_id` FROM `characters`.`character_item_upgrade`;
