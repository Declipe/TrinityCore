DELETE FROM `trinity_string2` WHERE `entry` BETWEEN 40215 AND 40231;
INSERT INTO `trinity_string2` (`entry`, `content_default`, `content_ruRU`) VALUES
-- loc8 = ruRU --not_used74
(40215, 'Pay with: ', 'Обменять: '), 
(40216, '(you have: ', '(у вас: '),
(40217, 'Close', 'Закрыть'),
(40218, 'Receive: ', 'Получить: '),
(40219, '(rate: )', '(курс: )'),
(40220, 'Enter the amount of ', 'Введите количество '),
(40221, 'you want to SPEND:', ' которое хотите отдать:'),
(40222, '<- Back', '<- Назад'),
(40223, '|cffff0000You didn\'t enter anything.|r', '|cffff0000Вы ничего не ввели.|r'),
(40224, 'Please enter a positive whole number.', 'Введите целое положительное число.'),
(40225, 'Amount must be between 1 and %u.', 'Количество должно быть от 1 до %u.'),
(40226, 'Too few. Minimum: %u x %s (for %u x %s).', 'Слишком мало. Минимум: %u x %s (за %u x %s).'),
(40227, 'Note: only %u will be spent (%u is not enough for another exchange).', 'Внимание: будет списано только %u (%u недостаточно для ещё одного обмена).'),
(40228, 'Not enough %s. Required: %u, you have: %u', 'Недостаточно %s. Требуется: %u, у вас: %u'),
(40229, 'Not enough bag space for %u x %s.', 'Недостаточно места в сумках для %u x %s.'),
(40230, 'Exchange failed, currency refunded.', 'Обмен не удался, валюта возвращена.'),
(40231, 'Spent %u x %s -> received %u x %s.', 'Отдано %u x %s -> получено %u x %s.');




