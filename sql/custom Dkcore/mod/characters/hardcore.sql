-- Таблица для логирования смертей hardcore игроков
CREATE TABLE IF NOT EXISTS `hardcore_deaths` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `char_guid` int(11) NOT NULL,
    `char_name` varchar(12) NOT NULL,
    `level` tinyint(3) unsigned NOT NULL,
    `playtime` int(11) unsigned NOT NULL,
    `killer_name` varchar(50) NOT NULL,
    `death_time` bigint(20) unsigned NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
