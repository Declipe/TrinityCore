
-- Таблица наград за время
CREATE TABLE `hardcore_time_rewards` (
    `id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    `required_time` INT(10) UNSIGNED NOT NULL COMMENT 'Время в секундах',
    `item_id` INT(10) UNSIGNED DEFAULT NULL,
    `item_count` INT(10) UNSIGNED DEFAULT '1',
    `money` INT(10) UNSIGNED DEFAULT '0',
    `title_id` INT(10) UNSIGNED DEFAULT NULL,
    `achievement_id` INT(10) UNSIGNED DEFAULT NULL,
    `spell_id` INT(10) UNSIGNED DEFAULT NULL,
    `name` VARCHAR(255) NOT NULL,
    `description` TEXT,
    `enabled` TINYINT(1) NOT NULL DEFAULT '1',
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Таблица полученных наград
CREATE TABLE `hardcore_rewards_claimed` (
    `guid` INT(10) UNSIGNED NOT NULL,
    `reward_id` INT(10) UNSIGNED NOT NULL,
    `claimed_time` INT(10) UNSIGNED NOT NULL,
    `playtime_when_claimed` INT(10) UNSIGNED NOT NULL,
    PRIMARY KEY (`guid`, `reward_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Примеры наград
INSERT INTO `hardcore_time_rewards` (`required_time`, `item_id`, `item_count`, `money`, `name`, `description`) VALUES
(3600, 6948, 1, 0, 'Камень возвращения', 'Награда за 1 час выживания'),
(7200, 0, 0, 100000, 'Золотая награда', '1 золото за 2 часа выживания'),
(14400, 25, 1, 0, 'Меч правосудия', 'Особое оружие за 4 часа выживания'),
(21600, 0, 0, 500000, 'Большая награда', '5 золота за 6 часов выживания'),
(43200, 19019, 1, 0, 'Громовая ярость', 'Легендарное оружие за 12 часов выживания');

