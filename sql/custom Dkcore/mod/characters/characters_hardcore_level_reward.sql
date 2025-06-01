-- Таблица для хранения информации о наградах за уровни
CREATE TABLE `hardcore_level_rewards` (
  `level` INT(11) NOT NULL PRIMARY KEY,
  `item_id` INT(11) DEFAULT NULL COMMENT 'ID предмета награды',
  `item_count` INT(11) DEFAULT 1 COMMENT 'Количество предметов',
  `money` INT(11) DEFAULT 0 COMMENT 'Количество денег',
  `title_id` INT(11) DEFAULT NULL COMMENT 'ID титула',
  `achievement_id` INT(11) DEFAULT NULL COMMENT 'ID достижения',
  `spell_id` INT(11) DEFAULT NULL COMMENT 'ID заклинания'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Награды за уровни в Hardcore-режиме';

-- Таблица для отслеживания полученных наград
CREATE TABLE `hardcore_rewards_received` (
  `id` INT(11) NOT NULL AUTO_INCREMENT,
  `player_guid` INT(11) NOT NULL,
  `level` INT(11) NOT NULL,
  `received_at` INT(11) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `player_level` (`player_guid`, `level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Полученные награды за уровни';

-- Пример заполнения таблицы наград
INSERT INTO `hardcore_level_rewards` (`level`, `item_id`, `item_count`, `money`, `title_id`, `achievement_id`, `spell_id`) VALUES
(10, 29434, 1, 100000, NULL, NULL, NULL),  -- Плоть нежити
(20, 35279, 1, 200000, NULL, NULL, NULL),  -- Кольцо защиты
(30, 38186, 1, 300000, NULL, NULL, NULL),  -- Нагрудник героя
(40, 40075, 1, 400000, NULL, NULL, NULL),  -- Меч героя
(50, 43246, 1, 500000, 1, NULL, NULL),    -- Титул "Закаленный в боях"
(60, 45085, 1, 1000000, 2, 1, 53142), -- Титул "Покоритель Нордскола" + достижение + телепорт в Далран
(70, 49426, 1, 2000000, 3, 2, 64488), -- Титул "Легенда" + достижение + телепорт в Штормград
(80, 50255, 1, 5000000, 4, 3, 74452); -- Титул "Победитель Лича" + достижение + телепорт в Ледяную Корону