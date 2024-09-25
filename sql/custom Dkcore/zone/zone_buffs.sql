/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 80036 (8.0.36)
 Source Host           : localhost:3306
 Source Schema         : zyn

 Target Server Type    : MySQL
 Target Server Version : 80036 (8.0.36)
 File Encoding         : 65001

 Date: 25/09/2024 00:55:25
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for zone_buffs
-- ----------------------------
DROP TABLE IF EXISTS `zone_buffs`;
CREATE TABLE `zone_buffs`  (
  `zone_id` int NOT NULL,
  `buff_id` int NOT NULL,
  PRIMARY KEY (`zone_id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of zone_buffs
-- ----------------------------
INSERT INTO `zone_buffs` VALUES (12, 48469);
INSERT INTO `zone_buffs` VALUES (14, 48162);
INSERT INTO `zone_buffs` VALUES (85, 46668);
INSERT INTO `zone_buffs` VALUES (4080, 48074);

SET FOREIGN_KEY_CHECKS = 1;
