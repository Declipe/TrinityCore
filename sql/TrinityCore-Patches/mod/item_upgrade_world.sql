DROP TABLE IF EXISTS `world_item_upgrade`;
CREATE TABLE `world_item_upgrade` (
  `enchant_id` int(10) NOT NULL,
  `prev_enchant_id` int(10) NOT NULL DEFAULT '0',
  `description` varchar(255) NOT NULL,
  `golds` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`enchant_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DELETE FROM `world_item_upgrade`;
INSERT INTO `world_item_upgrade` VALUES ('66', '0', '+1 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('72', '66', '+2 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('73', '72', '+3 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('102', '73', '+4 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('103', '102', '+5 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('104', '103', '+6 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('105', '104', '+7 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('353', '105', '+8 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('356', '353', '+9 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('361', '356', '+10 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('366', '361', '+11 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('371', '366', '+12 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('409', '371', '+13 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('411', '409', '+14 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1068', '411', '+15 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1069', '1068', '+16 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1070', '1069', '+17 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1071', '1070', '+18 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1072', '1071', '+19 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1073', '1072', '+20 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1074', '1073', '+21 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1075', '1074', '+22 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1076', '1075', '+23 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1077', '1076', '+24 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1078', '1077', '+25 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1079', '1078', '+26 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1080', '1079', '+27 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1081', '1080', '+28 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1082', '1081', '+29 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1083', '1082', '+30 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1084', '1083', '+31 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1085', '1084', '+32 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1086', '1085', '+33 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1087', '1086', '+34 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1088', '1087', '+35 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1089', '1088', '+36 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1090', '1089', '+37 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1091', '1090', '+38 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1092', '1091', '+39 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1093', '1092', '+40 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1209', '1093', '+41 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1210', '1209', '+42 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1211', '1210', '+43 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1212', '1211', '+44 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1213', '1212', '+45 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1214', '1213', '+46 к выносливости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('3851', '1214', '+50 к выносливости', '4000');
INSERT INTO `world_item_upgrade` VALUES ('68', '0', '+1 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('69', '68', '+2 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('70', '69', '+3 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('106', '70', '+4 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('107', '106', '+5 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('108', '107', '+6 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('109', '108', '+7 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('352', '109', '+8 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('357', '352', '+9 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('362', '357', '+10 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('367', '362', '+11 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('372', '367', '+12 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('410', '372', '+13 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('412', '410', '+14 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('684', '412', '+15 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1043', '684', '+16 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1044', '1043', '+17 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1045', '1044', '+18 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1046', '1045', '+19 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1047', '1046', '+20 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1048', '1047', '+21 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1049', '1048', '+22 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1050', '1049', '+23 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1051', '1050', '+24 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1052', '1051', '+25 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1053', '1052', '+26 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1054', '1053', '+27 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1055', '1054', '+28 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1056', '1055', '+29 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1057', '1056', '+30 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1058', '1057', '+31 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1059', '1058', '+32 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1060', '1059', '+33 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1061', '1060', '+34 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1062', '1061', '+35 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1063', '1062', '+36 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1064', '1063', '+37 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1065', '1064', '+38 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1066', '1065', '+39 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1067', '1066', '+40 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1203', '1067', '+41 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1204', '1203', '+42 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1205', '1204', '+43 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1206', '1205', '+44 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1207', '1206', '+45 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1208', '1207', '+46 к силе', '1000');
INSERT INTO `world_item_upgrade` VALUES ('79', '0', '+1 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('80', '79', '+2 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('81', '80', '+3 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('94', '81', '+4 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('95', '94', '+5 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('96', '95', '+6 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('97', '96', '+7 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('350', '97', '+8 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('354', '350', '+9 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('359', '354', '+10 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('364', '359', '+11 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('369', '364', '+12 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('405', '369', '+13 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('406', '405', '+14 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1118', '406', '+15 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1119', '1118', '+16 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1120', '1119', '+17 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1121', '1120', '+18 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1122', '1121', '+19 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1123', '1122', '+20 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1124', '1123', '+21 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1125', '1124', '+22 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1126', '1125', '+23 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1127', '1126', '+24 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1128', '1127', '+25 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1129', '1128', '+26 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1130', '1129', '+27 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1131', '1130', '+28 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1132', '1131', '+29 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1133', '1132', '+30 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1134', '1133', '+31 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1135', '1134', '+32 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1136', '1135', '+33 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1137', '1136', '+34 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1138', '1137', '+35 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1139', '1138', '+36 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1140', '1139', '+37 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1141', '1140', '+38 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1142', '1141', '+39 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1143', '1142', '+40 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1221', '1143', '+41 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1222', '1221', '+42 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1223', '1222', '+43 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1224', '1223', '+44 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1225', '1224', '+45 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1226', '1225', '+46 к интеллекту', '1000');
INSERT INTO `world_item_upgrade` VALUES ('206', '0', '+1 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('207', '206', '+2 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2910', '207', '+3 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('208', '2910', '+4 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('209', '208', '+5 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('210', '209', '+6 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('211', '210', '+7 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('212', '211', '+8 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('430', '212', '+9 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('431', '430', '+11 к силе заклинаний', '2000');
INSERT INTO `world_item_upgrade` VALUES ('432', '431', '+12 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2317', '432', '+13 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2318', '2317', '+14 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2319', '2318', '+15 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2320', '2319', '+16 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2321', '2320', '+18 к силе заклинаний', '2000');
INSERT INTO `world_item_upgrade` VALUES ('2322', '2321', '+19 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2323', '2322', '+20 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2324', '2323', '+21 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2325', '2324', '+22 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2326', '2325', '+23 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2327', '2326', '+25 к силе заклинаний', '2000');
INSERT INTO `world_item_upgrade` VALUES ('2328', '2327', '+26 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2329', '2328', '+27 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2330', '2329', '+28 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2331', '2330', '+29 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2332', '2331', '+30 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2333', '2332', '+32 к силе заклинаний', '2000');
INSERT INTO `world_item_upgrade` VALUES ('2334', '2333', '+33 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2335', '2334', '+34 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2336', '2335', '+35 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2337', '2336', '+36 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2338', '2337', '+37 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2339', '2338', '+39 к силе заклинаний', '2000');
INSERT INTO `world_item_upgrade` VALUES ('2340', '2339', '+40 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2341', '2340', '+41 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2342', '2341', '+42 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2343', '2342', '+43 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2344', '2343', '+44 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('3257', '2344', '+54 к силе заклинаний', '10000');
INSERT INTO `world_item_upgrade` VALUES ('3255', '3257', '+55 к силе заклинаний', '1000');
INSERT INTO `world_item_upgrade` VALUES ('74', '0', '+1 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('75', '74', '+2 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('76', '75', '+3 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('90', '76', '+4 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('91', '90', '+5 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('92', '91', '+6 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('93', '92', '+7 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('343', '93', '+8 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('349', '343', '+9 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('358', '349', '+10 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('363', '358', '+11 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('368', '363', '+12 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('403', '368', '+13 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('404', '403', '+14 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('883', '404', '+15 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('983', '883', '+16 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1094', '983', '+17 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1095', '1094', '+18 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1096', '1095', '+19 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1097', '1096', '+20 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1098', '1097', '+21 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1099', '1098', '+22 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1100', '1099', '+23 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1101', '1100', '+24 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1102', '1101', '+25 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1103', '1102', '+26 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1104', '1103', '+27 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1105', '1104', '+28 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1106', '1105', '+29 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1107', '1106', '+30 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1108', '1107', '+31 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1109', '1108', '+32 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1110', '1109', '+33 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1111', '1110', '+34 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1112', '1111', '+35 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1113', '1112', '+36 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1114', '1113', '+37 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1115', '1114', '+38 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1116', '1115', '+39 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1117', '1116', '+40 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1215', '1117', '+41 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1216', '1215', '+42 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1217', '1216', '+43 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1218', '1217', '+44 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1219', '1218', '+45 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1220', '1219', '+46 к ловкости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2801', '0', '+8 к устойчивости', '8000');
INSERT INTO `world_item_upgrade` VALUES ('2788', '2801', '+9 к устойчивости', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2789', '2788', '+15 к устойчивости', '6000');
INSERT INTO `world_item_upgrade` VALUES ('1563', '0', '+2 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1583', '1563', '+4 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1584', '1583', '+6 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1585', '1584', '+8 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1586', '1585', '+10 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1587', '1586', '+12 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1588', '1587', '+14 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1589', '1588', '+16 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1590', '1589', '+18 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1591', '1590', '+20 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1592', '1591', '+22 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1593', '1592', '+24 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1594', '1593', '+26 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1595', '1594', '+28 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1596', '1595', '+30 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1597', '1596', '+32 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1598', '1597', '+34 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1599', '1598', '+36 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1600', '1599', '+38 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('3521', '1600', '+40 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1602', '3521', '+42 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1603', '1602', '+44 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1604', '1603', '+46 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1605', '1604', '+48 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('3845', '1605', '+50 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1607', '3845', '+52 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1608', '1607', '+54 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1609', '1608', '+56 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1610', '1609', '+58 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1611', '1610', '+60 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1612', '1611', '+62 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1613', '1612', '+64 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1614', '1613', '+66 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1615', '1614', '+68 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1616', '1615', '+70 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1617', '1616', '+72 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1618', '1617', '+74 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1619', '1618', '+76 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1620', '1619', '+78 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1621', '1620', '+80 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1622', '1621', '+82 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1623', '1622', '+84 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1624', '1623', '+86 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1625', '1624', '+88 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1626', '1625', '+90 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1627', '1626', '+92 к силе атаки', '1000');
INSERT INTO `world_item_upgrade` VALUES ('2838', '0', '+7 к рейтингу критического удара', '1500');
INSERT INTO `world_item_upgrade` VALUES ('195', '2838', '+14 к рейтингу критического удара', '3000');
INSERT INTO `world_item_upgrade` VALUES ('196', '195', '+28 к рейтингу критического удара', '6000');
INSERT INTO `world_item_upgrade` VALUES ('197', '196', '+42 к рейтингу критического удара', '8000');
INSERT INTO `world_item_upgrade` VALUES ('198', '197', '+56 к рейтингу критического удара', '12000');
INSERT INTO `world_item_upgrade` VALUES ('436', '198', '+70 к рейтингу критического удара', '15000');
INSERT INTO `world_item_upgrade` VALUES ('82', '0', '+1 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('83', '82', '+2 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('84', '83', '+3 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('98', '84', '+4 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('99', '98', '+5 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('100', '99', '+6 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('101', '100', '+7 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('351', '101', '+8 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('355', '351', '+9 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('360', '355', '+10 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('365', '360', '+11 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('370', '365', '+12 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('407', '370', '+13 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('408', '407', '+14 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1144', '408', '+15 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1145', '1144', '+16 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1146', '1145', '+17 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1147', '1146', '+18 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1148', '1147', '+19 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1149', '1148', '+20 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1150', '1149', '+21 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1151', '1150', '+22 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1152', '1151', '+23 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1153', '1152', '+24 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1154', '1153', '+25 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1155', '1154', '+26 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1156', '1155', '+27 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1157', '1156', '+28 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1158', '1157', '+29 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1159', '1158', '+30 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1160', '1159', '+31 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1161', '1160', '+32 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1162', '1161', '+33 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1163', '1162', '+34 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1183', '1163', '+35 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1164', '1183', '+36 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1165', '1164', '+37 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1166', '1165', '+38 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1167', '1166', '+39 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1168', '1167', '+40 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1227', '1168', '+41 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1228', '1227', '+42 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1229', '1228', '+43 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1230', '1229', '+44 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1231', '1230', '+45 к духу', '1000');
INSERT INTO `world_item_upgrade` VALUES ('1232', '1231', '+46 к духу', '1000');