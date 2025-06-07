/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 80041 (8.0.41)
 Source Host           : localhost:3306
 Source Schema         : qwe

 Target Server Type    : MySQL
 Target Server Version : 80041 (8.0.41)
 File Encoding         : 65001

 Date: 07/06/2025 19:28:26
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for account_data
-- ----------------------------
DROP TABLE IF EXISTS `account_data`;
CREATE TABLE `account_data`  (
  `accountId` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Account Identifier',
  `type` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  `data` blob NOT NULL,
  PRIMARY KEY (`accountId`, `type`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of account_data
-- ----------------------------

-- ----------------------------
-- Table structure for account_instance_times
-- ----------------------------
DROP TABLE IF EXISTS `account_instance_times`;
CREATE TABLE `account_instance_times`  (
  `accountId` int UNSIGNED NOT NULL,
  `instanceId` int UNSIGNED NOT NULL DEFAULT 0,
  `releaseTime` bigint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`accountId`, `instanceId`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of account_instance_times
-- ----------------------------

-- ----------------------------
-- Table structure for account_tutorial
-- ----------------------------
DROP TABLE IF EXISTS `account_tutorial`;
CREATE TABLE `account_tutorial`  (
  `accountId` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Account Identifier',
  `tut0` int UNSIGNED NOT NULL DEFAULT 0,
  `tut1` int UNSIGNED NOT NULL DEFAULT 0,
  `tut2` int UNSIGNED NOT NULL DEFAULT 0,
  `tut3` int UNSIGNED NOT NULL DEFAULT 0,
  `tut4` int UNSIGNED NOT NULL DEFAULT 0,
  `tut5` int UNSIGNED NOT NULL DEFAULT 0,
  `tut6` int UNSIGNED NOT NULL DEFAULT 0,
  `tut7` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`accountId`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of account_tutorial
-- ----------------------------

-- ----------------------------
-- Table structure for addons
-- ----------------------------
DROP TABLE IF EXISTS `addons`;
CREATE TABLE `addons`  (
  `name` varchar(120) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `crc` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`name`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Addons' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of addons
-- ----------------------------

-- ----------------------------
-- Table structure for arena_team
-- ----------------------------
DROP TABLE IF EXISTS `arena_team`;
CREATE TABLE `arena_team`  (
  `arenaTeamId` int UNSIGNED NOT NULL DEFAULT 0,
  `name` varchar(24) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `captainGuid` int UNSIGNED NOT NULL DEFAULT 0,
  `type` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `rating` smallint UNSIGNED NOT NULL DEFAULT 0,
  `seasonGames` smallint UNSIGNED NOT NULL DEFAULT 0,
  `seasonWins` smallint UNSIGNED NOT NULL DEFAULT 0,
  `weekGames` smallint UNSIGNED NOT NULL DEFAULT 0,
  `weekWins` smallint UNSIGNED NOT NULL DEFAULT 0,
  `rank` int UNSIGNED NOT NULL DEFAULT 0,
  `backgroundColor` int UNSIGNED NOT NULL DEFAULT 0,
  `emblemStyle` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `emblemColor` int UNSIGNED NOT NULL DEFAULT 0,
  `borderStyle` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `borderColor` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`arenaTeamId`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of arena_team
-- ----------------------------

-- ----------------------------
-- Table structure for arena_team_member
-- ----------------------------
DROP TABLE IF EXISTS `arena_team_member`;
CREATE TABLE `arena_team_member`  (
  `arenaTeamId` int UNSIGNED NOT NULL DEFAULT 0,
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `weekGames` smallint UNSIGNED NOT NULL DEFAULT 0,
  `weekWins` smallint UNSIGNED NOT NULL DEFAULT 0,
  `seasonGames` smallint UNSIGNED NOT NULL DEFAULT 0,
  `seasonWins` smallint UNSIGNED NOT NULL DEFAULT 0,
  `personalRating` smallint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`arenaTeamId`, `guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of arena_team_member
-- ----------------------------

-- ----------------------------
-- Table structure for auctionbidders
-- ----------------------------
DROP TABLE IF EXISTS `auctionbidders`;
CREATE TABLE `auctionbidders`  (
  `id` int UNSIGNED NOT NULL DEFAULT 0,
  `bidderguid` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`, `bidderguid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of auctionbidders
-- ----------------------------

-- ----------------------------
-- Table structure for auctionhouse
-- ----------------------------
DROP TABLE IF EXISTS `auctionhouse`;
CREATE TABLE `auctionhouse`  (
  `id` int UNSIGNED NOT NULL DEFAULT 0,
  `houseid` tinyint UNSIGNED NOT NULL DEFAULT 7,
  `itemguid` int UNSIGNED NOT NULL DEFAULT 0,
  `itemowner` int UNSIGNED NOT NULL DEFAULT 0,
  `buyoutprice` int UNSIGNED NOT NULL DEFAULT 0,
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  `buyguid` int UNSIGNED NOT NULL DEFAULT 0,
  `lastbid` int UNSIGNED NOT NULL DEFAULT 0,
  `startbid` int UNSIGNED NOT NULL DEFAULT 0,
  `deposit` int UNSIGNED NOT NULL DEFAULT 0,
  `Flags` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `item_guid`(`itemguid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of auctionhouse
-- ----------------------------

-- ----------------------------
-- Table structure for banned_addons
-- ----------------------------
DROP TABLE IF EXISTS `banned_addons`;
CREATE TABLE `banned_addons`  (
  `Id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `Name` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `Version` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `Timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`Id`) USING BTREE,
  UNIQUE INDEX `idx_name_ver`(`Name` ASC, `Version` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of banned_addons
-- ----------------------------

-- ----------------------------
-- Table structure for battleground_deserters
-- ----------------------------
DROP TABLE IF EXISTS `battleground_deserters`;
CREATE TABLE `battleground_deserters`  (
  `guid` int UNSIGNED NOT NULL COMMENT 'characters.guid',
  `type` tinyint UNSIGNED NOT NULL COMMENT 'type of the desertion',
  `datetime` datetime NOT NULL COMMENT 'datetime of the desertion'
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of battleground_deserters
-- ----------------------------

-- ----------------------------
-- Table structure for boss_history
-- ----------------------------
DROP TABLE IF EXISTS `boss_history`;
CREATE TABLE `boss_history`  (
  `Guid` bigint NOT NULL AUTO_INCREMENT,
  `GuildId` int NOT NULL,
  `CreatureEntry` int NOT NULL,
  `CreatureName` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `KillerCount` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `KillTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `KillData` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `MapRaid` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `MapId` smallint UNSIGNED NOT NULL DEFAULT 0,
  `MapName` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `MapSpawnMode` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `MapMaxPlayers` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`Guid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 177 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of boss_history
-- ----------------------------

-- ----------------------------
-- Table structure for bugreport
-- ----------------------------
DROP TABLE IF EXISTS `bugreport`;
CREATE TABLE `bugreport`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT COMMENT 'Identifier',
  `type` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `content` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Debug System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of bugreport
-- ----------------------------

-- ----------------------------
-- Table structure for calendar_events
-- ----------------------------
DROP TABLE IF EXISTS `calendar_events`;
CREATE TABLE `calendar_events`  (
  `id` bigint UNSIGNED NOT NULL DEFAULT 0,
  `creator` int UNSIGNED NOT NULL DEFAULT 0,
  `title` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `description` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `type` tinyint UNSIGNED NOT NULL DEFAULT 4,
  `dungeon` int NOT NULL DEFAULT -1,
  `eventtime` int UNSIGNED NOT NULL DEFAULT 0,
  `flags` int UNSIGNED NOT NULL DEFAULT 0,
  `time2` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of calendar_events
-- ----------------------------

-- ----------------------------
-- Table structure for calendar_invites
-- ----------------------------
DROP TABLE IF EXISTS `calendar_invites`;
CREATE TABLE `calendar_invites`  (
  `id` bigint UNSIGNED NOT NULL DEFAULT 0,
  `event` bigint UNSIGNED NOT NULL DEFAULT 0,
  `invitee` int UNSIGNED NOT NULL DEFAULT 0,
  `sender` int UNSIGNED NOT NULL DEFAULT 0,
  `status` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `statustime` int UNSIGNED NOT NULL DEFAULT 0,
  `rank` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `text` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of calendar_invites
-- ----------------------------

-- ----------------------------
-- Table structure for channels
-- ----------------------------
DROP TABLE IF EXISTS `channels`;
CREATE TABLE `channels`  (
  `name` varchar(128) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `team` int UNSIGNED NOT NULL,
  `announce` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `ownership` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `password` varchar(32) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL DEFAULT NULL,
  `bannedList` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `lastUsed` int UNSIGNED NOT NULL,
  PRIMARY KEY (`name`, `team`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Channel System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of channels
-- ----------------------------

-- ----------------------------
-- Table structure for character_account_data
-- ----------------------------
DROP TABLE IF EXISTS `character_account_data`;
CREATE TABLE `character_account_data`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `type` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  `data` blob NOT NULL,
  PRIMARY KEY (`guid`, `type`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_account_data
-- ----------------------------

-- ----------------------------
-- Table structure for character_achievement
-- ----------------------------
DROP TABLE IF EXISTS `character_achievement`;
CREATE TABLE `character_achievement`  (
  `guid` int UNSIGNED NOT NULL,
  `achievement` smallint UNSIGNED NOT NULL,
  `date` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `achievement`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_achievement
-- ----------------------------

-- ----------------------------
-- Table structure for character_achievement_progress
-- ----------------------------
DROP TABLE IF EXISTS `character_achievement_progress`;
CREATE TABLE `character_achievement_progress`  (
  `guid` int UNSIGNED NOT NULL,
  `criteria` smallint UNSIGNED NOT NULL,
  `counter` int UNSIGNED NOT NULL,
  `date` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `criteria`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_achievement_progress
-- ----------------------------

-- ----------------------------
-- Table structure for character_action
-- ----------------------------
DROP TABLE IF EXISTS `character_action`;
CREATE TABLE `character_action`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `spec` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `button` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `action` int UNSIGNED NOT NULL DEFAULT 0,
  `type` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `spec`, `button`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_action
-- ----------------------------

-- ----------------------------
-- Table structure for character_arena_stats
-- ----------------------------
DROP TABLE IF EXISTS `character_arena_stats`;
CREATE TABLE `character_arena_stats`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `slot` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `matchMakerRating` smallint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `slot`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_arena_stats
-- ----------------------------

-- ----------------------------
-- Table structure for character_aura
-- ----------------------------
DROP TABLE IF EXISTS `character_aura`;
CREATE TABLE `character_aura`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `casterGuid` bigint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Full Global Unique Identifier',
  `itemGuid` bigint UNSIGNED NOT NULL DEFAULT 0,
  `spell` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `effectMask` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `recalculateMask` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `stackCount` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `amount0` int NOT NULL DEFAULT 0,
  `amount1` int NOT NULL DEFAULT 0,
  `amount2` int NOT NULL DEFAULT 0,
  `base_amount0` int NOT NULL DEFAULT 0,
  `base_amount1` int NOT NULL DEFAULT 0,
  `base_amount2` int NOT NULL DEFAULT 0,
  `maxDuration` int NOT NULL DEFAULT 0,
  `remainTime` int NOT NULL DEFAULT 0,
  `remainCharges` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `critChance` float NOT NULL DEFAULT 0,
  `applyResilience` tinyint NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `casterGuid`, `itemGuid`, `spell`, `effectMask`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_aura
-- ----------------------------

-- ----------------------------
-- Table structure for character_banned
-- ----------------------------
DROP TABLE IF EXISTS `character_banned`;
CREATE TABLE `character_banned`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `bandate` int UNSIGNED NOT NULL DEFAULT 0,
  `unbandate` int UNSIGNED NOT NULL DEFAULT 0,
  `bannedby` varchar(50) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `banreason` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `active` tinyint UNSIGNED NOT NULL DEFAULT 1,
  PRIMARY KEY (`guid`, `bandate`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Ban List' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_banned
-- ----------------------------

-- ----------------------------
-- Table structure for character_battleground_data
-- ----------------------------
DROP TABLE IF EXISTS `character_battleground_data`;
CREATE TABLE `character_battleground_data`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `instanceId` int UNSIGNED NOT NULL COMMENT 'Instance Identifier',
  `team` smallint UNSIGNED NOT NULL,
  `joinX` float NOT NULL DEFAULT 0,
  `joinY` float NOT NULL DEFAULT 0,
  `joinZ` float NOT NULL DEFAULT 0,
  `joinO` float NOT NULL DEFAULT 0,
  `joinMapId` smallint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Map Identifier',
  `taxiStart` int UNSIGNED NOT NULL DEFAULT 0,
  `taxiEnd` int UNSIGNED NOT NULL DEFAULT 0,
  `mountSpell` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_battleground_data
-- ----------------------------

-- ----------------------------
-- Table structure for character_battleground_random
-- ----------------------------
DROP TABLE IF EXISTS `character_battleground_random`;
CREATE TABLE `character_battleground_random`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_battleground_random
-- ----------------------------

-- ----------------------------
-- Table structure for character_declinedname
-- ----------------------------
DROP TABLE IF EXISTS `character_declinedname`;
CREATE TABLE `character_declinedname`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `genitive` varchar(15) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `dative` varchar(15) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `accusative` varchar(15) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `instrumental` varchar(15) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `prepositional` varchar(15) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_declinedname
-- ----------------------------

-- ----------------------------
-- Table structure for character_equipmentsets
-- ----------------------------
DROP TABLE IF EXISTS `character_equipmentsets`;
CREATE TABLE `character_equipmentsets`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `setguid` bigint UNSIGNED NOT NULL AUTO_INCREMENT,
  `setindex` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `name` varchar(31) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `iconname` varchar(100) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `ignore_mask` int UNSIGNED NOT NULL DEFAULT 0,
  `item0` int UNSIGNED NOT NULL DEFAULT 0,
  `item1` int UNSIGNED NOT NULL DEFAULT 0,
  `item2` int UNSIGNED NOT NULL DEFAULT 0,
  `item3` int UNSIGNED NOT NULL DEFAULT 0,
  `item4` int UNSIGNED NOT NULL DEFAULT 0,
  `item5` int UNSIGNED NOT NULL DEFAULT 0,
  `item6` int UNSIGNED NOT NULL DEFAULT 0,
  `item7` int UNSIGNED NOT NULL DEFAULT 0,
  `item8` int UNSIGNED NOT NULL DEFAULT 0,
  `item9` int UNSIGNED NOT NULL DEFAULT 0,
  `item10` int UNSIGNED NOT NULL DEFAULT 0,
  `item11` int UNSIGNED NOT NULL DEFAULT 0,
  `item12` int UNSIGNED NOT NULL DEFAULT 0,
  `item13` int UNSIGNED NOT NULL DEFAULT 0,
  `item14` int UNSIGNED NOT NULL DEFAULT 0,
  `item15` int UNSIGNED NOT NULL DEFAULT 0,
  `item16` int UNSIGNED NOT NULL DEFAULT 0,
  `item17` int UNSIGNED NOT NULL DEFAULT 0,
  `item18` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`setguid`) USING BTREE,
  UNIQUE INDEX `idx_set`(`guid` ASC, `setguid` ASC, `setindex` ASC) USING BTREE,
  INDEX `Idx_setindex`(`setindex` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_equipmentsets
-- ----------------------------

-- ----------------------------
-- Table structure for character_fishingsteps
-- ----------------------------
DROP TABLE IF EXISTS `character_fishingsteps`;
CREATE TABLE `character_fishingsteps`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `fishingSteps` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_fishingsteps
-- ----------------------------

-- ----------------------------
-- Table structure for character_gifts
-- ----------------------------
DROP TABLE IF EXISTS `character_gifts`;
CREATE TABLE `character_gifts`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `item_guid` int UNSIGNED NOT NULL DEFAULT 0,
  `entry` int UNSIGNED NOT NULL DEFAULT 0,
  `flags` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`item_guid`) USING BTREE,
  INDEX `idx_guid`(`guid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_gifts
-- ----------------------------

-- ----------------------------
-- Table structure for character_glyphs
-- ----------------------------
DROP TABLE IF EXISTS `character_glyphs`;
CREATE TABLE `character_glyphs`  (
  `guid` int UNSIGNED NOT NULL,
  `talentGroup` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `glyph1` smallint UNSIGNED NULL DEFAULT 0,
  `glyph2` smallint UNSIGNED NULL DEFAULT 0,
  `glyph3` smallint UNSIGNED NULL DEFAULT 0,
  `glyph4` smallint UNSIGNED NULL DEFAULT 0,
  `glyph5` smallint UNSIGNED NULL DEFAULT 0,
  `glyph6` smallint UNSIGNED NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `talentGroup`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_glyphs
-- ----------------------------

-- ----------------------------
-- Table structure for character_homebind
-- ----------------------------
DROP TABLE IF EXISTS `character_homebind`;
CREATE TABLE `character_homebind`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `mapId` smallint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Map Identifier',
  `zoneId` smallint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Zone Identifier',
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_homebind
-- ----------------------------

-- ----------------------------
-- Table structure for character_instance
-- ----------------------------
DROP TABLE IF EXISTS `character_instance`;
CREATE TABLE `character_instance`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `instance` int UNSIGNED NOT NULL DEFAULT 0,
  `permanent` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `extendState` tinyint UNSIGNED NOT NULL DEFAULT 1,
  PRIMARY KEY (`guid`, `instance`) USING BTREE,
  INDEX `instance`(`instance` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_instance
-- ----------------------------

-- ----------------------------
-- Table structure for character_inventory
-- ----------------------------
DROP TABLE IF EXISTS `character_inventory`;
CREATE TABLE `character_inventory`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `bag` int UNSIGNED NOT NULL DEFAULT 0,
  `slot` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `item` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Item Global Unique Identifier',
  PRIMARY KEY (`item`) USING BTREE,
  UNIQUE INDEX `guid`(`guid` ASC, `bag` ASC, `slot` ASC) USING BTREE,
  INDEX `idx_guid`(`guid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_inventory
-- ----------------------------

-- ----------------------------
-- Table structure for character_item_upgrade
-- ----------------------------
DROP TABLE IF EXISTS `character_item_upgrade`;
CREATE TABLE `character_item_upgrade`  (
  `guid` int UNSIGNED NOT NULL,
  `item_guid` int UNSIGNED NOT NULL,
  `stat_id` int UNSIGNED NOT NULL,
  PRIMARY KEY (`guid`, `item_guid`, `stat_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_item_upgrade
-- ----------------------------
INSERT INTO `character_item_upgrade` VALUES (12, 2668049, 33);
INSERT INTO `character_item_upgrade` VALUES (12, 2668049, 34);
INSERT INTO `character_item_upgrade` VALUES (12, 2668049, 37);
INSERT INTO `character_item_upgrade` VALUES (12, 2668049, 39);
INSERT INTO `character_item_upgrade` VALUES (12, 2668049, 40);
INSERT INTO `character_item_upgrade` VALUES (13, 1319042, 99);
INSERT INTO `character_item_upgrade` VALUES (13, 1319042, 100);
INSERT INTO `character_item_upgrade` VALUES (13, 1319042, 101);
INSERT INTO `character_item_upgrade` VALUES (13, 4419873, 101);
INSERT INTO `character_item_upgrade` VALUES (13, 4419873, 102);
INSERT INTO `character_item_upgrade` VALUES (60, 3668935, 99);
INSERT INTO `character_item_upgrade` VALUES (60, 3668935, 101);
INSERT INTO `character_item_upgrade` VALUES (60, 3668935, 102);
INSERT INTO `character_item_upgrade` VALUES (60, 3668935, 104);

-- ----------------------------
-- Table structure for character_pet
-- ----------------------------
DROP TABLE IF EXISTS `character_pet`;
CREATE TABLE `character_pet`  (
  `id` int UNSIGNED NOT NULL DEFAULT 0,
  `entry` int UNSIGNED NOT NULL DEFAULT 0,
  `owner` int UNSIGNED NOT NULL DEFAULT 0,
  `modelid` int UNSIGNED NULL DEFAULT 0,
  `CreatedBySpell` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `PetType` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `level` smallint UNSIGNED NOT NULL DEFAULT 1,
  `exp` int UNSIGNED NOT NULL DEFAULT 0,
  `Reactstate` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `name` varchar(21) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT 'Pet',
  `renamed` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `slot` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `curhealth` int UNSIGNED NOT NULL DEFAULT 1,
  `curmana` int UNSIGNED NOT NULL DEFAULT 0,
  `curhappiness` int UNSIGNED NOT NULL DEFAULT 0,
  `savetime` int UNSIGNED NOT NULL DEFAULT 0,
  `abdata` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `owner`(`owner` ASC) USING BTREE,
  INDEX `idx_slot`(`slot` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Pet System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_pet
-- ----------------------------

-- ----------------------------
-- Table structure for character_pet_declinedname
-- ----------------------------
DROP TABLE IF EXISTS `character_pet_declinedname`;
CREATE TABLE `character_pet_declinedname`  (
  `id` int UNSIGNED NOT NULL DEFAULT 0,
  `owner` int UNSIGNED NOT NULL DEFAULT 0,
  `genitive` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `dative` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `accusative` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `instrumental` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `prepositional` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `owner_key`(`owner` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_pet_declinedname
-- ----------------------------

-- ----------------------------
-- Table structure for character_queststatus
-- ----------------------------
DROP TABLE IF EXISTS `character_queststatus`;
CREATE TABLE `character_queststatus`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  `status` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `explored` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `timer` int UNSIGNED NOT NULL DEFAULT 0,
  `mobcount1` smallint UNSIGNED NOT NULL DEFAULT 0,
  `mobcount2` smallint UNSIGNED NOT NULL DEFAULT 0,
  `mobcount3` smallint UNSIGNED NOT NULL DEFAULT 0,
  `mobcount4` smallint UNSIGNED NOT NULL DEFAULT 0,
  `itemcount1` smallint UNSIGNED NOT NULL DEFAULT 0,
  `itemcount2` smallint UNSIGNED NOT NULL DEFAULT 0,
  `itemcount3` smallint UNSIGNED NOT NULL DEFAULT 0,
  `itemcount4` smallint UNSIGNED NOT NULL DEFAULT 0,
  `itemcount5` smallint UNSIGNED NOT NULL DEFAULT 0,
  `itemcount6` smallint UNSIGNED NOT NULL DEFAULT 0,
  `playercount` smallint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `quest`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_queststatus
-- ----------------------------

-- ----------------------------
-- Table structure for character_queststatus_daily
-- ----------------------------
DROP TABLE IF EXISTS `character_queststatus_daily`;
CREATE TABLE `character_queststatus_daily`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `quest`) USING BTREE,
  INDEX `idx_guid`(`guid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_queststatus_daily
-- ----------------------------

-- ----------------------------
-- Table structure for character_queststatus_monthly
-- ----------------------------
DROP TABLE IF EXISTS `character_queststatus_monthly`;
CREATE TABLE `character_queststatus_monthly`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`, `quest`) USING BTREE,
  INDEX `idx_guid`(`guid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_queststatus_monthly
-- ----------------------------

-- ----------------------------
-- Table structure for character_queststatus_rewarded
-- ----------------------------
DROP TABLE IF EXISTS `character_queststatus_rewarded`;
CREATE TABLE `character_queststatus_rewarded`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  `active` tinyint UNSIGNED NOT NULL DEFAULT 1,
  PRIMARY KEY (`guid`, `quest`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_queststatus_rewarded
-- ----------------------------

-- ----------------------------
-- Table structure for character_queststatus_seasonal
-- ----------------------------
DROP TABLE IF EXISTS `character_queststatus_seasonal`;
CREATE TABLE `character_queststatus_seasonal`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  `event` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Event Identifier',
  PRIMARY KEY (`guid`, `quest`) USING BTREE,
  INDEX `idx_guid`(`guid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_queststatus_seasonal
-- ----------------------------

-- ----------------------------
-- Table structure for character_queststatus_weekly
-- ----------------------------
DROP TABLE IF EXISTS `character_queststatus_weekly`;
CREATE TABLE `character_queststatus_weekly`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `quest` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Quest Identifier',
  PRIMARY KEY (`guid`, `quest`) USING BTREE,
  INDEX `idx_guid`(`guid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_queststatus_weekly
-- ----------------------------

-- ----------------------------
-- Table structure for character_reputation
-- ----------------------------
DROP TABLE IF EXISTS `character_reputation`;
CREATE TABLE `character_reputation`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `faction` smallint UNSIGNED NOT NULL DEFAULT 0,
  `standing` int NOT NULL DEFAULT 0,
  `flags` smallint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `faction`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_reputation
-- ----------------------------

-- ----------------------------
-- Table structure for character_skills
-- ----------------------------
DROP TABLE IF EXISTS `character_skills`;
CREATE TABLE `character_skills`  (
  `guid` int UNSIGNED NOT NULL COMMENT 'Global Unique Identifier',
  `skill` smallint UNSIGNED NOT NULL,
  `value` smallint UNSIGNED NOT NULL,
  `max` smallint UNSIGNED NOT NULL,
  PRIMARY KEY (`guid`, `skill`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_skills
-- ----------------------------

-- ----------------------------
-- Table structure for character_social
-- ----------------------------
DROP TABLE IF EXISTS `character_social`;
CREATE TABLE `character_social`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  `friend` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Friend Global Unique Identifier',
  `flags` tinyint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Friend Flags',
  `note` varchar(48) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '' COMMENT 'Friend Note',
  PRIMARY KEY (`guid`, `friend`, `flags`) USING BTREE,
  INDEX `friend`(`friend` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_social
-- ----------------------------

-- ----------------------------
-- Table structure for character_spell
-- ----------------------------
DROP TABLE IF EXISTS `character_spell`;
CREATE TABLE `character_spell`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `spell` mediumint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Spell Identifier',
  `active` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `disabled` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `spell`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_spell
-- ----------------------------

-- ----------------------------
-- Table structure for character_spell_cooldown
-- ----------------------------
DROP TABLE IF EXISTS `character_spell_cooldown`;
CREATE TABLE `character_spell_cooldown`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier, Low part',
  `spell` mediumint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Spell Identifier',
  `item` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Item Identifier',
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  `categoryId` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Spell category Id',
  `categoryEnd` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `spell`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_spell_cooldown
-- ----------------------------

-- ----------------------------
-- Table structure for character_stats
-- ----------------------------
DROP TABLE IF EXISTS `character_stats`;
CREATE TABLE `character_stats`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier, Low part',
  `maxhealth` int UNSIGNED NOT NULL DEFAULT 0,
  `maxpower1` int UNSIGNED NOT NULL DEFAULT 0,
  `maxpower2` int UNSIGNED NOT NULL DEFAULT 0,
  `maxpower3` int UNSIGNED NOT NULL DEFAULT 0,
  `maxpower4` int UNSIGNED NOT NULL DEFAULT 0,
  `maxpower5` int UNSIGNED NOT NULL DEFAULT 0,
  `maxpower6` int UNSIGNED NOT NULL DEFAULT 0,
  `maxpower7` int UNSIGNED NOT NULL DEFAULT 0,
  `strength` int UNSIGNED NOT NULL DEFAULT 0,
  `agility` int UNSIGNED NOT NULL DEFAULT 0,
  `stamina` int UNSIGNED NOT NULL DEFAULT 0,
  `intellect` int UNSIGNED NOT NULL DEFAULT 0,
  `spirit` int UNSIGNED NOT NULL DEFAULT 0,
  `armor` int UNSIGNED NOT NULL DEFAULT 0,
  `resHoly` int UNSIGNED NOT NULL DEFAULT 0,
  `resFire` int UNSIGNED NOT NULL DEFAULT 0,
  `resNature` int UNSIGNED NOT NULL DEFAULT 0,
  `resFrost` int UNSIGNED NOT NULL DEFAULT 0,
  `resShadow` int UNSIGNED NOT NULL DEFAULT 0,
  `resArcane` int UNSIGNED NOT NULL DEFAULT 0,
  `blockPct` float UNSIGNED NOT NULL DEFAULT 0,
  `dodgePct` float UNSIGNED NOT NULL DEFAULT 0,
  `parryPct` float UNSIGNED NOT NULL DEFAULT 0,
  `critPct` float UNSIGNED NOT NULL DEFAULT 0,
  `rangedCritPct` float UNSIGNED NOT NULL DEFAULT 0,
  `spellCritPct` float UNSIGNED NOT NULL DEFAULT 0,
  `attackPower` int UNSIGNED NOT NULL DEFAULT 0,
  `rangedAttackPower` int UNSIGNED NOT NULL DEFAULT 0,
  `spellPower` int UNSIGNED NOT NULL DEFAULT 0,
  `resilience` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_stats
-- ----------------------------

-- ----------------------------
-- Table structure for character_talent
-- ----------------------------
DROP TABLE IF EXISTS `character_talent`;
CREATE TABLE `character_talent`  (
  `guid` int UNSIGNED NOT NULL,
  `spell` mediumint UNSIGNED NOT NULL,
  `talentGroup` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `spell`, `talentGroup`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of character_talent
-- ----------------------------

-- ----------------------------
-- Table structure for characters
-- ----------------------------
DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `account` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Account Identifier',
  `name` varchar(12) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `race` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `class` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `gender` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `level` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `xp` int UNSIGNED NOT NULL DEFAULT 0,
  `money` int UNSIGNED NOT NULL DEFAULT 0,
  `skin` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `face` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `hairStyle` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `hairColor` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `facialStyle` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `bankSlots` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `restState` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `playerFlags` int UNSIGNED NOT NULL DEFAULT 0,
  `position_x` float NOT NULL DEFAULT 0,
  `position_y` float NOT NULL DEFAULT 0,
  `position_z` float NOT NULL DEFAULT 0,
  `map` smallint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Map Identifier',
  `instance_id` int UNSIGNED NOT NULL DEFAULT 0,
  `instance_mode_mask` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `orientation` float NOT NULL DEFAULT 0,
  `taximask` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `online` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `cinematic` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `totaltime` int UNSIGNED NOT NULL DEFAULT 0,
  `leveltime` int UNSIGNED NOT NULL DEFAULT 0,
  `logout_time` int UNSIGNED NOT NULL DEFAULT 0,
  `is_logout_resting` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `rest_bonus` float NOT NULL DEFAULT 0,
  `resettalents_cost` int UNSIGNED NOT NULL DEFAULT 0,
  `resettalents_time` int UNSIGNED NOT NULL DEFAULT 0,
  `trans_x` float NOT NULL DEFAULT 0,
  `trans_y` float NOT NULL DEFAULT 0,
  `trans_z` float NOT NULL DEFAULT 0,
  `trans_o` float NOT NULL DEFAULT 0,
  `transguid` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `extra_flags` smallint UNSIGNED NOT NULL DEFAULT 0,
  `stable_slots` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `at_login` smallint UNSIGNED NOT NULL DEFAULT 0,
  `zone` smallint UNSIGNED NOT NULL DEFAULT 0,
  `death_expire_time` int UNSIGNED NOT NULL DEFAULT 0,
  `taxi_path` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `arenaPoints` int UNSIGNED NOT NULL DEFAULT 0,
  `totalHonorPoints` int UNSIGNED NOT NULL DEFAULT 0,
  `todayHonorPoints` int UNSIGNED NOT NULL DEFAULT 0,
  `yesterdayHonorPoints` int UNSIGNED NOT NULL DEFAULT 0,
  `totalKills` int UNSIGNED NOT NULL DEFAULT 0,
  `todayKills` smallint UNSIGNED NOT NULL DEFAULT 0,
  `yesterdayKills` smallint UNSIGNED NOT NULL DEFAULT 0,
  `chosenTitle` int UNSIGNED NOT NULL DEFAULT 0,
  `knownCurrencies` bigint UNSIGNED NOT NULL DEFAULT 0,
  `watchedFaction` int UNSIGNED NOT NULL DEFAULT 0,
  `drunk` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `health` int UNSIGNED NOT NULL DEFAULT 0,
  `power1` int UNSIGNED NOT NULL DEFAULT 0,
  `power2` int UNSIGNED NOT NULL DEFAULT 0,
  `power3` int UNSIGNED NOT NULL DEFAULT 0,
  `power4` int UNSIGNED NOT NULL DEFAULT 0,
  `power5` int UNSIGNED NOT NULL DEFAULT 0,
  `power6` int UNSIGNED NOT NULL DEFAULT 0,
  `power7` int UNSIGNED NOT NULL DEFAULT 0,
  `latency` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `talentGroupsCount` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `activeTalentGroup` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `exploredZones` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `equipmentCache` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `ammoId` int UNSIGNED NOT NULL DEFAULT 0,
  `knownTitles` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `actionBars` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `grantableLevels` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `deleteInfos_Account` int UNSIGNED NULL DEFAULT NULL,
  `deleteInfos_Name` varchar(12) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL DEFAULT NULL,
  `deleteDate` int UNSIGNED NULL DEFAULT NULL,
  `FreezeTimer` int UNSIGNED NOT NULL DEFAULT 0,
  `StunTimer` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE,
  UNIQUE INDEX `idx_name`(`name` ASC) USING BTREE,
  INDEX `idx_account`(`account` ASC) USING BTREE,
  INDEX `idx_online`(`online` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of characters
-- ----------------------------

-- ----------------------------
-- Table structure for characters_npcbot
-- ----------------------------
DROP TABLE IF EXISTS `characters_npcbot`;
CREATE TABLE `characters_npcbot`  (
  `entry` int UNSIGNED NOT NULL COMMENT 'creature_template.entry',
  `owner` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'characters.guid (lowguid)',
  `roles` int UNSIGNED NOT NULL COMMENT 'bitmask: tank(1),dps(2),heal(4),ranged(8)',
  `spec` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `faction` int UNSIGNED NOT NULL DEFAULT 35,
  `equipMhEx` int UNSIGNED NOT NULL DEFAULT 0,
  `equipOhEx` int UNSIGNED NOT NULL DEFAULT 0,
  `equipRhEx` int UNSIGNED NOT NULL DEFAULT 0,
  `equipHead` int UNSIGNED NOT NULL DEFAULT 0,
  `equipShoulders` int UNSIGNED NOT NULL DEFAULT 0,
  `equipChest` int UNSIGNED NOT NULL DEFAULT 0,
  `equipWaist` int UNSIGNED NOT NULL DEFAULT 0,
  `equipLegs` int UNSIGNED NOT NULL DEFAULT 0,
  `equipFeet` int UNSIGNED NOT NULL DEFAULT 0,
  `equipWrist` int UNSIGNED NOT NULL DEFAULT 0,
  `equipHands` int UNSIGNED NOT NULL DEFAULT 0,
  `equipBack` int UNSIGNED NOT NULL DEFAULT 0,
  `equipBody` int UNSIGNED NOT NULL DEFAULT 0,
  `equipFinger1` int UNSIGNED NOT NULL DEFAULT 0,
  `equipFinger2` int UNSIGNED NOT NULL DEFAULT 0,
  `equipTrinket1` int UNSIGNED NOT NULL DEFAULT 0,
  `equipTrinket2` int UNSIGNED NOT NULL DEFAULT 0,
  `equipNeck` int UNSIGNED NOT NULL DEFAULT 0,
  `spells_disabled` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  PRIMARY KEY (`entry`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of characters_npcbot
-- ----------------------------

-- ----------------------------
-- Table structure for characters_npcbot_stats
-- ----------------------------
DROP TABLE IF EXISTS `characters_npcbot_stats`;
CREATE TABLE `characters_npcbot_stats`  (
  `entry` int UNSIGNED NOT NULL DEFAULT 0,
  `maxhealth` int UNSIGNED NOT NULL DEFAULT 0,
  `maxpower` int UNSIGNED NOT NULL DEFAULT 0,
  `strength` int UNSIGNED NOT NULL DEFAULT 0,
  `agility` int UNSIGNED NOT NULL DEFAULT 0,
  `stamina` int UNSIGNED NOT NULL DEFAULT 0,
  `intellect` int UNSIGNED NOT NULL DEFAULT 0,
  `spirit` int UNSIGNED NOT NULL DEFAULT 0,
  `armor` int UNSIGNED NOT NULL DEFAULT 0,
  `defense` int UNSIGNED NOT NULL DEFAULT 0,
  `resHoly` int UNSIGNED NOT NULL DEFAULT 0,
  `resFire` int UNSIGNED NOT NULL DEFAULT 0,
  `resNature` int UNSIGNED NOT NULL DEFAULT 0,
  `resFrost` int UNSIGNED NOT NULL DEFAULT 0,
  `resShadow` int UNSIGNED NOT NULL DEFAULT 0,
  `resArcane` int UNSIGNED NOT NULL DEFAULT 0,
  `blockPct` float UNSIGNED NOT NULL DEFAULT 0,
  `dodgePct` float UNSIGNED NOT NULL DEFAULT 0,
  `parryPct` float UNSIGNED NOT NULL DEFAULT 0,
  `critPct` float UNSIGNED NOT NULL DEFAULT 0,
  `attackPower` int UNSIGNED NOT NULL DEFAULT 0,
  `spellPower` int UNSIGNED NOT NULL DEFAULT 0,
  `spellPen` int UNSIGNED NOT NULL DEFAULT 0,
  `hastePct` float UNSIGNED NOT NULL DEFAULT 0,
  `hitBonusPct` float UNSIGNED NOT NULL DEFAULT 0,
  `expertise` int UNSIGNED NOT NULL DEFAULT 0,
  `armorPenPct` float UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`entry`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of characters_npcbot_stats
-- ----------------------------

-- ----------------------------
-- Table structure for corpse
-- ----------------------------
DROP TABLE IF EXISTS `corpse`;
CREATE TABLE `corpse`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  `orientation` float NOT NULL DEFAULT 0,
  `mapId` smallint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Map Identifier',
  `phaseMask` int UNSIGNED NOT NULL DEFAULT 1,
  `displayId` int UNSIGNED NOT NULL DEFAULT 0,
  `itemCache` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `bytes1` int UNSIGNED NOT NULL DEFAULT 0,
  `bytes2` int UNSIGNED NOT NULL DEFAULT 0,
  `guildId` int UNSIGNED NOT NULL DEFAULT 0,
  `flags` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `dynFlags` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  `corpseType` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `instanceId` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Instance Identifier',
  PRIMARY KEY (`guid`) USING BTREE,
  INDEX `idx_type`(`corpseType` ASC) USING BTREE,
  INDEX `idx_instance`(`instanceId` ASC) USING BTREE,
  INDEX `idx_time`(`time` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Death System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of corpse
-- ----------------------------

-- ----------------------------
-- Table structure for custom_reforging
-- ----------------------------
DROP TABLE IF EXISTS `custom_reforging`;
CREATE TABLE `custom_reforging`  (
  `GUID` int UNSIGNED NOT NULL COMMENT 'item guid low',
  `increase` int UNSIGNED NOT NULL COMMENT 'stat_type',
  `decrease` int UNSIGNED NOT NULL COMMENT 'stat_type',
  `stat_value` int NOT NULL DEFAULT 0 COMMENT 'stat change',
  `Owner` int UNSIGNED NULL DEFAULT NULL COMMENT 'player guid',
  PRIMARY KEY (`GUID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of custom_reforging
-- ----------------------------
INSERT INTO `custom_reforging` VALUES (3668919, 6, 32, 100, 60);

-- ----------------------------
-- Table structure for custom_transmogrification
-- ----------------------------
DROP TABLE IF EXISTS `custom_transmogrification`;
CREATE TABLE `custom_transmogrification`  (
  `GUID` int UNSIGNED NOT NULL COMMENT 'Item guidLow',
  `FakeEntry` int UNSIGNED NOT NULL COMMENT 'Item entry',
  `Owner` int UNSIGNED NOT NULL COMMENT 'Player guidLow',
  PRIMARY KEY (`GUID`) USING BTREE,
  INDEX `Owner`(`Owner` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = '6_2' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of custom_transmogrification
-- ----------------------------
INSERT INTO `custom_transmogrification` VALUES (3931533, 16952, 122);
INSERT INTO `custom_transmogrification` VALUES (3931596, 16958, 122);
INSERT INTO `custom_transmogrification` VALUES (3931597, 49323, 122);
INSERT INTO `custom_transmogrification` VALUES (3931598, 16953, 122);
INSERT INTO `custom_transmogrification` VALUES (3931600, 16954, 122);

-- ----------------------------
-- Table structure for custom_transmogrification_sets
-- ----------------------------
DROP TABLE IF EXISTS `custom_transmogrification_sets`;
CREATE TABLE `custom_transmogrification_sets`  (
  `Owner` int UNSIGNED NOT NULL COMMENT 'Player guidlow',
  `PresetID` tinyint UNSIGNED NOT NULL COMMENT 'Preset identifier',
  `SetName` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL COMMENT 'SetName',
  `SetData` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL COMMENT 'Slot1 Entry1 Slot2 Entry2',
  PRIMARY KEY (`Owner`, `PresetID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = '6_1' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of custom_transmogrification_sets
-- ----------------------------
INSERT INTO `custom_transmogrification_sets` VALUES (13, 0, 'df', '0 51306 ');
INSERT INTO `custom_transmogrification_sets` VALUES (16, 0, 'kadavr', '0 31056 2 31059 4 31057 6 31058 9 31055 15 54806 ');

-- ----------------------------
-- Table structure for game_event_condition_save
-- ----------------------------
DROP TABLE IF EXISTS `game_event_condition_save`;
CREATE TABLE `game_event_condition_save`  (
  `eventEntry` tinyint UNSIGNED NOT NULL,
  `condition_id` int UNSIGNED NOT NULL DEFAULT 0,
  `done` float NULL DEFAULT 0,
  PRIMARY KEY (`eventEntry`, `condition_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of game_event_condition_save
-- ----------------------------

-- ----------------------------
-- Table structure for game_event_save
-- ----------------------------
DROP TABLE IF EXISTS `game_event_save`;
CREATE TABLE `game_event_save`  (
  `eventEntry` tinyint UNSIGNED NOT NULL,
  `state` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `next_start` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`eventEntry`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of game_event_save
-- ----------------------------

-- ----------------------------
-- Table structure for gm_subsurvey
-- ----------------------------
DROP TABLE IF EXISTS `gm_subsurvey`;
CREATE TABLE `gm_subsurvey`  (
  `surveyId` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `questionId` int UNSIGNED NOT NULL DEFAULT 0,
  `answer` int UNSIGNED NOT NULL DEFAULT 0,
  `answerComment` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  PRIMARY KEY (`surveyId`, `questionId`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of gm_subsurvey
-- ----------------------------

-- ----------------------------
-- Table structure for gm_survey
-- ----------------------------
DROP TABLE IF EXISTS `gm_survey`;
CREATE TABLE `gm_survey`  (
  `surveyId` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `mainSurvey` int UNSIGNED NOT NULL DEFAULT 0,
  `comment` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `createTime` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`surveyId`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of gm_survey
-- ----------------------------

-- ----------------------------
-- Table structure for gm_ticket
-- ----------------------------
DROP TABLE IF EXISTS `gm_ticket`;
CREATE TABLE `gm_ticket`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `type` tinyint UNSIGNED NOT NULL DEFAULT 0 COMMENT '0 open, 1 closed, 2 character deleted',
  `playerGuid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier of ticket creator',
  `name` varchar(12) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL COMMENT 'Name of ticket creator',
  `description` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `createTime` int UNSIGNED NOT NULL DEFAULT 0,
  `mapId` smallint UNSIGNED NOT NULL DEFAULT 0,
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  `lastModifiedTime` int UNSIGNED NOT NULL DEFAULT 0,
  `closedBy` int NOT NULL DEFAULT 0,
  `assignedTo` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'GUID of admin to whom ticket is assigned',
  `comment` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `response` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `completed` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `escalated` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `viewed` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `needMoreHelp` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `resolvedBy` int NOT NULL DEFAULT 0 COMMENT 'GUID of GM who resolved the ticket',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of gm_ticket
-- ----------------------------

-- ----------------------------
-- Table structure for group_instance
-- ----------------------------
DROP TABLE IF EXISTS `group_instance`;
CREATE TABLE `group_instance`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `instance` int UNSIGNED NOT NULL DEFAULT 0,
  `permanent` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `instance`) USING BTREE,
  INDEX `instance`(`instance` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of group_instance
-- ----------------------------

-- ----------------------------
-- Table structure for group_member
-- ----------------------------
DROP TABLE IF EXISTS `group_member`;
CREATE TABLE `group_member`  (
  `guid` int UNSIGNED NOT NULL,
  `memberGuid` int UNSIGNED NOT NULL,
  `memberFlags` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `subgroup` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `roles` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`memberGuid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Groups' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of group_member
-- ----------------------------

-- ----------------------------
-- Table structure for groups
-- ----------------------------
DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups`  (
  `guid` int UNSIGNED NOT NULL,
  `leaderGuid` int UNSIGNED NOT NULL,
  `lootMethod` tinyint UNSIGNED NOT NULL,
  `looterGuid` int UNSIGNED NOT NULL,
  `lootThreshold` tinyint UNSIGNED NOT NULL,
  `icon1` bigint UNSIGNED NOT NULL,
  `icon2` bigint UNSIGNED NOT NULL,
  `icon3` bigint UNSIGNED NOT NULL,
  `icon4` bigint UNSIGNED NOT NULL,
  `icon5` bigint UNSIGNED NOT NULL,
  `icon6` bigint UNSIGNED NOT NULL,
  `icon7` bigint UNSIGNED NOT NULL,
  `icon8` bigint UNSIGNED NOT NULL,
  `groupType` tinyint UNSIGNED NOT NULL,
  `difficulty` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `raidDifficulty` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `masterLooterGuid` int UNSIGNED NOT NULL,
  PRIMARY KEY (`guid`) USING BTREE,
  INDEX `leaderGuid`(`leaderGuid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Groups' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of groups
-- ----------------------------

-- ----------------------------
-- Table structure for guild
-- ----------------------------
DROP TABLE IF EXISTS `guild`;
CREATE TABLE `guild`  (
  `guildid` int UNSIGNED NOT NULL DEFAULT 0,
  `name` varchar(24) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `leaderguid` int UNSIGNED NOT NULL DEFAULT 0,
  `EmblemStyle` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `EmblemColor` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `BorderStyle` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `BorderColor` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `BackgroundColor` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `info` varchar(500) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `motd` varchar(128) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `createdate` int UNSIGNED NOT NULL DEFAULT 0,
  `BankMoney` bigint UNSIGNED NOT NULL DEFAULT 0,
  `xp` int NOT NULL DEFAULT 0,
  `level` tinyint NOT NULL DEFAULT 0,
  PRIMARY KEY (`guildid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Guild System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild
-- ----------------------------

-- ----------------------------
-- Table structure for guild_bank_eventlog
-- ----------------------------
DROP TABLE IF EXISTS `guild_bank_eventlog`;
CREATE TABLE `guild_bank_eventlog`  (
  `guildid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Guild Identificator',
  `LogGuid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Log record identificator - auxiliary column',
  `TabId` tinyint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Guild bank TabId',
  `EventType` tinyint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Event type',
  `PlayerGuid` int UNSIGNED NOT NULL DEFAULT 0,
  `ItemOrMoney` int UNSIGNED NOT NULL DEFAULT 0,
  `ItemStackCount` smallint UNSIGNED NOT NULL DEFAULT 0,
  `DestTabId` tinyint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Destination Tab Id',
  `TimeStamp` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Event UNIX time',
  PRIMARY KEY (`guildid`, `LogGuid`, `TabId`) USING BTREE,
  INDEX `guildid_key`(`guildid` ASC) USING BTREE,
  INDEX `Idx_PlayerGuid`(`PlayerGuid` ASC) USING BTREE,
  INDEX `Idx_LogGuid`(`LogGuid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_bank_eventlog
-- ----------------------------

-- ----------------------------
-- Table structure for guild_bank_item
-- ----------------------------
DROP TABLE IF EXISTS `guild_bank_item`;
CREATE TABLE `guild_bank_item`  (
  `guildid` int UNSIGNED NOT NULL DEFAULT 0,
  `TabId` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `SlotId` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `item_guid` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guildid`, `TabId`, `SlotId`) USING BTREE,
  INDEX `guildid_key`(`guildid` ASC) USING BTREE,
  INDEX `Idx_item_guid`(`item_guid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_bank_item
-- ----------------------------

-- ----------------------------
-- Table structure for guild_bank_right
-- ----------------------------
DROP TABLE IF EXISTS `guild_bank_right`;
CREATE TABLE `guild_bank_right`  (
  `guildid` int UNSIGNED NOT NULL DEFAULT 0,
  `TabId` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `rid` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `gbright` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `SlotPerDay` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guildid`, `TabId`, `rid`) USING BTREE,
  INDEX `guildid_key`(`guildid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_bank_right
-- ----------------------------

-- ----------------------------
-- Table structure for guild_bank_tab
-- ----------------------------
DROP TABLE IF EXISTS `guild_bank_tab`;
CREATE TABLE `guild_bank_tab`  (
  `guildid` int UNSIGNED NOT NULL DEFAULT 0,
  `TabId` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `TabName` varchar(16) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `TabIcon` varchar(100) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `TabText` varchar(500) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`guildid`, `TabId`) USING BTREE,
  INDEX `guildid_key`(`guildid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_bank_tab
-- ----------------------------

-- ----------------------------
-- Table structure for guild_bonus_config
-- ----------------------------
DROP TABLE IF EXISTS `guild_bonus_config`;
CREATE TABLE `guild_bonus_config`  (
  `BonusId` smallint NOT NULL DEFAULT 0,
  `RequiredGuildLevel` tinyint NOT NULL DEFAULT 0,
  `comment` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_bonus_config
-- ----------------------------

-- ----------------------------
-- Table structure for guild_eventlog
-- ----------------------------
DROP TABLE IF EXISTS `guild_eventlog`;
CREATE TABLE `guild_eventlog`  (
  `guildid` int UNSIGNED NOT NULL COMMENT 'Guild Identificator',
  `LogGuid` int UNSIGNED NOT NULL COMMENT 'Log record identificator - auxiliary column',
  `EventType` tinyint UNSIGNED NOT NULL COMMENT 'Event type',
  `PlayerGuid1` int UNSIGNED NOT NULL COMMENT 'Player 1',
  `PlayerGuid2` int UNSIGNED NOT NULL COMMENT 'Player 2',
  `NewRank` tinyint UNSIGNED NOT NULL COMMENT 'New rank(in case promotion/demotion)',
  `TimeStamp` int UNSIGNED NOT NULL COMMENT 'Event UNIX time',
  PRIMARY KEY (`guildid`, `LogGuid`) USING BTREE,
  INDEX `Idx_PlayerGuid1`(`PlayerGuid1` ASC) USING BTREE,
  INDEX `Idx_PlayerGuid2`(`PlayerGuid2` ASC) USING BTREE,
  INDEX `Idx_LogGuid`(`LogGuid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Guild Eventlog' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_eventlog
-- ----------------------------

-- ----------------------------
-- Table structure for guild_member
-- ----------------------------
DROP TABLE IF EXISTS `guild_member`;
CREATE TABLE `guild_member`  (
  `guildid` int UNSIGNED NOT NULL COMMENT 'Guild Identificator',
  `guid` int UNSIGNED NOT NULL,
  `rank` tinyint UNSIGNED NOT NULL,
  `pnote` varchar(31) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `offnote` varchar(31) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  UNIQUE INDEX `guid_key`(`guid` ASC) USING BTREE,
  INDEX `guildid_key`(`guildid` ASC) USING BTREE,
  INDEX `guildid_rank_key`(`guildid` ASC, `rank` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Guild System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_member
-- ----------------------------

-- ----------------------------
-- Table structure for guild_member_withdraw
-- ----------------------------
DROP TABLE IF EXISTS `guild_member_withdraw`;
CREATE TABLE `guild_member_withdraw`  (
  `guid` int UNSIGNED NOT NULL,
  `tab0` int UNSIGNED NOT NULL DEFAULT 0,
  `tab1` int UNSIGNED NOT NULL DEFAULT 0,
  `tab2` int UNSIGNED NOT NULL DEFAULT 0,
  `tab3` int UNSIGNED NOT NULL DEFAULT 0,
  `tab4` int UNSIGNED NOT NULL DEFAULT 0,
  `tab5` int UNSIGNED NOT NULL DEFAULT 0,
  `money` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Guild Member Daily Withdraws' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_member_withdraw
-- ----------------------------

-- ----------------------------
-- Table structure for guild_rank
-- ----------------------------
DROP TABLE IF EXISTS `guild_rank`;
CREATE TABLE `guild_rank`  (
  `guildid` int UNSIGNED NOT NULL DEFAULT 0,
  `rid` tinyint UNSIGNED NOT NULL,
  `rname` varchar(20) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  `rights` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `BankMoneyPerDay` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guildid`, `rid`) USING BTREE,
  INDEX `Idx_rid`(`rid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Guild System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_rank
-- ----------------------------

-- ----------------------------
-- Table structure for guild_xp_for_next_level
-- ----------------------------
DROP TABLE IF EXISTS `guild_xp_for_next_level`;
CREATE TABLE `guild_xp_for_next_level`  (
  `level` tinyint NOT NULL DEFAULT 0,
  `xp_for_next_level` int NOT NULL DEFAULT 0,
  PRIMARY KEY (`level`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of guild_xp_for_next_level
-- ----------------------------

-- ----------------------------
-- Table structure for hardcore_deaths
-- ----------------------------
DROP TABLE IF EXISTS `hardcore_deaths`;
CREATE TABLE `hardcore_deaths`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `char_guid` int NOT NULL,
  `char_name` varchar(12) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `level` tinyint UNSIGNED NOT NULL,
  `playtime` int UNSIGNED NOT NULL,
  `killer_name` varchar(50) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `death_time` bigint UNSIGNED NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hardcore_deaths
-- ----------------------------
INSERT INTO `hardcore_deaths` VALUES (1, 141, 'Допермуня', 1, 3206, 'Dunemaul Enforcer', 1748570841);
INSERT INTO `hardcore_deaths` VALUES (2, 160, 'Id', 1, 102, 'Diseased Young Wolf', 1748570986);

-- ----------------------------
-- Table structure for hardcore_extra_lives
-- ----------------------------
DROP TABLE IF EXISTS `hardcore_extra_lives`;
CREATE TABLE `hardcore_extra_lives`  (
  `player_guid` int UNSIGNED NOT NULL,
  `has_extra_life` tinyint(1) NOT NULL DEFAULT 0,
  PRIMARY KEY (`player_guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hardcore_extra_lives
-- ----------------------------

-- ----------------------------
-- Table structure for hardcore_level_rewards
-- ----------------------------
DROP TABLE IF EXISTS `hardcore_level_rewards`;
CREATE TABLE `hardcore_level_rewards`  (
  `level` int NOT NULL,
  `item_id` int NULL DEFAULT NULL COMMENT 'ID предмета награды',
  `item_count` int NULL DEFAULT 1 COMMENT 'Количество предметов',
  `money` int NULL DEFAULT 0 COMMENT 'Количество денег',
  `title_id` int NULL DEFAULT NULL COMMENT 'ID титула',
  `achievement_id` int NULL DEFAULT NULL COMMENT 'ID достижения',
  `spell_id` int NULL DEFAULT NULL COMMENT 'ID заклинания',
  PRIMARY KEY (`level`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci COMMENT = 'Награды за уровни в Hardcore-режиме' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hardcore_level_rewards
-- ----------------------------
INSERT INTO `hardcore_level_rewards` VALUES (10, 29434, 1, 100000, NULL, NULL, NULL);
INSERT INTO `hardcore_level_rewards` VALUES (20, 35279, 1, 200000, NULL, NULL, NULL);
INSERT INTO `hardcore_level_rewards` VALUES (30, 38186, 1, 300000, NULL, NULL, NULL);
INSERT INTO `hardcore_level_rewards` VALUES (40, 40075, 1, 400000, NULL, NULL, NULL);
INSERT INTO `hardcore_level_rewards` VALUES (50, 43246, 1, 500000, 1, NULL, NULL);
INSERT INTO `hardcore_level_rewards` VALUES (60, 45085, 1, 1000000, 2, 1, 53142);
INSERT INTO `hardcore_level_rewards` VALUES (70, 49426, 1, 2000000, 3, 2, 64488);
INSERT INTO `hardcore_level_rewards` VALUES (80, 50255, 1, 5000000, 4, 3, 74452);

-- ----------------------------
-- Table structure for hardcore_rewards_claimed
-- ----------------------------
DROP TABLE IF EXISTS `hardcore_rewards_claimed`;
CREATE TABLE `hardcore_rewards_claimed`  (
  `guid` int UNSIGNED NOT NULL,
  `reward_id` int UNSIGNED NOT NULL,
  `claimed_time` int UNSIGNED NOT NULL,
  `playtime_when_claimed` int UNSIGNED NOT NULL,
  PRIMARY KEY (`guid`, `reward_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hardcore_rewards_claimed
-- ----------------------------

-- ----------------------------
-- Table structure for hardcore_rewards_received
-- ----------------------------
DROP TABLE IF EXISTS `hardcore_rewards_received`;
CREATE TABLE `hardcore_rewards_received`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `player_guid` int NOT NULL,
  `level` int NOT NULL,
  `received_at` int NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `player_level`(`player_guid` ASC, `level` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci COMMENT = 'Полученные награды за уровни' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hardcore_rewards_received
-- ----------------------------

-- ----------------------------
-- Table structure for hardcore_time_rewards
-- ----------------------------
DROP TABLE IF EXISTS `hardcore_time_rewards`;
CREATE TABLE `hardcore_time_rewards`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `required_time` int UNSIGNED NOT NULL COMMENT 'Время в секундах',
  `item_id` int UNSIGNED NULL DEFAULT NULL,
  `item_count` int UNSIGNED NULL DEFAULT 1,
  `money` int UNSIGNED NULL DEFAULT 0,
  `title_id` int UNSIGNED NULL DEFAULT NULL,
  `achievement_id` int UNSIGNED NULL DEFAULT NULL,
  `spell_id` int UNSIGNED NULL DEFAULT NULL,
  `name` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `experience` int NULL DEFAULT NULL,
  `description` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `enabled` tinyint(1) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 7 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of hardcore_time_rewards
-- ----------------------------
INSERT INTO `hardcore_time_rewards` VALUES (1, 1500, 6948, 1, 0, NULL, NULL, NULL, 'Камень возвращения', NULL, 'Награда', 1);
INSERT INTO `hardcore_time_rewards` VALUES (2, 7200, 0, 0, 100000, NULL, NULL, NULL, 'Золотая награда', NULL, '1 золото', 1);
INSERT INTO `hardcore_time_rewards` VALUES (3, 6666, 25, 1, 0, NULL, NULL, NULL, 'Меч правосудия', NULL, 'Особое оружие', 1);
INSERT INTO `hardcore_time_rewards` VALUES (4, 6060, 25, 0, 500000, NULL, NULL, NULL, 'Большая награда', NULL, '5 золота', 1);
INSERT INTO `hardcore_time_rewards` VALUES (5, 3000, 19019, 1, 0, NULL, NULL, NULL, 'Громовая ярость', NULL, 'Легендарное оружие', 1);
INSERT INTO `hardcore_time_rewards` VALUES (6, 66, 0, 0, 0, NULL, NULL, NULL, 'ГРОМПЫХ', 60000, NULL, 1);

-- ----------------------------
-- Table structure for instance
-- ----------------------------
DROP TABLE IF EXISTS `instance`;
CREATE TABLE `instance`  (
  `id` int UNSIGNED NOT NULL DEFAULT 0,
  `map` smallint UNSIGNED NOT NULL DEFAULT 0,
  `resettime` bigint UNSIGNED NOT NULL DEFAULT 0,
  `difficulty` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `completedEncounters` int UNSIGNED NOT NULL DEFAULT 0,
  `data` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `map`(`map` ASC) USING BTREE,
  INDEX `resettime`(`resettime` ASC) USING BTREE,
  INDEX `difficulty`(`difficulty` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of instance
-- ----------------------------

-- ----------------------------
-- Table structure for instance_reset
-- ----------------------------
DROP TABLE IF EXISTS `instance_reset`;
CREATE TABLE `instance_reset`  (
  `mapid` smallint UNSIGNED NOT NULL DEFAULT 0,
  `difficulty` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `resettime` bigint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`mapid`, `difficulty`) USING BTREE,
  INDEX `difficulty`(`difficulty` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of instance_reset
-- ----------------------------

-- ----------------------------
-- Table structure for item_instance
-- ----------------------------
DROP TABLE IF EXISTS `item_instance`;
CREATE TABLE `item_instance`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `itemEntry` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `owner_guid` int UNSIGNED NOT NULL DEFAULT 0,
  `creatorGuid` int UNSIGNED NOT NULL DEFAULT 0,
  `giftCreatorGuid` int UNSIGNED NOT NULL DEFAULT 0,
  `count` int UNSIGNED NOT NULL DEFAULT 1,
  `duration` int NOT NULL DEFAULT 0,
  `charges` tinytext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `flags` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `enchantments` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `randomPropertyId` smallint NOT NULL DEFAULT 0,
  `durability` smallint UNSIGNED NOT NULL DEFAULT 0,
  `playedTime` int UNSIGNED NOT NULL DEFAULT 0,
  `text` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `transmog` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE,
  INDEX `idx_owner_guid`(`owner_guid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Item System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_instance
-- ----------------------------

-- ----------------------------
-- Table structure for item_loot_items
-- ----------------------------
DROP TABLE IF EXISTS `item_loot_items`;
CREATE TABLE `item_loot_items`  (
  `container_id` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'guid of container (item_instance.guid)',
  `item_id` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'loot item entry (item_instance.itemEntry)',
  `item_count` int NOT NULL DEFAULT 0 COMMENT 'stack size',
  `item_index` int UNSIGNED NOT NULL DEFAULT 0,
  `follow_rules` tinyint(1) NOT NULL DEFAULT 0 COMMENT 'follow loot rules',
  `ffa` tinyint(1) NOT NULL DEFAULT 0 COMMENT 'free-for-all',
  `blocked` tinyint(1) NOT NULL DEFAULT 0,
  `counted` tinyint(1) NOT NULL DEFAULT 0,
  `under_threshold` tinyint(1) NOT NULL DEFAULT 0,
  `needs_quest` tinyint(1) NOT NULL DEFAULT 0 COMMENT 'quest drop',
  `rnd_prop` int NOT NULL DEFAULT 0 COMMENT 'random enchantment added when originally rolled',
  `rnd_suffix` int NOT NULL DEFAULT 0 COMMENT 'random suffix added when originally rolled'
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_loot_items
-- ----------------------------

-- ----------------------------
-- Table structure for item_loot_money
-- ----------------------------
DROP TABLE IF EXISTS `item_loot_money`;
CREATE TABLE `item_loot_money`  (
  `container_id` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'guid of container (item_instance.guid)',
  `money` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'money loot (in copper)',
  PRIMARY KEY (`container_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_loot_money
-- ----------------------------

-- ----------------------------
-- Table structure for item_refund_instance
-- ----------------------------
DROP TABLE IF EXISTS `item_refund_instance`;
CREATE TABLE `item_refund_instance`  (
  `item_guid` int UNSIGNED NOT NULL COMMENT 'Item GUID',
  `player_guid` int UNSIGNED NOT NULL COMMENT 'Player GUID',
  `paidMoney` int UNSIGNED NOT NULL DEFAULT 0,
  `paidExtendedCost` smallint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`item_guid`, `player_guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Item Refund System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_refund_instance
-- ----------------------------

-- ----------------------------
-- Table structure for item_soulbound_trade_data
-- ----------------------------
DROP TABLE IF EXISTS `item_soulbound_trade_data`;
CREATE TABLE `item_soulbound_trade_data`  (
  `itemGuid` int UNSIGNED NOT NULL COMMENT 'Item GUID',
  `allowedPlayers` text CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL COMMENT 'Space separated GUID list of players who can receive this item in trade',
  PRIMARY KEY (`itemGuid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Item Refund System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of item_soulbound_trade_data
-- ----------------------------

-- ----------------------------
-- Table structure for lag_reports
-- ----------------------------
DROP TABLE IF EXISTS `lag_reports`;
CREATE TABLE `lag_reports`  (
  `reportId` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `guid` int UNSIGNED NOT NULL DEFAULT 0,
  `lagType` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `mapId` smallint UNSIGNED NOT NULL DEFAULT 0,
  `posX` float NOT NULL DEFAULT 0,
  `posY` float NOT NULL DEFAULT 0,
  `posZ` float NOT NULL DEFAULT 0,
  `latency` int UNSIGNED NOT NULL DEFAULT 0,
  `createTime` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`reportId`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of lag_reports
-- ----------------------------

-- ----------------------------
-- Table structure for lfg_data
-- ----------------------------
DROP TABLE IF EXISTS `lfg_data`;
CREATE TABLE `lfg_data`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `dungeon` int UNSIGNED NOT NULL DEFAULT 0,
  `state` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'LFG Data' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of lfg_data
-- ----------------------------

-- ----------------------------
-- Table structure for mail
-- ----------------------------
DROP TABLE IF EXISTS `mail`;
CREATE TABLE `mail`  (
  `id` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Identifier',
  `messageType` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `stationery` tinyint NOT NULL DEFAULT 41,
  `mailTemplateId` smallint UNSIGNED NOT NULL DEFAULT 0,
  `sender` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  `receiver` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  `subject` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `body` longtext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  `has_items` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `expire_time` int UNSIGNED NOT NULL DEFAULT 0,
  `deliver_time` int UNSIGNED NOT NULL DEFAULT 0,
  `money` int UNSIGNED NOT NULL DEFAULT 0,
  `cod` int UNSIGNED NOT NULL DEFAULT 0,
  `checked` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `idx_receiver`(`receiver` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Mail System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mail
-- ----------------------------

-- ----------------------------
-- Table structure for mail_external
-- ----------------------------
DROP TABLE IF EXISTS `mail_external`;
CREATE TABLE `mail_external`  (
  `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT,
  `receiver` bigint UNSIGNED NOT NULL,
  `subject` varchar(200) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL DEFAULT 'Support Message',
  `message` varchar(500) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL DEFAULT 'Support Message',
  `money` bigint UNSIGNED NOT NULL DEFAULT 0,
  `item` bigint UNSIGNED NOT NULL DEFAULT 0,
  `item_count` bigint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 7525 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mail_external
-- ----------------------------

-- ----------------------------
-- Table structure for mail_items
-- ----------------------------
DROP TABLE IF EXISTS `mail_items`;
CREATE TABLE `mail_items`  (
  `mail_id` int UNSIGNED NOT NULL DEFAULT 0,
  `item_guid` int UNSIGNED NOT NULL DEFAULT 0,
  `receiver` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Character Global Unique Identifier',
  PRIMARY KEY (`item_guid`) USING BTREE,
  INDEX `idx_receiver`(`receiver` ASC) USING BTREE,
  INDEX `idx_mail_id`(`mail_id` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mail_items
-- ----------------------------

-- ----------------------------
-- Table structure for mail_server_character
-- ----------------------------
DROP TABLE IF EXISTS `mail_server_character`;
CREATE TABLE `mail_server_character`  (
  `guid` int UNSIGNED NOT NULL,
  `mailId` int UNSIGNED NOT NULL,
  PRIMARY KEY (`guid`, `mailId`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mail_server_character
-- ----------------------------

-- ----------------------------
-- Table structure for mail_server_template
-- ----------------------------
DROP TABLE IF EXISTS `mail_server_template`;
CREATE TABLE `mail_server_template`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `reqLevel` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `reqPlayTime` int UNSIGNED NOT NULL DEFAULT 0,
  `moneyA` int UNSIGNED NOT NULL DEFAULT 0,
  `moneyH` int UNSIGNED NOT NULL DEFAULT 0,
  `itemA` int UNSIGNED NOT NULL DEFAULT 0,
  `itemCountA` int UNSIGNED NOT NULL DEFAULT 0,
  `itemH` int UNSIGNED NOT NULL DEFAULT 0,
  `itemCountH` int UNSIGNED NOT NULL DEFAULT 0,
  `subject` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `body` text CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci NOT NULL,
  `active` tinyint UNSIGNED NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mail_server_template
-- ----------------------------

-- ----------------------------
-- Table structure for mod_item_upgrade_stats
-- ----------------------------
DROP TABLE IF EXISTS `mod_item_upgrade_stats`;
CREATE TABLE `mod_item_upgrade_stats`  (
  `id` int UNSIGNED NOT NULL,
  `stat_type` tinyint UNSIGNED NOT NULL,
  `stat_mod_pct` float NOT NULL,
  `stat_rank` smallint UNSIGNED NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  UNIQUE INDEX `idx_stat`(`stat_type` ASC, `stat_mod_pct` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of mod_item_upgrade_stats
-- ----------------------------

-- ----------------------------
-- Table structure for pet_aura
-- ----------------------------
DROP TABLE IF EXISTS `pet_aura`;
CREATE TABLE `pet_aura`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `casterGuid` bigint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Full Global Unique Identifier',
  `spell` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `effectMask` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `recalculateMask` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `stackCount` tinyint UNSIGNED NOT NULL DEFAULT 1,
  `amount0` mediumint NOT NULL,
  `amount1` mediumint NOT NULL,
  `amount2` mediumint NOT NULL,
  `base_amount0` mediumint NOT NULL,
  `base_amount1` mediumint NOT NULL,
  `base_amount2` mediumint NOT NULL,
  `maxDuration` int NOT NULL DEFAULT 0,
  `remainTime` int NOT NULL DEFAULT 0,
  `remainCharges` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `critChance` float NOT NULL DEFAULT 0,
  `applyResilience` tinyint NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `casterGuid`, `spell`, `effectMask`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Pet System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pet_aura
-- ----------------------------

-- ----------------------------
-- Table structure for pet_spell
-- ----------------------------
DROP TABLE IF EXISTS `pet_spell`;
CREATE TABLE `pet_spell`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier',
  `spell` mediumint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Spell Identifier',
  `active` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `spell`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Pet System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pet_spell
-- ----------------------------

-- ----------------------------
-- Table structure for pet_spell_cooldown
-- ----------------------------
DROP TABLE IF EXISTS `pet_spell_cooldown`;
CREATE TABLE `pet_spell_cooldown`  (
  `guid` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Global Unique Identifier, Low part',
  `spell` mediumint UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Spell Identifier',
  `time` int UNSIGNED NOT NULL DEFAULT 0,
  `categoryId` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Spell category Id',
  `categoryEnd` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`guid`, `spell`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pet_spell_cooldown
-- ----------------------------

-- ----------------------------
-- Table structure for petition
-- ----------------------------
DROP TABLE IF EXISTS `petition`;
CREATE TABLE `petition`  (
  `ownerguid` int UNSIGNED NOT NULL,
  `petitionguid` int UNSIGNED NULL DEFAULT 0,
  `name` varchar(24) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `type` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`ownerguid`, `type`) USING BTREE,
  UNIQUE INDEX `index_ownerguid_petitionguid`(`ownerguid` ASC, `petitionguid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Guild System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of petition
-- ----------------------------

-- ----------------------------
-- Table structure for petition_sign
-- ----------------------------
DROP TABLE IF EXISTS `petition_sign`;
CREATE TABLE `petition_sign`  (
  `ownerguid` int UNSIGNED NOT NULL,
  `petitionguid` int UNSIGNED NOT NULL DEFAULT 0,
  `playerguid` int UNSIGNED NOT NULL DEFAULT 0,
  `player_account` int UNSIGNED NOT NULL DEFAULT 0,
  `type` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`petitionguid`, `playerguid`) USING BTREE,
  INDEX `Idx_playerguid`(`playerguid` ASC) USING BTREE,
  INDEX `Idx_ownerguid`(`ownerguid` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Guild System' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of petition_sign
-- ----------------------------

-- ----------------------------
-- Table structure for pool_quest_save
-- ----------------------------
DROP TABLE IF EXISTS `pool_quest_save`;
CREATE TABLE `pool_quest_save`  (
  `pool_id` int UNSIGNED NOT NULL DEFAULT 0,
  `quest_id` int UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`pool_id`, `quest_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pool_quest_save
-- ----------------------------

-- ----------------------------
-- Table structure for pvpstats_battlegrounds
-- ----------------------------
DROP TABLE IF EXISTS `pvpstats_battlegrounds`;
CREATE TABLE `pvpstats_battlegrounds`  (
  `id` bigint UNSIGNED NOT NULL AUTO_INCREMENT,
  `winner_faction` tinyint NOT NULL,
  `bracket_id` tinyint UNSIGNED NOT NULL,
  `type` tinyint UNSIGNED NOT NULL,
  `date` datetime NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pvpstats_battlegrounds
-- ----------------------------

-- ----------------------------
-- Table structure for pvpstats_players
-- ----------------------------
DROP TABLE IF EXISTS `pvpstats_players`;
CREATE TABLE `pvpstats_players`  (
  `battleground_id` bigint UNSIGNED NOT NULL,
  `character_guid` int UNSIGNED NOT NULL,
  `winner` bit(1) NOT NULL,
  `score_killing_blows` mediumint UNSIGNED NOT NULL,
  `score_deaths` mediumint UNSIGNED NOT NULL,
  `score_honorable_kills` mediumint UNSIGNED NOT NULL,
  `score_bonus_honor` mediumint UNSIGNED NOT NULL,
  `score_damage_done` mediumint UNSIGNED NOT NULL,
  `score_healing_done` mediumint UNSIGNED NOT NULL,
  `attr_1` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `attr_2` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `attr_3` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `attr_4` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `attr_5` mediumint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`battleground_id`, `character_guid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of pvpstats_players
-- ----------------------------

-- ----------------------------
-- Table structure for quest_tracker
-- ----------------------------
DROP TABLE IF EXISTS `quest_tracker`;
CREATE TABLE `quest_tracker`  (
  `id` mediumint UNSIGNED NOT NULL DEFAULT 0,
  `character_guid` int UNSIGNED NOT NULL DEFAULT 0,
  `quest_accept_time` datetime NOT NULL,
  `quest_complete_time` datetime NULL DEFAULT NULL,
  `quest_abandon_time` datetime NULL DEFAULT NULL,
  `completed_by_gm` tinyint(1) NOT NULL DEFAULT 0,
  `core_hash` varchar(120) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '0',
  `core_revision` varchar(120) CHARACTER SET latin1 COLLATE latin1_swedish_ci NOT NULL DEFAULT '0'
) ENGINE = InnoDB CHARACTER SET = latin1 COLLATE = latin1_swedish_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of quest_tracker
-- ----------------------------

-- ----------------------------
-- Table structure for reserved_name
-- ----------------------------
DROP TABLE IF EXISTS `reserved_name`;
CREATE TABLE `reserved_name`  (
  `name` varchar(12) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`name`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Player Reserved Names' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of reserved_name
-- ----------------------------

-- ----------------------------
-- Table structure for respawn
-- ----------------------------
DROP TABLE IF EXISTS `respawn`;
CREATE TABLE `respawn`  (
  `type` smallint UNSIGNED NOT NULL,
  `spawnId` int UNSIGNED NOT NULL,
  `respawnTime` bigint UNSIGNED NOT NULL,
  `mapId` smallint UNSIGNED NOT NULL,
  `instanceId` int UNSIGNED NOT NULL,
  PRIMARY KEY (`type`, `spawnId`, `instanceId`) USING BTREE,
  INDEX `idx_instance`(`instanceId` ASC) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Stored respawn times' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of respawn
-- ----------------------------

-- ----------------------------
-- Table structure for updates
-- ----------------------------
DROP TABLE IF EXISTS `updates`;
CREATE TABLE `updates`  (
  `name` varchar(200) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL COMMENT 'filename with extension of the update.',
  `hash` char(40) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL DEFAULT '' COMMENT 'sha1 hash of the sql file.',
  `state` enum('RELEASED','ARCHIVED') CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT 'RELEASED' COMMENT 'defines if an update is released or archived.',
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'timestamp when the query was applied.',
  `speed` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'time the query takes to apply in ms.',
  PRIMARY KEY (`name`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'List of all applied updates in this database.' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of updates
-- ----------------------------
INSERT INTO `updates` VALUES ('2015_03_20_00_characters.sql', 'B761760804EA73BD297F296C5C1919687DF7191C', 'ARCHIVED', '2015-03-22 00:44:15', 0);
INSERT INTO `updates` VALUES ('2015_03_20_01_characters.sql', '894F08B70449A5481FFAF394EE5571D7FC4D8A3A', 'ARCHIVED', '2015-03-22 00:44:15', 0);
INSERT INTO `updates` VALUES ('2015_03_20_02_characters.sql', '97D7BE0CAADC79F3F11B9FD296B8C6CD40FE593B', 'ARCHIVED', '2015-03-22 00:44:51', 0);
INSERT INTO `updates` VALUES ('2015_06_26_00_characters_335.sql', 'C2CC6E50AFA1ACCBEBF77CC519AAEB09F3BBAEBC', 'ARCHIVED', '2015-07-14 02:49:22', 0);
INSERT INTO `updates` VALUES ('2015_09_28_00_characters_335.sql', 'F8682A431D50E54BDC4AC0E7DBED21AE8AAB6AD4', 'ARCHIVED', '2015-09-29 00:00:00', 0);
INSERT INTO `updates` VALUES ('2015_08_26_00_characters_335.sql', 'C7D6A3A00FECA3EBFF1E71744CA40D3076582374', 'ARCHIVED', '2015-08-27 00:00:00', 0);
INSERT INTO `updates` VALUES ('2015_10_06_00_characters.sql', '16842FDD7E8547F2260D3312F53EFF8761EFAB35', 'ARCHIVED', '2015-10-06 19:06:38', 0);
INSERT INTO `updates` VALUES ('2015_10_07_00_characters.sql', 'E15AB463CEBE321001D7BFDEA4B662FF618728FD', 'ARCHIVED', '2015-10-08 02:32:00', 0);
INSERT INTO `updates` VALUES ('2015_10_12_00_characters.sql', 'D6F9927BDED72AD0A81D6EC2C6500CBC34A39FA2', 'ARCHIVED', '2015-10-12 18:35:47', 0);
INSERT INTO `updates` VALUES ('2015_10_28_00_characters.sql', '622A9CA8FCE690429EBE23BA071A37C7A007BF8B', 'ARCHIVED', '2015-10-19 17:32:22', 0);
INSERT INTO `updates` VALUES ('2015_10_29_00_characters_335.sql', '4555A7F35C107E54C13D74D20F141039ED42943E', 'ARCHIVED', '2015-10-29 20:05:43', 0);
INSERT INTO `updates` VALUES ('2015_11_03_00_characters.sql', 'CC045717B8FDD9733351E52A5302560CD08AAD57', 'ARCHIVED', '2015-10-12 18:23:33', 0);
INSERT INTO `updates` VALUES ('2015_11_07_00_characters.sql', '0ACDD35EC9745231BCFA701B78056DEF94D0CC53', 'ARCHIVED', '2016-04-11 03:42:36', 0);
INSERT INTO `updates` VALUES ('2016_02_10_00_characters.sql', 'F1B4DA202819CABC7319A4470A2D224A34609E97', 'ARCHIVED', '2016-02-10 03:00:00', 0);
INSERT INTO `updates` VALUES ('2016_03_13_2016_01_05_00_characters.sql', '0EAD24977F40DE2476B4567DA2B477867CC0DA1A', 'ARCHIVED', '2016-03-13 23:03:56', 0);
INSERT INTO `updates` VALUES ('2016_04_11_00_characters.sql', '0ACDD35EC9745231BCFA701B78056DEF94D0CC53', 'ARCHIVED', '2016-04-11 06:18:17', 0);
INSERT INTO `updates` VALUES ('2016_09_13_00_characters.sql', '27A04615B11B2CFC3A26778F52F74C071E4F9C54', 'ARCHIVED', '2016-07-06 21:55:18', 0);
INSERT INTO `updates` VALUES ('2016_10_16_00_characters.sql', '0ACDD35EC9745231BCFA701B78056DEF94D0CC53', 'ARCHIVED', '2016-10-16 17:02:49', 0);
INSERT INTO `updates` VALUES ('2016_10_30_00_characters.sql', '7E2D5B226907B5A9AF320797F46E86DC27B7EC90', 'ARCHIVED', '2016-10-30 03:00:00', 0);
INSERT INTO `updates` VALUES ('2017_04_03_00_characters.sql', 'CB072C56692C9FBF170C4036F15773DD86D368B5', 'ARCHIVED', '2017-04-03 03:00:00', 0);
INSERT INTO `updates` VALUES ('2017_04_12_00_characters.sql', '4FE3C6866A6DCD4926D451F6009464D290C2EF1F', 'ARCHIVED', '2017-04-12 03:00:00', 0);
INSERT INTO `updates` VALUES ('2017_04_12_01_characters.sql', '5A8A1215E3A2356722F52CD7A64BBE03D21FBEA3', 'ARCHIVED', '2017-04-12 03:00:00', 0);
INSERT INTO `updates` VALUES ('2017_04_19_00_characters.sql', 'CE06FA9005C8A8EE4BDD925520278A5D83E87485', 'ARCHIVED', '2017-04-19 03:07:40', 0);
INSERT INTO `updates` VALUES ('2017_10_29_00_characters.sql', '8CFC473E7E87E58C317A72016BF69E9050D3BC83', 'ARCHIVED', '2017-04-19 03:07:40', 0);
INSERT INTO `updates` VALUES ('2017_11_27_00_characters.sql', '6FF1F84B8985ADFC7FF97F0BF8E53403CF13C320', 'ARCHIVED', '2017-11-28 01:08:42', 0);
INSERT INTO `updates` VALUES ('2018_01_13_00_characters.sql', 'E3C0DA9995BA71ED5A267294470CD03DC51862DD', 'ARCHIVED', '2018-01-13 03:00:00', 0);
INSERT INTO `updates` VALUES ('2018_02_19_00_characters.sql', 'FE5C5F9B88F0791549DDE680942493781E2269E6', 'ARCHIVED', '2018-02-18 22:49:38', 0);
INSERT INTO `updates` VALUES ('2018_04_24_00_characters.sql', '77264AB7BEF421C0A4BB81EEAFD0D8C1CBCA840F', 'ARCHIVED', '2018-04-20 12:38:10', 0);
INSERT INTO `updates` VALUES ('2018_07_09_00_characters.sql', '6F3EA22DD5E4CD9F9C60C4332B147E3DBF2E8A44', 'ARCHIVED', '2018-07-09 21:19:18', 0);
INSERT INTO `updates` VALUES ('2018_11_09_00_characters.sql', '50429D68E6EBD1149CDA14A9EA642BC06A1FAE3D', 'ARCHIVED', '2018-11-09 23:49:47', 0);
INSERT INTO `updates` VALUES ('2019_03_19_00_characters.sql', '1FD394E354CB9E854ABDC8CFD02329240AE07C3F', 'ARCHIVED', '2019-03-19 10:17:45', 0);
INSERT INTO `updates` VALUES ('2019_04_15_00_characters.sql', '942FB57BF890E523B35B9BFEF3686CB0AA52B795', 'ARCHIVED', '2019-04-15 09:16:09', 0);
INSERT INTO `updates` VALUES ('2019_05_15_00_characters.sql', 'A12F21C8044C8BC8E2AA17F4C6CEB8B722CBC714', 'ARCHIVED', '2019-05-15 09:13:20', 0);
INSERT INTO `updates` VALUES ('2019_06_15_00_characters.sql', '32DA6E004D7DD6EFFB0BB26238D17F6CC9E51DE6', 'ARCHIVED', '2019-06-15 10:33:45', 0);
INSERT INTO `updates` VALUES ('2019_07_14_00_characters.sql', 'A141F4F15BDF0320483921429871D4C572BD7E2D', 'ARCHIVED', '2019-07-04 03:00:00', 0);
INSERT INTO `updates` VALUES ('2019_07_15_00_characters.sql', '5BCF35896BB36A306CE79CF1E3F1945FAF9019D9', 'ARCHIVED', '2019-07-15 03:00:00', 0);
INSERT INTO `updates` VALUES ('2019_07_15_01_characters.sql', '5D383B026AB9EDE7114F249D206DE7E432E19468', 'ARCHIVED', '2019-07-15 03:00:00', 0);
INSERT INTO `updates` VALUES ('2019_07_16_00_characters.sql', '76AE193EFA3129FA1702BF7B6FA7C4127B543BDF', 'ARCHIVED', '2019-07-16 03:00:00', 0);
INSERT INTO `updates` VALUES ('2019_08_16_00_characters.sql', '7E21060060513C9504107C4A06B106166CC0768E', 'ARCHIVED', '2019-08-16 09:25:07', 0);
INSERT INTO `updates` VALUES ('2019_09_15_00_characters.sql', '75F3355AF6E9C0A2CAF5F523D87208C726C9D042', 'ARCHIVED', '2019-09-15 12:21:36', 0);
INSERT INTO `updates` VALUES ('2019_10_18_00_characters.sql', '143669FB0AA803C7287A4FF4ABE85F45F63369E5', 'ARCHIVED', '2019-10-18 11:37:37', 0);
INSERT INTO `updates` VALUES ('2019_11_16_00_characters.sql', 'F29BBE2869E2187B278B27236A3D156B849F0E43', 'ARCHIVED', '2019-11-16 15:06:06', 0);
INSERT INTO `updates` VALUES ('2019_12_15_00_characters.sql', '2EE449B59D56F884796B5D43C89B7C73DBF53939', 'ARCHIVED', '2019-12-15 21:26:21', 0);
INSERT INTO `updates` VALUES ('2020_01_15_00_characters.sql', 'CCA31041B25FC4BD631D07BB2E42B3C8F32465F0', 'ARCHIVED', '2020-01-15 10:45:18', 0);
INSERT INTO `updates` VALUES ('2020_02_15_00_characters.sql', '645EDA60CDD479B3D1E78D2D89DAB5D6EA1FB7BC', 'ARCHIVED', '2020-02-15 20:36:05', 0);
INSERT INTO `updates` VALUES ('2020_03_16_00_characters.sql', 'A38437DA80F6A5D35958A09CFC74EE1CDC465BA8', 'ARCHIVED', '2020-03-16 11:47:49', 0);
INSERT INTO `updates` VALUES ('2020_04_15_00_characters.sql', '61F7DE3B81C7E479FFF9463A39DF568826926F39', 'ARCHIVED', '2020-04-15 14:03:56', 0);
INSERT INTO `updates` VALUES ('2020_05_15_00_characters.sql', 'F1C2FECAA4EB623560914E7758E0BB8364CA135A', 'ARCHIVED', '2020-05-15 11:55:56', 0);
INSERT INTO `updates` VALUES ('2020_06_15_00_characters.sql', '99C4F85580421E928003380D8F992C4EF5E627DF', 'ARCHIVED', '2020-06-15 10:48:08', 0);
INSERT INTO `updates` VALUES ('2020_07_15_00_characters.sql', 'D87627DC6E4D222F68A1F56F0B3B986EF9A590EF', 'ARCHIVED', '2020-07-15 13:35:41', 0);
INSERT INTO `updates` VALUES ('2020_08_15_00_characters.sql', '70979D488ACD23DEB8E45D31C3ADC690A1B81F79', 'ARCHIVED', '2020-08-15 12:34:44', 0);
INSERT INTO `updates` VALUES ('2020_08_22_00_characters.sql', '78251072C9281D98BC4EAC523DA0858C9F8425D9', 'ARCHIVED', '2020-08-22 19:27:27', 0);
INSERT INTO `updates` VALUES ('2020_09_02_00_characters.sql', '627F320D58A42F401AB10ABA927F2B37C1981576', 'ARCHIVED', '2020-09-02 20:41:04', 0);
INSERT INTO `updates` VALUES ('2020_09_15_00_characters.sql', 'F9B6FCB934D787336188157EF688541A5F186EF5', 'ARCHIVED', '2021-07-20 23:50:08', 6);
INSERT INTO `updates` VALUES ('2020_10_15_00_characters.sql', '72F769B6EFFA4C2C5E08235C89EF2629C0FA82EF', 'ARCHIVED', '2021-10-17 06:28:18', 28);
INSERT INTO `updates` VALUES ('2020_11_16_00_characters.sql', '6389519BF44A6EC61E744E6A9727E9448337A276', 'ARCHIVED', '2021-10-17 06:28:18', 33);
INSERT INTO `updates` VALUES ('2020_12_15_00_characters.sql', '650EE26F85517977FBDEB42CCB97CEFA6462502E', 'ARCHIVED', '2021-10-17 06:28:19', 33);
INSERT INTO `updates` VALUES ('2021_01_15_00_characters.sql', '4D3A4C71ACD4CB04B014C300E1D0B33C0699DBE7', 'ARCHIVED', '2021-10-17 06:28:19', 34);
INSERT INTO `updates` VALUES ('2021_02_15_00_characters.sql', '53D94CFC60329E7BD036B77D4B298785AF57AD79', 'ARCHIVED', '2021-10-17 06:28:19', 33);
INSERT INTO `updates` VALUES ('2021_03_15_00_characters.sql', 'D6274D688A3E2A4F727B565481D99A49E9C642D4', 'ARCHIVED', '2021-10-17 06:28:19', 33);
INSERT INTO `updates` VALUES ('2021_04_16_00_characters.sql', '0EBBF50CE3EB1197973E403C2F1D60881BB497FD', 'ARCHIVED', '2021-10-17 06:28:19', 27);
INSERT INTO `updates` VALUES ('2021_05_14_00_characters.sql', '9834A657E1E1F650E9A7E4E793BAB3E2AFF65293', 'ARCHIVED', '2021-10-17 06:28:19', 33);
INSERT INTO `updates` VALUES ('2021_06_15_00_characters.sql', '4432846E2B0769C01E2B333EEDD90ABA44BF2BC1', 'ARCHIVED', '2021-10-17 06:28:19', 32);
INSERT INTO `updates` VALUES ('2021_07_15_00_characters.sql', '2ACFF71253DEA0F059476AEA52A55196E6D5DCFE', 'ARCHIVED', '2021-10-17 06:28:19', 33);
INSERT INTO `updates` VALUES ('2021_08_15_00_characters.sql', 'A8A32D47C65FB6A0C7995F8342D85B6A50C6C63F', 'ARCHIVED', '2021-10-17 06:29:53', 27);
INSERT INTO `updates` VALUES ('2021_09_28_00_characters.sql', 'A57869AE14FFFA935AB57318F65F4F1217AA2421', 'ARCHIVED', '2021-10-17 06:29:53', 29);
INSERT INTO `updates` VALUES ('2021_10_15_00_characters.sql', '174355CFEA8FADA50B731F54E028561AFCF46AA6', 'RELEASED', '2021-10-17 06:29:53', 26);

-- ----------------------------
-- Table structure for updates_include
-- ----------------------------
DROP TABLE IF EXISTS `updates_include`;
CREATE TABLE `updates_include`  (
  `path` varchar(200) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL COMMENT 'directory to include. $ means relative to the source directory.',
  `state` enum('RELEASED','ARCHIVED') CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL DEFAULT 'RELEASED' COMMENT 'defines if the directory contains released or archived updates.',
  PRIMARY KEY (`path`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'List of directories where we want to include sql updates.' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of updates_include
-- ----------------------------
INSERT INTO `updates_include` VALUES ('$/sql/updates/characters', 'RELEASED');
INSERT INTO `updates_include` VALUES ('$/sql/custom/characters2', 'RELEASED');
INSERT INTO `updates_include` VALUES ('$/sql/old/3.3.5a/characters', 'ARCHIVED');

-- ----------------------------
-- Table structure for warden_action
-- ----------------------------
DROP TABLE IF EXISTS `warden_action`;
CREATE TABLE `warden_action`  (
  `wardenId` smallint UNSIGNED NOT NULL,
  `action` tinyint UNSIGNED NULL DEFAULT NULL,
  PRIMARY KEY (`wardenId`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of warden_action
-- ----------------------------

-- ----------------------------
-- Table structure for world_coded
-- ----------------------------
DROP TABLE IF EXISTS `world_coded`;
CREATE TABLE `world_coded`  (
  `CodeId` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `CodeData` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `CodeStart` timestamp NOT NULL DEFAULT '1970-03-01 21:34:53',
  `CodeEnd` timestamp NOT NULL DEFAULT '2000-03-01 21:34:53',
  `MailSubject` varchar(200) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `MailText` varchar(200) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `MailMoney` int NOT NULL DEFAULT 0,
  `MaxCountForCharacter` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `MaxCountForAccount` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `MaxCountTotal` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `Comment` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  PRIMARY KEY (`CodeId`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 2 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of world_coded
-- ----------------------------
INSERT INTO `world_coded` VALUES (1, '123456789', '2023-01-01 21:34:53', '2036-01-01 15:42:39', 'test', 'test', 100000, 1, 1, 100, 'ntcn');

-- ----------------------------
-- Table structure for world_coded_history
-- ----------------------------
DROP TABLE IF EXISTS `world_coded_history`;
CREATE TABLE `world_coded_history`  (
  `Id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `CodeId` int UNSIGNED NOT NULL DEFAULT 0,
  `CharacterGuid` int UNSIGNED NOT NULL DEFAULT 0,
  `AccountId` int UNSIGNED NOT NULL DEFAULT 0,
  `SessionIp` varchar(32) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NOT NULL,
  `TimeActivated` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`Id`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 19 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of world_coded_history
-- ----------------------------
INSERT INTO `world_coded_history` VALUES (1, 1, 37, 1, '192.168.100.11', '2023-03-12 09:15:31');
INSERT INTO `world_coded_history` VALUES (2, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:14');
INSERT INTO `world_coded_history` VALUES (3, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:27');
INSERT INTO `world_coded_history` VALUES (4, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:31');
INSERT INTO `world_coded_history` VALUES (5, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:35');
INSERT INTO `world_coded_history` VALUES (6, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:38');
INSERT INTO `world_coded_history` VALUES (7, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:43');
INSERT INTO `world_coded_history` VALUES (8, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:47');
INSERT INTO `world_coded_history` VALUES (9, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:53');
INSERT INTO `world_coded_history` VALUES (10, 1, 37, 1, '192.168.100.11', '2023-03-12 09:17:57');
INSERT INTO `world_coded_history` VALUES (11, 1, 13, 1, '127.0.0.1', '2023-03-20 23:24:35');
INSERT INTO `world_coded_history` VALUES (12, 1, 13, 1, '127.0.0.1', '2023-03-20 23:32:21');
INSERT INTO `world_coded_history` VALUES (13, 1, 13, 1, '127.0.0.1', '2023-03-20 23:42:04');
INSERT INTO `world_coded_history` VALUES (14, 1, 28, 3, '127.0.0.1', '2023-03-24 17:38:49');
INSERT INTO `world_coded_history` VALUES (15, 1, 12, 1, '178.120.212.149', '2025-02-02 17:03:56');
INSERT INTO `world_coded_history` VALUES (16, 1, 12, 1, '178.120.212.149', '2025-02-02 17:04:10');
INSERT INTO `world_coded_history` VALUES (17, 1, 12, 1, '178.120.212.149', '2025-02-02 17:04:48');
INSERT INTO `world_coded_history` VALUES (18, 1, 12, 1, '178.120.212.149', '2025-02-02 17:04:57');

-- ----------------------------
-- Table structure for world_coded_items
-- ----------------------------
DROP TABLE IF EXISTS `world_coded_items`;
CREATE TABLE `world_coded_items`  (
  `CodeId` int NOT NULL,
  `ItemId` int UNSIGNED NOT NULL DEFAULT 0,
  `ItemCount` smallint UNSIGNED NOT NULL DEFAULT 1,
  `ItemId1` bigint UNSIGNED NOT NULL DEFAULT 0,
  `ItemCount1` bigint UNSIGNED NOT NULL DEFAULT 0,
  `ItemId2` bigint UNSIGNED NOT NULL DEFAULT 0,
  `ItemCount2` bigint UNSIGNED NOT NULL DEFAULT 0,
  `ItemId3` bigint UNSIGNED NOT NULL DEFAULT 0,
  `ItemCount3` bigint UNSIGNED NOT NULL DEFAULT 0,
  `ItemId4` bigint UNSIGNED NOT NULL DEFAULT 0,
  `ItemCount4` bigint UNSIGNED NOT NULL DEFAULT 0,
  `ItemClassMask` int UNSIGNED NOT NULL DEFAULT 0,
  `ItemRaceMask` int UNSIGNED NOT NULL DEFAULT 0,
  `Comment` varchar(255) CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL DEFAULT NULL,
  PRIMARY KEY (`CodeId`, `ItemId`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of world_coded_items
-- ----------------------------
INSERT INTO `world_coded_items` VALUES (1, 51225, 1, 51226, 1, 51227, 1, 51228, 1, 51229, 1, 1535, 1791, 'vn');

-- ----------------------------
-- Table structure for worldstates
-- ----------------------------
DROP TABLE IF EXISTS `worldstates`;
CREATE TABLE `worldstates`  (
  `entry` int UNSIGNED NOT NULL DEFAULT 0,
  `value` int UNSIGNED NOT NULL DEFAULT 0,
  `comment` tinytext CHARACTER SET utf8mb3 COLLATE utf8mb3_general_ci NULL,
  PRIMARY KEY (`entry`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb3 COLLATE = utf8mb3_general_ci COMMENT = 'Variable Saves' ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of worldstates
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
