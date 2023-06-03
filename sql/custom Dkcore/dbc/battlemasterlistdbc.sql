/*
SQLyog Community v13.2.0 (64 bit)
MySQL - 8.0.33 : Database - dbc
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

CREATE TABLE `battlemasterlistdbc` (
  `ID` int NOT NULL DEFAULT '0',
  `MapID_1` int NOT NULL DEFAULT '0',
  `MapID_2` int NOT NULL DEFAULT '0',
  `MapID_3` int NOT NULL DEFAULT '0',
  `MapID_4` int NOT NULL DEFAULT '0',
  `MapID_5` int NOT NULL DEFAULT '0',
  `MapID_6` int NOT NULL DEFAULT '0',
  `MapID_7` int NOT NULL DEFAULT '0',
  `MapID_8` int NOT NULL DEFAULT '0',
  `InstanceType` int NOT NULL DEFAULT '0',
  `GroupsAllowed` int NOT NULL DEFAULT '0',
  `Name_Lang_enUS` text,
  `Name_Lang_enGB` text,
  `Name_Lang_koKR` text,
  `Name_Lang_frFR` text,
  `Name_Lang_deDE` text,
  `Name_Lang_enCN` text,
  `Name_Lang_zhCN` text,
  `Name_Lang_enTW` text,
  `Name_Lang_zhTW` text,
  `Name_Lang_esES` text,
  `Name_Lang_esMX` text,
  `Name_Lang_ruRU` text,
  `Name_Lang_ptPT` text,
  `Name_Lang_ptBR` text,
  `Name_Lang_itIT` text,
  `Name_Lang_Unk` text,
  `Name_Lang_Mask` int unsigned NOT NULL DEFAULT '0',
  `MaxGroupSize` int NOT NULL DEFAULT '0',
  `HolidayWorldState` int NOT NULL DEFAULT '0',
  `Minlevel` int NOT NULL DEFAULT '0',
  `Maxlevel` int NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

/*Data for the table `dbc_battlemasterlist` */

insert  into `battlemasterlistdbc`(`ID`,`MapID_1`,`MapID_2`,`MapID_3`,`MapID_4`,`MapID_5`,`MapID_6`,`MapID_7`,`MapID_8`,`InstanceType`,`GroupsAllowed`,`Name_Lang_enUS`,`Name_Lang_enGB`,`Name_Lang_koKR`,`Name_Lang_frFR`,`Name_Lang_deDE`,`Name_Lang_enCN`,`Name_Lang_zhCN`,`Name_Lang_enTW`,`Name_Lang_zhTW`,`Name_Lang_esES`,`Name_Lang_esMX`,`Name_Lang_ruRU`,`Name_Lang_ptPT`,`Name_Lang_ptBR`,`Name_Lang_itIT`,`Name_Lang_Unk`,`Name_Lang_Mask`,`MaxGroupSize`,`HolidayWorldState`,`Minlevel`,`Maxlevel`) values 
(1,30,-1,-1,-1,-1,-1,-1,-1,3,1,'Alterac Valley','','','Vallée d\'Alterac','Alteractal','','奥特兰克山谷','','','Valle de Alterac','','Альтеракская долина','','','','',16712190,5,1941,51,80),
(2,489,-1,-1,-1,-1,-1,-1,-1,3,1,'Warsong Gulch','','','Goulet des Chanteguerres','Kriegshymnenschlucht','','战歌峡谷','','','Garganta Grito de Guerra','','Ущелье Песни Войны','','','','',16712190,10,1942,10,80),
(3,529,-1,-1,-1,-1,-1,-1,-1,3,1,'Arathi Basin','','','Bassin d\'Arathi','Arathibecken','','阿拉希盆地','','','Cuenca de Arathi','','Низина Арати','','','','',16712190,15,1943,20,80),
(4,559,-1,-1,-1,-1,-1,-1,-1,4,1,'Nagrand Arena','','','Arène de Nagrand','Arena von Nagrand','','纳格兰竞技场','','','Arena de Nagrand','','Арена Награнда','','','','',16712190,5,0,10,80),
(5,562,-1,-1,-1,-1,-1,-1,-1,4,1,'Blade\'s Edge Arena','','','Arène des Tranchantes','Arena des Schergrats','','刀锋竞技场','','','Arena Filospada','','Арена Острогорья','','','','',16712190,5,0,10,80),
(6,559,562,572,617,618,-1,-1,-1,4,1,'All Arenas','','','Toutes les arènes','Alle Arenen','','所有竞技场','','','Todas las arenas','','Все арены','','','','',16712190,5,0,10,80),
(7,566,-1,-1,-1,-1,-1,-1,-1,3,1,'Eye of the Storm','','','L\'Œil du cyclone','Auge des Sturms','','风暴之眼','','','Ojo de la Tormenta','','Око Бури','','','','',16712190,15,2851,61,80),
(8,572,-1,-1,-1,-1,-1,-1,-1,4,1,'Ruins of Lordaeron','','','Ruines de Lordaeron','Ruinen von Lordaeron','','洛丹伦废墟','','','Ruinas de Lordaeron','','Руины Лордерона','','','','',16712190,5,0,10,80),
(9,607,-1,-1,-1,-1,-1,-1,-1,3,1,'Strand of the Ancients','','','Rivage des Anciens','Strand der Uralten','','远古海滩','','','Playa de los Ancestros','','Берег Древних','','','','',16712190,15,3695,71,80),
(10,617,-1,-1,-1,-1,-1,-1,-1,4,1,'Dalaran Sewers','','','Egouts de Dalaran','Abwasserkanäle von Dalaran','','达拉然下水道','','','Cloacas de Dalaran','','Стоки Даларана','','','','',16712190,5,0,10,80),
(11,618,-1,-1,-1,-1,-1,-1,-1,4,1,'The Ring of Valor','','','L\'arène des valeureux','Der Ring der Ehre','','勇气竞技场','','','El Círculo del Valor','','Арена Доблести','','','','',16712190,5,0,10,80),
(30,628,-1,-1,-1,-1,-1,-1,-1,3,1,'Isle of Conquest','','','Île des Conquérants','Insel der Eroberung','','征服之岛','','','Isla de la Conquista','','Остров Завоеваний','','','','',16712190,5,4273,71,80),
(32,30,489,529,566,607,628,-1,-1,3,1,'Random Battleground','','','Champ de bataille aléatoire','Zufälliges Schlachtfeld','','随机战场','','','Campo de batalla aleatorio','','Случайное поле боя','','','','',16712190,5,0,0,0);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
