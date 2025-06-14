/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 80041 (8.0.41)
 Source Host           : localhost:3306
 Source Schema         : characters

 Target Server Type    : MySQL
 Target Server Version : 80041 (8.0.41)
 File Encoding         : 65001

 Date: 14/06/2025 05:02:31
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for character_bg_replays
-- ----------------------------
DROP TABLE IF EXISTS `character_bg_replays`;
CREATE TABLE `character_bg_replays`  (
  `id` int NOT NULL AUTO_INCREMENT,
  `arenaTypeId` int NOT NULL,
  `typeId` int NOT NULL,
  `contentSize` int NOT NULL,
  `contents` longblob NOT NULL,
  `mapId` int NOT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `idx_arenaTypeId`(`arenaTypeId` ASC) USING BTREE,
  INDEX `idx_mapId`(`mapId` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of character_bg_replays
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
