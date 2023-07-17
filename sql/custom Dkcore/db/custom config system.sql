/*
SQLyog Community v13.2.0 (64 bit)
MySQL - 8.0.33 : Database - zyn
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`zyn` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

/*Table structure for table `custom_config` */

DROP TABLE IF EXISTS `custom_config`;

CREATE TABLE `custom_config` (
  `OptionName` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `Type` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL COMMENT 'Support types - bool, int, float, string',
  `DefaultValue` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `CustomValue` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci DEFAULT NULL,
  `Description` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci,
  PRIMARY KEY (`OptionName`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3 ROW_FORMAT=DYNAMIC;

/*Data for the table `custom_config` */
insert  into `custom_config`(`OptionName`,`Type`,`DefaultValue`,`CustomValue`,`Description`) values 
('ItemUpgrade.Enable','bool','0','1','on/off system'), 
('ItemUpgrade.Text.AreYouSure','int','0','11300','String ID from `trinity_string` for gossip menu'),
('ItemUpgrade.Text.NoEffect','int','0','11301','String ID from `trinity_string` for gossip menu'),
('ItemUpgrade.Text.EffectNow','int','0','11302','String ID from `trinity_string` for gossip menu'),
('ItemUpgrade.Text.EffectRemove','int','0','11303','String ID from `trinity_string` for gossip menu'),
('AutoLearn.Enable','bool','0','1','Enabe/Disable Autolearn System'),
('AutoLearn.Check.Level','bool','0','1','Enabe/Disable Check Level For Spells'),
('AutoLearn.SpellClass','bool','0','1','Enabe/Disable Learn Class Spells'),
('AutoLearn.SpellRiding','bool','0','1','Enabe/Disable Learn Riding Spells'),
('AutoLearn.SpellMount','bool','0','1','Enabe/Disable Learn Mount Spells'),
('AutoLearn.SpellWeapon','bool','0','1','Enabe/Disable Learn Weapon Spells'),
('AutoLearn.DualSpec','bool','0','1','Enabe/Disable Learn Dual Spec.'),
('AutoLearn.Login.Spell','bool','0','1','Check spells on login'),
('AutoLearn.Create.Spell','bool','0','0','test'),
('AutoLearn.SpellProfession','bool','0','1','Enabe/Disable Learn Profession Spells.'),
('AutoLearn.Login.Skill','bool','0','1','Check skills on login'),
('AutoLearn.Create.Skill','bool','0','0','test'),
('BossHistory.Enable','bool','0','1','test'),
('BossHistory.InstanceOnly','bool','0','1','test'),
('BossHistory.Announce','bool','0','1','test'),
('BossHistory.Log','bool','0','1','test'),
('BossHistory.NormalTextId','int','0','31006','test'),
('BossHistory.HeroicTextId','int','0','31007','test'),
('DefaultSpec.Enable','bool','0','1','on/off system'),
('GuildDefault.IDA','int','0','0','guild def A'),
('GuildDefault.IDH','int','0','0','guild def H'),
('HonorRank.Enable','bool','0','0','on/off honor rank'),
('HonorRank.Check.Kill','bool','0','0','#check on kill'),
('HonorRank.Check.Login','bool','0','0','check on login'),
('HonorRank.SetNew','bool','0','0','set new rank'),
('SpecialCode.Enable','bool','0','1','on/off system'),
('Tournaments.Enable','bool','0','1','on/off system'),
('Tournaments.Debug','bool','0','0','Debug mode for Tournaments System'),
('Tournaments.Ladder','bool','0','1','Enable/Disable Ladder system for Tournaments. (1..2..3..levels, die: reset'),
('Tournaments.LadderText','int','0','0','Trinity string for organizer gossip menu (default: "[%u] Next level'),
('CustomRewardPvP.Enable','bool','0','0','on/off system'),
('DeathKnight.SkipQuest','bool','0','0','on/off system'),
('Ip.enabled','bool','0','0','system 1ip na bg'),
('PlayedTimeReward.Interval','int','0','3600000','ms time convert'),
('PlayedTimeReward.Money','int','0','50','on/off system'),
('PlayedTimeReward.Honor','int','0','500','on/off system'),
('PlayedTimeReward.Arena','int','0','500','on/off system'),
('PlayedTimeReward.item1id','int','0','0','item id'),
('PlayedTimeReward.item2id','int','0','0','item id'),
('PlayedTimeReward.item3id','int','0','0','item id'),
('PlayedTimeReward.item4id','int','0','0','item id'),
('PlayedTimeReward.item5id','int','0','49426','item id'),
('PlayedTimeReward.item1','int','0','0','item col-vo'),
('PlayedTimeReward.item2','int','0','0','item col-vo'),
('PlayedTimeReward.item3','int','0','0','item col-vo'),
('PlayedTimeReward.item4','int','0','0','item col-vo'),
('PlayedTimeReward.item5','int','0','100','item col-vo'),
('Item.Disableall','bool','1','0','item disable all'),
('Item.Disable1','bool','1','1','item disable'),
('Item.Disable2','bool','1','1','item disable'),
('Item.Disable3','bool','1','1','item disable'),
('Item.Disable4','bool','1','1','item disable'),
('Item.Disable5','bool','1','0','item disable'),
('Command.Bank.Premium','bool','0','1','on/off'),
('Command.Mail.Premium','bool','0','1','on/off'),
('Vip.Debuff.Command','bool','0','1','on/off'),
('Vip.Repair.Command','bool','0','1','on/off'),
('Vip.Reset.Talents.Command','bool','0','1','on/off'),
('Vip.Taxi.Command','bool','0','1','on/off'),
('Vip.Home.Command','bool','0','1','on/off'),
('Vip.Capital.Command','bool','0','1','on/off'),
('Vip.Customize.Command','bool','0','1','on/off'),
('Vip.Changerace.Command','bool','0','1','on/off'),
('Vip.Appear.Command','bool','0','1','on/off'),
('Rate.XP.Kill.Premium','float','1','50','rate'),
('Rate.XP.Quest.Premium','float','1','1','rate'),
('Rate.XP.Explore.Premium','float','1','1','rate'),
('Rate.Honor.Premium','float','1','1','rate'),
('Rate.Reputation.Premium','float','1','1','rate'),
('Guildhouse.TokenOrGold','bool','0','1','Use tokens or money to buy guild houses'),
('Guildhouse.BuyCost','int','1000','1000','Amount of gold or quantity of tokens to buy guildhouse'),
('Guildhouse.SellCost','int','500','400','Amount of gold or quantity of tokens gained when selling guildhouse'),
('GuildHouse.Cost','int','0','0','on/off'),
('Arena.1v1.Enable','bool','0','1','on/off'),
('Arena.1v1.Announcer','bool','0','0','on/off'),
('Arena.1v1.MinLevel','int','0','80','on/off'),
('Arena.1v1.Costs','int','0','400000','on/off'),
('Arena.1v1.VendorRating','bool','0','0','on/off'),
('Arena.1v1.ArenaPointsMulti','float','0.64','0.64',''),
('Arena.1v1.BlockForbiddenTalents','bool','0','1','on/off'),
('TestOption.Bool','bool','1',NULL,'Test option bool'),
('TestOption.Float','float','1.5232',NULL,'Test option float'),
('TestOption.Int','int','100',NULL,'Test option int'),
('TestOption.String','string','test string',NULL,'Test option string');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
