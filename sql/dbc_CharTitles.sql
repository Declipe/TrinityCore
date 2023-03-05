/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 80026
 Source Host           : localhost:3306
 Source Schema         : ascemu_logon

 Target Server Type    : MySQL
 Target Server Version : 80026
 File Encoding         : 65001

 Date: 05/12/2021 13:43:59
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for dbc_chartitles
-- ----------------------------
DROP TABLE IF EXISTS `dbc_chartitles`;
CREATE TABLE `dbc_chartitles`  (
  `Id` bigint NOT NULL DEFAULT 0,
  `UnkRef` bigint NOT NULL DEFAULT 0,
  `Male` varchar(300) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `Male_loc2` varchar(300) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `Female` varchar(300) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `Female_loc2` varchar(300) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NULL DEFAULT NULL,
  `InGameOrder` int(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`Id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci COMMENT = 'Export of CharTitles.dbc' ROW_FORMAT = Dynamic;
--

-- ----------------------------
-- Records of dbc_chartitles
-- ----------------------------
INSERT INTO `dbc_chartitles` VALUES (1, 5879, 'Рядовой %s', 'Рядовой %s', 'Рядовой %s', 'Рядовой %s', 1);
INSERT INTO `dbc_chartitles` VALUES (2, 5880, 'Капрал %s', 'Капрал %s', 'Капрал %s', 'Капрал %s', 2);
INSERT INTO `dbc_chartitles` VALUES (3, 5881, 'Сержант %s', 'Сержант %s', 'Сержант %s', 'Сержант %s', 3);
INSERT INTO `dbc_chartitles` VALUES (4, 5882, 'Старший сержант %s', 'Старший сержант %s', 'Старший сержант %s', 'Старший сержант %s', 4);
INSERT INTO `dbc_chartitles` VALUES (5, 5883, 'Старшина %s', 'Старшина %s', 'Старшина %s', 'Старшина %s', 5);
INSERT INTO `dbc_chartitles` VALUES (6, 5884, 'Рыцарь %s', 'Рыцарь %s', 'Рыцарь %s', 'Рыцарь %s', 6);
INSERT INTO `dbc_chartitles` VALUES (7, 5885, 'Рыцарь-лейтенант %s', 'Рыцарь-лейтенант %s', 'Рыцарь-лейтенант %s', 'Рыцарь-лейтенант %s', 7);
INSERT INTO `dbc_chartitles` VALUES (8, 5886, 'Рыцарь-капитан %s', 'Рыцарь-капитан %s', 'Рыцарь-капитан %s', 'Рыцарь-капитан %s', 8);
INSERT INTO `dbc_chartitles` VALUES (9, 5887, 'Рыцарь-защитник %s', 'Рыцарь-защитник %s', 'Рыцарь-защитник %s', 'Рыцарь-защитник %s', 9);
INSERT INTO `dbc_chartitles` VALUES (10, 5888, 'Лейтенант-командор %s', 'Лейтенант-командор %s', 'Лейтенант-командор %s', 'Лейтенант-командор %s', 10);
INSERT INTO `dbc_chartitles` VALUES (11, 5889, 'Командор %s', 'Командор %s', 'Командор %s', 'Командор %s', 11);
INSERT INTO `dbc_chartitles` VALUES (12, 5890, 'Маршал %s', 'Маршал %s', 'Маршал %s', 'Маршал %s', 12);
INSERT INTO `dbc_chartitles` VALUES (13, 5891, 'Фельдмаршал %s', 'Фельдмаршал %s', 'Фельдмаршал %s', 'Фельдмаршал %s', 13);
INSERT INTO `dbc_chartitles` VALUES (14, 5892, 'Главнокомандующий %s', 'Главнокомандующий %s', 'Главнокомандующий %s', 'Главнокомандующий %s', 14);
INSERT INTO `dbc_chartitles` VALUES (15, 5893, 'Разведчик %s', 'Разведчик %s', 'Разведчица %s', 'Разведчица %s', 15);
INSERT INTO `dbc_chartitles` VALUES (16, 5894, 'Пехотинец %s', 'Пехотинец %s', 'Пехотинец %s', 'Пехотинец %s', 16);
INSERT INTO `dbc_chartitles` VALUES (17, 5895, 'Сержант %s', 'Сержант %s', 'Сержант %s', 'Сержант %s', 17);
INSERT INTO `dbc_chartitles` VALUES (18, 5896, 'Старший сержант %s', 'Старший сержант %s', 'Старший сержант %s', 'Старший сержант %s', 18);
INSERT INTO `dbc_chartitles` VALUES (19, 5897, 'Старшина %s', 'Старшина %s', 'Старшина %s', 'Старшина %s', 19);
INSERT INTO `dbc_chartitles` VALUES (20, 5898, 'Каменный страж %s', 'Каменный страж %s', 'Каменный страж %s', 'Каменный страж %s', 20);
INSERT INTO `dbc_chartitles` VALUES (21, 5899, 'Кровавый страж %s', 'Кровавый страж %s', 'Кровавая стражница %s', 'Кровавая стражница %s', 21);
INSERT INTO `dbc_chartitles` VALUES (22, 5900, 'Легионер %s', 'Легионер %s', 'Легионер %s', 'Легионер %s', 22);
INSERT INTO `dbc_chartitles` VALUES (23, 5901, 'Центурион %s', 'Центурион %s', 'Центурион %s', 'Центурион %s', 23);
INSERT INTO `dbc_chartitles` VALUES (24, 5902, 'Защитник %s', 'Защитник %s', 'Защитница %s', 'Защитница %s', 24);
INSERT INTO `dbc_chartitles` VALUES (25, 5903, 'Генерал-лейтенант %s', 'Генерал-лейтенант %s', 'Генерал-лейтенант %s', 'Генерал-лейтенант %s', 25);
INSERT INTO `dbc_chartitles` VALUES (26, 5904, 'Генерал %s', 'Генерал %s', 'Генерал %s', 'Генерал %s', 26);
INSERT INTO `dbc_chartitles` VALUES (27, 5905, 'Вождь %s', 'Вождь %s', 'Вождь %s', 'Вождь %s', 27);
INSERT INTO `dbc_chartitles` VALUES (28, 5906, 'Верховный вождь %s', 'Верховный вождь %s', 'Верховный вождь %s', 'Верховный вождь %s', 28);
INSERT INTO `dbc_chartitles` VALUES (42, 0, 'Гладиатор %s', 'Гладиатор %s', 'Гладиатор %s', 'Гладиатор %s', 29);
INSERT INTO `dbc_chartitles` VALUES (43, 0, 'Дуэлянт %s', 'Дуэлянт %s', 'Дуэлянт %s', 'Дуэлянт %s', 30);
INSERT INTO `dbc_chartitles` VALUES (44, 0, 'Фаворит %s', 'Фаворит %s', 'Фаворит %s', 'Фаворит %s', 31);
INSERT INTO `dbc_chartitles` VALUES (45, 0, 'Претендент %s', 'Претендент %s', 'Претендент %s', 'Претендент %s', 32);
INSERT INTO `dbc_chartitles` VALUES (46, 6341, 'Повелитель Скарабеев %s', 'Повелитель Скарабеев %s', 'Повелительница Скарабеев %s', 'Повелительница Скарабеев %s', 33);
INSERT INTO `dbc_chartitles` VALUES (47, 6407, 'Завоеватель %s', 'Завоеватель %s', 'Завоевательница %s', 'Завоевательница %s', 34);
INSERT INTO `dbc_chartitles` VALUES (48, 6406, 'Вершитель правосудия %s', 'Вершитель правосудия %s', 'Вершительница правосудия %s', 'Вершительница правосудия %s', 35);
INSERT INTO `dbc_chartitles` VALUES (53, 6686, '%s, защитник Наару', '%s, защитник Наару', '%s, защитница Наару', '%s, защитница Наару', 36);
INSERT INTO `dbc_chartitles` VALUES (62, 0, 'Безжалостный гладиатор %s', 'Безжалостный гладиатор %s', 'Безжалостный гладиатор %s', 'Безжалостный гладиатор %s', 37);
INSERT INTO `dbc_chartitles` VALUES (63, 6806, '%s из Расколотого Солнца', '%s из Расколотого Солнца', '%s из Расколотого Солнца', '%s из Расколотого Солнца', 38);
INSERT INTO `dbc_chartitles` VALUES (64, 6944, '%s, Длань А\'дала', '%s, Длань А\'дала', '%s, Длань А\'дала', '%s, Длань А\'дала', 39);
INSERT INTO `dbc_chartitles` VALUES (71, 0, 'Мстительный гладиатор %s', 'Мстительный гладиатор %s', 'Мстительный гладиатор %s', 'Мстительный гладиатор %s', 40);
INSERT INTO `dbc_chartitles` VALUES (72, 7530, 'Военачальник %s', 'Военачальник %s', 'Военачальник %s', 'Военачальник %s', 41);
INSERT INTO `dbc_chartitles` VALUES (74, 7533, 'Премудрый %s', 'Премудрый %s', 'Премудрая %s', 'Премудрая %s', 43);
INSERT INTO `dbc_chartitles` VALUES (75, 7534, 'Страж огня %s', 'Страж огня %s', 'Страж огня %s', 'Страж огня %s', 44);
INSERT INTO `dbc_chartitles` VALUES (76, 7535, 'Хранитель огня %s', 'Хранитель огня %s', 'Хранительница огня %s', 'Хранительница огня %s', 45);
INSERT INTO `dbc_chartitles` VALUES (77, 7565, '%s Превозносимый', '%s Превозносимый', '%s Превозносимая', '%s Превозносимая', 46);
INSERT INTO `dbc_chartitles` VALUES (78, 7695, 'Исследователь %s', 'Исследователь %s', 'Исследовательница %s', 'Исследовательница %s', 47);
INSERT INTO `dbc_chartitles` VALUES (79, 7748, 'Дипломат %s', 'Дипломат %s', 'Дипломат %s', 'Дипломат %s', 48);
INSERT INTO `dbc_chartitles` VALUES (80, 0, 'Жестокий гладиатор %s', 'Жестокий гладиатор %s', 'Жестокий гладиатор %s', 'Жестокий гладиатор %s', 49);
INSERT INTO `dbc_chartitles` VALUES (81, 7759, 'Искатель %s', 'Искатель %s', 'Искательница %s', 'Искательница %s', 42);
INSERT INTO `dbc_chartitles` VALUES (82, 7749, 'Повелитель арены %s', 'Повелитель арены %s', 'Повелительница арены %s', 'Повелительница арены %s', 50);
INSERT INTO `dbc_chartitles` VALUES (83, 7750, 'Морской дьявол %s', 'Морской дьявол %s', 'Морской дьявол %s', 'Морской дьявол %s', 51);
INSERT INTO `dbc_chartitles` VALUES (84, 7754, 'Шеф-повар %s', 'Шеф-повар %s', 'Шеф-повар %s', 'Шеф-повар %s', 52);
INSERT INTO `dbc_chartitles` VALUES (85, 7703, '%s Первейший', '%s Первейший', '%s Первейшая', '%s Первейшая', 53);
INSERT INTO `dbc_chartitles` VALUES (86, 7703, '%s, повелитель штормов', '%s, повелитель штормов', '%s, повелительница штормов', '%s, повелительница штормов', 54);
INSERT INTO `dbc_chartitles` VALUES (87, 7703, 'Изумрудный Сновидец %s', 'Изумрудный Сновидец %s', 'Изумрудная Сновидица %s', 'Изумрудная Сновидица %s', 55);
INSERT INTO `dbc_chartitles` VALUES (89, 7703, 'Пророк %s', 'Пророк %s', 'Пророчица %s', 'Пророчица %s', 57);
INSERT INTO `dbc_chartitles` VALUES (90, 7703, '%s Тлетворный', '%s Тлетворный', '%s Тлетворная', '%s Тлетворная', 58);
INSERT INTO `dbc_chartitles` VALUES (91, 7703, 'Ловчий %s', 'Ловчий %s', 'Ловчий %s', 'Ловчий %s', 59);
INSERT INTO `dbc_chartitles` VALUES (92, 7703, '%s из Черного Клинка', '%s из Черного Клинка', '%s из Черного Клинка', '%s из Черного Клинка', 60);
INSERT INTO `dbc_chartitles` VALUES (93, 7703, 'Верховный маг %s', 'Верховный маг %s', 'Верховный маг %s', 'Верховный маг %s', 61);
INSERT INTO `dbc_chartitles` VALUES (94, 7703, 'Вестник войны %s', 'Вестник войны %s', 'Вестница войны %s', 'Вестница войны %s', 62);
INSERT INTO `dbc_chartitles` VALUES (95, 7703, 'Ассасин %s', 'Ассасин %s', 'Ассасин %s', 'Ассасин %s', 63);
INSERT INTO `dbc_chartitles` VALUES (96, 7703, 'Великий алхимик %s', 'Великий алхимик %s', 'Великий алхимик %s', 'Великий алхимик %s', 64);
INSERT INTO `dbc_chartitles` VALUES (97, 7703, 'Великий кузнец %s', 'Великий кузнец %s', 'Великий кузнец %s', 'Великий кузнец %s', 65);
INSERT INTO `dbc_chartitles` VALUES (98, 7703, 'Великий шеф-повар %s', 'Великий шеф-повар %s', 'Великий шеф-повар %s', 'Великий шеф-повар %s', 66);
INSERT INTO `dbc_chartitles` VALUES (99, 7703, 'Великий зачаровыватель %s', 'Великий зачаровыватель %s', 'Великая зачаровывательница %s', 'Великая зачаровывательница %s', 67);
INSERT INTO `dbc_chartitles` VALUES (100, 7703, 'Великий инженер %s', 'Великий инженер %s', 'Великий инженер %s', 'Великий инженер %s', 68);
INSERT INTO `dbc_chartitles` VALUES (101, 7703, 'Доктор %s', 'Доктор %s', 'Доктор %s', 'Доктор %s', 69);
INSERT INTO `dbc_chartitles` VALUES (102, 7703, 'Великий рыболов %s', 'Великий рыболов %s', 'Великий рыболов %s', 'Великий рыболов %s', 70);
INSERT INTO `dbc_chartitles` VALUES (103, 7703, 'Великий травник %s', 'Великий травник %s', 'Великая травница %s', 'Великая травница %s', 71);
INSERT INTO `dbc_chartitles` VALUES (104, 7703, 'Великий начертатель %s', 'Великий начертатель %s', 'Великая начертательница %s', 'Великая начертательница %s', 72);
INSERT INTO `dbc_chartitles` VALUES (105, 7703, 'Великий ювелир %s', 'Великий ювелир %s', 'Великий ювелир %s', 'Великий ювелир %s', 73);
INSERT INTO `dbc_chartitles` VALUES (106, 7703, 'Великий кожевник %s', 'Великий кожевник %s', 'Великая кожевница %s', 'Великая кожевница %s', 74);
INSERT INTO `dbc_chartitles` VALUES (107, 7703, 'Великий мастер горного дела %s', 'Великий мастер горного дела %s', 'Великий мастер горного дела %s', 'Великий мастер горного дела %s', 75);
INSERT INTO `dbc_chartitles` VALUES (108, 7703, 'Великий мастер снятия шкур %s', 'Великий мастер снятия шкур %s', 'Великий мастер снятия шкур %s', 'Великий мастер снятия шкур %s', 76);
INSERT INTO `dbc_chartitles` VALUES (109, 7703, 'Великий портной %s', 'Великий портной %s', 'Великий портной %s', 'Великий портной %s', 77);
INSERT INTO `dbc_chartitles` VALUES (110, 7703, '%s, сын Кель\'Таласа', '%s, сын Кель\'Таласа', '%s, дочь Кель\'Таласа', '%s, дочь Кель\'Таласа', 78);
INSERT INTO `dbc_chartitles` VALUES (111, 7703, '%s, сын Аргуса', '%s, сын Аргуса', '%s, дочь Аргуса', '%s, дочь Аргуса', 79);
INSERT INTO `dbc_chartitles` VALUES (112, 7703, '%s, сын Каз Модана', '%s, сын Каз Модана', '%s, дочь Каз Модана', '%s, дочь Каз Модана', 80);
INSERT INTO `dbc_chartitles` VALUES (113, 8236, '%s Гномреганский', '%s Гномреганский', '%s Гномреганская', '%s Гномреганская', 81);
INSERT INTO `dbc_chartitles` VALUES (114, 7703, '%s Львиное Сердце', '%s Львиное Сердце', '%s Львиное Сердце', '%s Львиное Сердце', 82);
INSERT INTO `dbc_chartitles` VALUES (115, 7703, '%s, защитник Элуны', '%s, защитник Элуны', '%s, защитница Элуны', '%s, защитница Элуны', 83);
INSERT INTO `dbc_chartitles` VALUES (116, 7703, '%s, герой Оргриммара', '%s, герой Оргриммара', '%s, героиня Оргриммара', '%s, героиня Оргриммара', 84);
INSERT INTO `dbc_chartitles` VALUES (117, 7703, '%s, хозяин лугов', '%s, хозяин лугов', '%s, хозяйка лугов', '%s, хозяйка лугов', 85);
INSERT INTO `dbc_chartitles` VALUES (118, 7703, '%s из Черного Копья', '%s из Черного Копья', '%s из Черного Копья', '%s из Черного Копья', 86);
INSERT INTO `dbc_chartitles` VALUES (119, 7703, '%s Отрекшийся', '%s Отрекшийся', '%s Отрекшаяся', '%s Отрекшаяся', 87);
INSERT INTO `dbc_chartitles` VALUES (120, 7812, '%s, искатель магии', '%s, искатель магии', '%s, искательница магии', '%s, искательница магии', 88);
INSERT INTO `dbc_chartitles` VALUES (121, 7813, '%s, покоритель сумрака', '%s, покоритель сумрака', '%s, покорительница сумрака', '%s, покорительница сумрака', 89);
INSERT INTO `dbc_chartitles` VALUES (122, 7814, '%s, Завоеватель Наксрамаса', '%s, Завоеватель Наксрамаса', '%s, Завоевательница Наксрамаса', '%s, Завоевательница Наксрамаса', 90);
INSERT INTO `dbc_chartitles` VALUES (123, 7815, '%s, Герой Нордскола', '%s, Герой Нордскола', '%s, Герой Нордскола', '%s, Герой Нордскола', 91);
INSERT INTO `dbc_chartitles` VALUES (124, 7820, 'Тыквер %s', 'Тыквер %s', 'Тыквер %s', 'Тыквер %s', 92);
INSERT INTO `dbc_chartitles` VALUES (125, 7849, 'Хранитель мудрости %s', 'Хранитель мудрости %s', 'Хранительница мудрости %s', 'Хранительница мудрости %s', 93);
INSERT INTO `dbc_chartitles` VALUES (126, 7853, '%s, сын Альянса', '%s, сын Альянса', '%s, дочь Альянса', '%s, дочь Альянса', 94);
INSERT INTO `dbc_chartitles` VALUES (127, 7854, '%s, сын Орды', '%s, сын Орды', '%s, дочь Орды', '%s, дочь Орды', 95);
INSERT INTO `dbc_chartitles` VALUES (128, 7856, '%s Всепобеждающий', '%s Всепобеждающий', '%s Всепобеждающая', '%s Всепобеждающая', 96);
INSERT INTO `dbc_chartitles` VALUES (129, 7857, '%s, защитник ледяных пустошей', '%s, защитник ледяных пустошей', '%s, защитница ледяных пустошей', '%s, защитница ледяных пустошей', 97);
INSERT INTO `dbc_chartitles` VALUES (130, 7858, 'Посланник %s', 'Посланник %s', 'Посланница %s', 'Посланница %s', 98);
INSERT INTO `dbc_chartitles` VALUES (131, 7859, '%s, Серебряный защитник', '%s, Серебряный защитник', '%s, Серебряная защитница', '%s, Серебряная защитница', 99);
INSERT INTO `dbc_chartitles` VALUES (132, 7860, '%s, защитник Кенария', '%s, защитник Кенария', '%s, защитница Кенария', '%s, защитница Кенария', 100);
INSERT INTO `dbc_chartitles` VALUES (133, 7861, 'Хмелевар %s', 'Хмелевар %s', 'Хмелевар %s', 'Хмелевар %s', 101);
INSERT INTO `dbc_chartitles` VALUES (134, 7864, 'Весельчак %s', 'Весельчак %s', 'Хохотушка %s', 'Хохотушка %s', 102);
INSERT INTO `dbc_chartitles` VALUES (135, 7875, 'Безумно влюбленный %s', 'Безумно влюбленный %s', 'Безумно влюбленная %s', 'Безумно влюбленная %s', 103);
INSERT INTO `dbc_chartitles` VALUES (137, 7893, 'Благодетельница %s', 'Благодетельница %s', 'Благодетельница %s', 'Благодетельница %s', 104);
INSERT INTO `dbc_chartitles` VALUES (138, 7894, 'Благодетель %s', 'Благодетель %s', 'Благодетель %s', 'Благодетель %s', 105);
INSERT INTO `dbc_chartitles` VALUES (139, 7964, 'Покоритель Обсидианового святилища %s', 'Покоритель Обсидианового святилища %s', 'Покорительница Обсидианового святилища %s', 'Покорительница Обсидианового святилища %s', 106);
INSERT INTO `dbc_chartitles` VALUES (140, 7965, 'Сумеречный %s', 'Сумеречный %s', 'Сумеречная %s', 'Сумеречная %s', 107);
INSERT INTO `dbc_chartitles` VALUES (141, 7990, '%s Бессмертный', '%s Бессмертный', '%s Бессмертная', '%s Бессмертная', 108);
INSERT INTO `dbc_chartitles` VALUES (142, 7991, 'Неумирающий %s', 'Неумирающий %s', 'Неумирающая %s', 'Неумирающая %s', 109);
INSERT INTO `dbc_chartitles` VALUES (143, 7997, '%s Дженкинс', '%s Дженкинс', '%s Дженкинс', '%s Дженкинс', 110);
INSERT INTO `dbc_chartitles` VALUES (144, 8045, 'Адмирал Кровавого Паруса %s', 'Адмирал Кровавого Паруса %s', 'Адмирал Кровавого Паруса %s', 'Адмирал Кровавого Паруса %s', 111);
INSERT INTO `dbc_chartitles` VALUES (145, 8121, '%s Чокнутый', '%s Чокнутый', '%s Чокнутая', '%s Чокнутая', 112);
INSERT INTO `dbc_chartitles` VALUES (146, 8237, '%s Экзодарский', '%s Экзодарский', '%s Экзодарская', '%s Экзодарская', 113);
INSERT INTO `dbc_chartitles` VALUES (147, 8238, '%s Дарнасский', '%s Дарнасский', '%s Дарнасская', '%s Дарнасская', 114);
INSERT INTO `dbc_chartitles` VALUES (148, 8239, '%s Стальгорнский', '%s Стальгорнский', '%s Стальгорнская', '%s Стальгорнская', 115);
INSERT INTO `dbc_chartitles` VALUES (149, 8240, '%s Штормградский', '%s Штормградский', '%s Штормградская', '%s Штормградская', 116);
INSERT INTO `dbc_chartitles` VALUES (150, 8241, '%s Оргриммарский', '%s Оргриммарский', '%s Оргриммарская', '%s Оргриммарская', 117);
INSERT INTO `dbc_chartitles` VALUES (151, 8242, '%s Сен\'джинский', '%s Сен\'джинский', '%s Сен\'джинская', '%s Сен\'джинская', 118);
INSERT INTO `dbc_chartitles` VALUES (152, 8243, '%s из Луносвета', '%s из Луносвета', '%s из Луносвета', '%s из Луносвета', 119);
INSERT INTO `dbc_chartitles` VALUES (153, 8244, '%s из Громового Утеса', '%s из Громового Утеса', '%s из Громового Утеса', '%s из Громового Утеса', 120);
INSERT INTO `dbc_chartitles` VALUES (154, 8245, '%s из Подгорода', '%s из Подгорода', '%s из Подгорода', '%s из Подгорода', 121);
INSERT INTO `dbc_chartitles` VALUES (155, 8303, 'Чудесный %s', 'Чудесный %s', 'Чудесная %s', 'Чудесная %s', 122);
INSERT INTO `dbc_chartitles` VALUES (156, 8332, 'Крестоносец %s', 'Крестоносец %s', 'Крестоносец %s', 'Крестоносец %s', 123);
INSERT INTO `dbc_chartitles` VALUES (157, 0, 'Смертоносный гладиатор %s', 'Смертоносный гладиатор %s', 'Смертоносный гладиатор %s', 'Смертоносный гладиатор %s', 56);
INSERT INTO `dbc_chartitles` VALUES (158, 8450, '%s, победивший смерть', '%s, победивший смерть', '%s, победившая смерть', '%s, победившая смерть', 124);
INSERT INTO `dbc_chartitles` VALUES (159, 8451, '%s, небесный заступник', '%s, небесный заступник', '%s, небесная заступница', '%s, небесная заступница', 125);
INSERT INTO `dbc_chartitles` VALUES (160, 8453, '%s, завоеватель Ульдуара', '%s, завоеватель Ульдуара', '%s, завоевательница Ульдуара', '%s, завоевательница Ульдуара', 126);
INSERT INTO `dbc_chartitles` VALUES (161, 8452, '%s, защитник Ульдуара', '%s, защитник Ульдуара', '%s, защитница Ульдуара', '%s, защитница Ульдуара', 127);
INSERT INTO `dbc_chartitles` VALUES (163, 0, 'Покоритель %s', 'Покоритель %s', 'Покорительница %s', 'Покорительница %s', 128);
INSERT INTO `dbc_chartitles` VALUES (164, 8467, '%s, говорящий со звездами', '%s, говорящий со звездами', '%s, говорящая со звездами', '%s, говорящая со звездами', 129);
INSERT INTO `dbc_chartitles` VALUES (165, 8468, '%s, звездный странник', '%s, звездный странник', '%s, звездная странница', '%s, звездная странница', 130);
INSERT INTO `dbc_chartitles` VALUES (166, 8469, '%s, посланник титанов', '%s, посланник титанов', '%s, посланница титанов', '%s, посланница титанов', 131);
INSERT INTO `dbc_chartitles` VALUES (167, 0, 'Гневный гладиатор %s', 'Гневный гладиатор %s', 'Гневный гладиатор %s', 'Гневный гладиатор %s', 132);
INSERT INTO `dbc_chartitles` VALUES (168, 8596, 'Странник %s', 'Странник %s', 'Странница %s', 'Странница %s', 133);
INSERT INTO `dbc_chartitles` VALUES (169, 0, 'Неумолимый гладиатор %s', 'Неумолимый гладиатор %s', 'Неумолимый гладиатор %s', 'Неумолимый гладиатор %s', 134);
INSERT INTO `dbc_chartitles` VALUES (170, 8777, 'Великий крестоносец %s', 'Великий крестоносец %s', 'Великий крестоносец %s', 'Великий крестоносец %s', 135);
INSERT INTO `dbc_chartitles` VALUES (171, 8778, '%s, Серебряный заступник', '%s, Серебряный заступник', '%s, Серебряная заступница', '%s, Серебряная заступница', 136);
INSERT INTO `dbc_chartitles` VALUES (172, 8977, '%s Терпеливый', '%s Терпеливый', '%s Терпеливая', '%s Терпеливая', 137);
INSERT INTO `dbc_chartitles` VALUES (173, 9043, '%s, Сияние Рассвета', '%s, Сияние Рассвета', '%s, Сияние Рассвета', '%s, Сияние Рассвета', 138);
INSERT INTO `dbc_chartitles` VALUES (174, 9045, '%s, победивший падшего Короля', '%s, победивший падшего Короля', '%s, победившая падшего Короля', '%s, победившая падшего Короля', 139);
INSERT INTO `dbc_chartitles` VALUES (175, 9046, '%s, свергнувший Короля', '%s, свергнувший Короля', '%s, свергнувшая Короля', '%s, свергнувшая Короля', 140);
INSERT INTO `dbc_chartitles` VALUES (176, 9138, '%s из Пепельного союза', '%s из Пепельного союза', '%s из Пепельного союза', '%s из Пепельного союза', 141);
INSERT INTO `dbc_chartitles` VALUES (177, 0, 'Разгневанный гладиатор %s', 'Разгневанный гладиатор %s', 'Разгневанный гладиатор %s', 'Разгневанный гладиатор %s', 142);

SET FOREIGN_KEY_CHECKS = 1;
