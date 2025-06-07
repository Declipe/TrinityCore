-- Таблица наград за время
CREATE TABLE `hardcore_time_rewards`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `required_time` int UNSIGNED NOT NULL COMMENT 'Время в секундах',
  `item_id` int UNSIGNED NULL DEFAULT NULL,
  `item_count` int UNSIGNED NULL DEFAULT 1,
  `money` int UNSIGNED NULL DEFAULT 0,
  `title_id` int UNSIGNED NULL DEFAULT NULL,
  `achievement_id` int UNSIGNED NULL DEFAULT NULL,
  `spell_id` int UNSIGNED NULL DEFAULT NULL,
  `name` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `experience` int NULL DEFAULT NULL,
  `description` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `enabled` tinyint(1) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 7 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = Dynamic;

-- Таблица полученных наград
CREATE TABLE `hardcore_rewards_claimed` (
    `guid` INT(10) UNSIGNED NOT NULL,
    `reward_id` INT(10) UNSIGNED NOT NULL,
    `claimed_time` INT(10) UNSIGNED NOT NULL,
    `playtime_when_claimed` INT(10) UNSIGNED NOT NULL,
    PRIMARY KEY (`guid`, `reward_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Примеры наград
INSERT INTO `hardcore_time_rewards` VALUES (1, 1500, 6948, 1, 0, NULL, NULL, NULL, 'Камень возвращения', NULL, 'Награда', 1);
INSERT INTO `hardcore_time_rewards` VALUES (2, 7200, 0, 0, 100000, NULL, NULL, NULL, 'Золотая награда', NULL, '1 золото', 1);
INSERT INTO `hardcore_time_rewards` VALUES (3, 6666, 25, 1, 0, NULL, NULL, NULL, 'Меч правосудия', NULL, 'Особое оружие', 1);
INSERT INTO `hardcore_time_rewards` VALUES (4, 6060, 25, 0, 500000, NULL, NULL, NULL, 'Большая награда', NULL, '5 золота', 1);
INSERT INTO `hardcore_time_rewards` VALUES (5, 3000, 19019, 1, 0, NULL, NULL, NULL, 'Громовая ярость', NULL, 'Легендарное оружие', 1);
INSERT INTO `hardcore_time_rewards` VALUES (6, 66, 0, 0, 0, NULL, NULL, NULL, 'ГРОМПЫХ', 60000, NULL, 1);