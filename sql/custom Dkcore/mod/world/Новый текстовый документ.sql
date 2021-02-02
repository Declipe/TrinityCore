
DELETE FROM `trinity_string` WHERE `entry` BETWEEN 11300 AND 11303;
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES
(11303, 'Remove effect', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Удалить эффект'),
(11302, 'Effect №%i (now: %s)', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Эффект №%i (%s)'),
(11301, 'no effect', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'нет эффекта'),
(11300, 'Are you sure you want to remove the effect?', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Вы уверены что хотите удалить эффект?');
