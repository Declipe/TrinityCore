/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 80031 (8.0.31)
 Source Host           : localhost:3306
 Source Schema         : ctuldb

 Target Server Type    : MySQL
 Target Server Version : 80031 (8.0.31)
 File Encoding         : 65001

 Date: 26/12/2022 13:18:18
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for spell_target_position
-- ----------------------------
DROP TABLE IF EXISTS `spell_target_position2`;
CREATE TABLE `spell_target_position2`  (
  `ID` int UNSIGNED NOT NULL DEFAULT 0 COMMENT 'Identifier',
  `EffectIndex` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `MapID` smallint UNSIGNED NOT NULL DEFAULT 0,
  `PositionX` float NOT NULL DEFAULT 0,
  `PositionY` float NOT NULL DEFAULT 0,
  `PositionZ` float NOT NULL DEFAULT 0,
  `Orientation` float NOT NULL DEFAULT 0,
  `VerifiedBuild` smallint NULL DEFAULT 0,
  PRIMARY KEY (`ID`, `EffectIndex`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci COMMENT = 'Spell System' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of spell_target_position
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
