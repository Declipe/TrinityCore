/*
SQLyog Community v13.2.0 (64 bit)
MySQL - 8.0.35 : Database - world3.3.5_beta
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
CREATE DATABASE /*!32312 IF NOT EXISTS*/`world3.3.5_beta` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

/*Table structure for table `trinity_string2` */

DROP TABLE IF EXISTS `trinity_string2`;

CREATE TABLE `trinity_string2` (
  `entry` int unsigned NOT NULL DEFAULT '0',
  `content_default` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `content_loc1` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  `content_loc2` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  `content_loc3` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  `content_loc4` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  `content_loc5` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  `content_loc6` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  `content_loc7` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  `content_loc8` mediumtext CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

/*Data for the table `trinity_string2` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
