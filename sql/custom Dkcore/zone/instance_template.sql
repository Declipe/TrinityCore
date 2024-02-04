/*
 Navicat Premium Data Transfer

 Source Server         : localhost_3306
 Source Server Type    : MySQL
 Source Server Version : 80035 (8.0.35)
 Source Host           : localhost:3306
 Source Schema         : world3.3.5_beta

 Target Server Type    : MySQL
 Target Server Version : 80035 (8.0.35)
 File Encoding         : 65001

 Date: 29/01/2024 15:32:23
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for instance_template
-- ----------------------------
DROP TABLE IF EXISTS `instance_template`;
CREATE TABLE `instance_template`  (
  `map` smallint UNSIGNED NOT NULL,
  `parent` smallint UNSIGNED NOT NULL,
  `script` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `allowMount` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`map`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of instance_template
-- ----------------------------
INSERT INTO `instance_template` VALUES (30, 0, '', 1);
INSERT INTO `instance_template` VALUES (33, 0, 'instance_shadowfang_keep', 0);
INSERT INTO `instance_template` VALUES (34, 0, 'instance_the_stockade', 0);
INSERT INTO `instance_template` VALUES (36, 0, 'instance_deadmines', 0);
INSERT INTO `instance_template` VALUES (43, 1, 'instance_wailing_caverns', 0);
INSERT INTO `instance_template` VALUES (44, 0, '', 0);
INSERT INTO `instance_template` VALUES (47, 1, 'instance_razorfen_kraul', 0);
INSERT INTO `instance_template` VALUES (48, 1, 'instance_blackfathom_deeps', 0);
INSERT INTO `instance_template` VALUES (70, 0, 'instance_uldaman', 0);
INSERT INTO `instance_template` VALUES (90, 0, 'instance_gnomeregan', 0);
INSERT INTO `instance_template` VALUES (109, 0, 'instance_sunken_temple', 0);
INSERT INTO `instance_template` VALUES (129, 1, 'instance_razorfen_downs', 0);
INSERT INTO `instance_template` VALUES (169, 0, '', 0);
INSERT INTO `instance_template` VALUES (189, 0, 'instance_scarlet_monastery', 0);
INSERT INTO `instance_template` VALUES (209, 1, 'instance_zulfarrak', 1);
INSERT INTO `instance_template` VALUES (229, 0, 'instance_blackrock_spire', 0);
INSERT INTO `instance_template` VALUES (230, 0, 'instance_blackrock_depths', 0);
INSERT INTO `instance_template` VALUES (249, 1, 'instance_onyxias_lair', 0);
INSERT INTO `instance_template` VALUES (269, 1, 'instance_the_black_morass', 1);
INSERT INTO `instance_template` VALUES (289, 0, 'instance_scholomance', 0);
INSERT INTO `instance_template` VALUES (309, 0, 'instance_zulgurub', 1);
INSERT INTO `instance_template` VALUES (329, 0, 'instance_stratholme', 0);
INSERT INTO `instance_template` VALUES (349, 1, 'instance_maraudon', 0);
INSERT INTO `instance_template` VALUES (389, 1, 'instance_ragefire_chasm', 0);
INSERT INTO `instance_template` VALUES (409, 230, 'instance_molten_core', 0);
INSERT INTO `instance_template` VALUES (429, 1, 'instance_dire_maul', 0);
INSERT INTO `instance_template` VALUES (469, 229, 'instance_blackwing_lair', 0);
INSERT INTO `instance_template` VALUES (489, 0, '', 1);
INSERT INTO `instance_template` VALUES (509, 1, 'instance_ruins_of_ahnqiraj', 1);
INSERT INTO `instance_template` VALUES (529, 0, '', 1);
INSERT INTO `instance_template` VALUES (531, 1, 'instance_temple_of_ahnqiraj', 1);
INSERT INTO `instance_template` VALUES (532, 0, 'instance_karazhan', 0);
INSERT INTO `instance_template` VALUES (533, 571, 'instance_naxxramas', 0);
INSERT INTO `instance_template` VALUES (534, 1, 'instance_hyjal', 1);
INSERT INTO `instance_template` VALUES (540, 530, 'instance_shattered_halls', 0);
INSERT INTO `instance_template` VALUES (542, 530, 'instance_blood_furnace', 0);
INSERT INTO `instance_template` VALUES (543, 530, 'instance_ramparts', 0);
INSERT INTO `instance_template` VALUES (544, 530, 'instance_magtheridons_lair', 0);
INSERT INTO `instance_template` VALUES (545, 530, 'instance_steam_vault', 0);
INSERT INTO `instance_template` VALUES (546, 530, 'instance_the_underbog', 0);
INSERT INTO `instance_template` VALUES (547, 530, 'instance_the_slave_pens', 0);
INSERT INTO `instance_template` VALUES (548, 530, 'instance_serpent_shrine', 0);
INSERT INTO `instance_template` VALUES (550, 530, 'instance_the_eye', 0);
INSERT INTO `instance_template` VALUES (552, 530, 'instance_arcatraz', 0);
INSERT INTO `instance_template` VALUES (553, 530, 'instance_the_botanica', 0);
INSERT INTO `instance_template` VALUES (554, 530, 'instance_mechanar', 0);
INSERT INTO `instance_template` VALUES (555, 530, 'instance_shadow_labyrinth', 0);
INSERT INTO `instance_template` VALUES (556, 530, 'instance_sethekk_halls', 0);
INSERT INTO `instance_template` VALUES (557, 530, 'instance_mana_tombs', 0);
INSERT INTO `instance_template` VALUES (558, 530, 'instance_auchenai_crypts', 0);
INSERT INTO `instance_template` VALUES (559, 0, '', 1);
INSERT INTO `instance_template` VALUES (560, 1, 'instance_old_hillsbrad', 1);
INSERT INTO `instance_template` VALUES (562, 0, '', 1);
INSERT INTO `instance_template` VALUES (564, 530, 'instance_black_temple', 1);
INSERT INTO `instance_template` VALUES (565, 530, 'instance_gruuls_lair', 0);
INSERT INTO `instance_template` VALUES (566, 0, '', 1);
INSERT INTO `instance_template` VALUES (568, 530, 'instance_zulaman', 1);
INSERT INTO `instance_template` VALUES (572, 0, '', 1);
INSERT INTO `instance_template` VALUES (574, 571, 'instance_utgarde_keep', 1);
INSERT INTO `instance_template` VALUES (575, 571, 'instance_utgarde_pinnacle', 0);
INSERT INTO `instance_template` VALUES (576, 571, 'instance_nexus', 0);
INSERT INTO `instance_template` VALUES (578, 571, 'instance_oculus', 1);
INSERT INTO `instance_template` VALUES (580, 530, 'instance_sunwell_plateau', 1);
INSERT INTO `instance_template` VALUES (585, 530, 'instance_magisters_terrace', 0);
INSERT INTO `instance_template` VALUES (595, 1, 'instance_culling_of_stratholme', 1);
INSERT INTO `instance_template` VALUES (598, 0, '', 0);
INSERT INTO `instance_template` VALUES (599, 571, 'instance_halls_of_stone', 0);
INSERT INTO `instance_template` VALUES (600, 571, 'instance_drak_tharon_keep', 1);
INSERT INTO `instance_template` VALUES (601, 571, 'instance_azjol_nerub', 0);
INSERT INTO `instance_template` VALUES (602, 571, 'instance_halls_of_lightning', 0);
INSERT INTO `instance_template` VALUES (603, 571, 'instance_ulduar', 1);
INSERT INTO `instance_template` VALUES (604, 571, 'instance_gundrak', 0);
INSERT INTO `instance_template` VALUES (607, 0, '', 1);
INSERT INTO `instance_template` VALUES (608, 571, 'instance_violet_hold', 0);
INSERT INTO `instance_template` VALUES (615, 571, 'instance_obsidian_sanctum', 1);
INSERT INTO `instance_template` VALUES (616, 571, 'instance_eye_of_eternity', 0);
INSERT INTO `instance_template` VALUES (619, 571, 'instance_ahnkahet', 0);
INSERT INTO `instance_template` VALUES (624, 571, 'instance_vault_of_archavon', 0);
INSERT INTO `instance_template` VALUES (628, 0, '', 1);
INSERT INTO `instance_template` VALUES (631, 571, 'instance_icecrown_citadel', 1);
INSERT INTO `instance_template` VALUES (632, 571, 'instance_forge_of_souls', 0);
INSERT INTO `instance_template` VALUES (649, 571, 'instance_trial_of_the_crusader', 0);
INSERT INTO `instance_template` VALUES (650, 571, 'dk_instance_trial_of_the_champion', 0);
INSERT INTO `instance_template` VALUES (658, 571, 'instance_pit_of_saron', 1);
INSERT INTO `instance_template` VALUES (668, 571, 'instance_halls_of_reflection', 1);
INSERT INTO `instance_template` VALUES (724, 571, 'instance_ruby_sanctum', 1);

SET FOREIGN_KEY_CHECKS = 1;
