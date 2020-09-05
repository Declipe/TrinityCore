SET @ITEM_ID = 9537;
SET @ITEM_QUALITY = 6;
SET @ITEM_PROTO_FLAG_OPENABLE = 4;
SET @ITEM_PROTO_FLAG_GIFT = 1024;
SET @ITEM_FLAGS = @ITEM_PROTO_FLAG_OPENABLE + @ITEM_PROTO_FLAG_GIFT;
SET @CONDITION_CLASS_MASK = 15;
SET @CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE = 5;

DELETE FROM `item_template` WHERE `entry` = @ITEM_ID;
INSERT INTO `item_template`
(`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`, `FlagsExtra`, `BuyCount`,
`BuyPrice`, `SellPrice`, `InventoryType`, `AllowableClass`, `AllowableRace`, `ItemLevel`, `RequiredLevel`, `RequiredSkill`,
`RequiredSkillRank`, `requiredspell`, `requiredhonorrank`, `RequiredCityRank`, `RequiredReputationFaction`, `RequiredReputationRank`,
`maxcount`, `stackable`, `ContainerSlots`, `StatsCount`, `stat_type1`, `stat_value1`, `stat_type2`, `stat_value2`, `stat_type3`,
`stat_value3`, `stat_type4`, `stat_value4`, `stat_type5`, `stat_value5`, `stat_type6`, `stat_value6`, `stat_type7`, `stat_value7`,
`stat_type8`, `stat_value8`, `stat_type9`, `stat_value9`, `stat_type10`, `stat_value10`, `ScalingStatDistribution`,
`ScalingStatValue`, `dmg_min1`, `dmg_max1`, `dmg_type1`, `dmg_min2`, `dmg_max2`, `dmg_type2`, `armor`, `holy_res`, `fire_res`,
`nature_res`, `frost_res`, `shadow_res`, `arcane_res`, `delay`, `ammo_type`, `RangedModRange`, `spellid_1`, `spelltrigger_1`,
`spellcharges_1`, `spellppmRate_1`, `spellcooldown_1`, `spellcategory_1`, `spellcategorycooldown_1`, `spellid_2`, `spelltrigger_2`,
`spellcharges_2`, `spellppmRate_2`, `spellcooldown_2`, `spellcategory_2`, `spellcategorycooldown_2`, `spellid_3`, `spelltrigger_3`,
`spellcharges_3`, `spellppmRate_3`, `spellcooldown_3`, `spellcategory_3`, `spellcategorycooldown_3`, `spellid_4`, `spelltrigger_4`,
`spellcharges_4`, `spellppmRate_4`, `spellcooldown_4`, `spellcategory_4`, `spellcategorycooldown_4`, `spellid_5`, `spelltrigger_5`,
`spellcharges_5`, `spellppmRate_5`, `spellcooldown_5`, `spellcategory_5`, `spellcategorycooldown_5`, `bonding`, `description`,
`PageText`, `LanguageID`, `PageMaterial`, `startquest`, `lockid`, `Material`, `sheath`, `RandomProperty`, `RandomSuffix`, `block`,
`itemset`, `MaxDurability`, `area`, `Map`, `BagFamily`, `TotemCategory`, `socketColor_1`, `socketContent_1`, `socketColor_2`,
`socketContent_2`, `socketColor_3`, `socketContent_3`, `socketBonus`, `GemProperties`, `RequiredDisenchantSkill`,
`ArmorDamageModifier`, `duration`, `ItemLimitCategory`, `HolidayId`, `ScriptName`, `DisenchantID`, `FoodType`, `minMoneyLoot`,
`maxMoneyLoot`, `flagsCustom`, `VerifiedBuild`) VALUES
(@ITEM_ID, 15, 0, -1, 'Starter pack', 18499, @ITEM_QUALITY, @ITEM_FLAGS, 0, 1, 0, 0, 0, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0,
0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, '', 0, 0, 0, 0, 0, -1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0, 1);

DELETE FROM `playercreateinfo_item` WHERE `itemid` = @ITEM_ID;
INSERT INTO `playercreateinfo_item` (`race`, `class`, `itemid`, `amount`) VALUES (0, 0, @ITEM_ID, 1);

DELETE FROM `item_loot_template` WHERE `entry` = @ITEM_ID;
INSERT INTO `item_loot_template`
(`entry`, `item`, `Chance`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`) VALUES
(@ITEM_ID, 20774, 100, 1, 0, 1, 1), 	-- Champion's Deathdealer Breastplate (Hunter)
(@ITEM_ID, 20775, 100, 1, 0, 1, 1), 	-- Balanced Heartseeker (Rogue x2)
(@ITEM_ID, 20776, 100, 1, 0, 1, 1), 	-- Charmed Ancient Bone Bow (Rogue, Hunter, Warrior)
(@ITEM_ID, 20777, 100, 1, 0, 1, 1), 	-- Dignified Headmaster's Charge (Priest, Shaman, Mage, Warlock, Druid)
(@ITEM_ID, 20778, 100, 1, 0, 1, 1),
(@ITEM_ID, 20779, 100, 1, 0, 1, 1),
(@ITEM_ID, 858, 100, 1, 0, 10, 10), 			-- Lesser Healing Potion (All x10)
(@ITEM_ID, 2455, 100, 1, 0, 10, 10), 			-- Minor Mana Potion (All x10)
(@ITEM_ID, 2459, 100, 1, 0, 5, 5); 				-- Swiftness Potion (All x5)

SET @CLASS_WARRIOR = 1;
SET @CLASS_PALADIN = 2;
SET @CLASS_HUNTER = 4;
SET @CLASS_ROGUE = 8;
SET @CLASS_PRIEST = 16;
SET @CLASS_DEATH_KNIGHT = 32;
SET @CLASS_SHAMAN = 64;
SET @CLASS_MAGE = 128;
SET @CLASS_WARLOCK = 256;
SET @CLASS_DRUID = 1024;
SET @CLASS_PLATE = @CLASS_WARRIOR + @CLASS_PALADIN + @CLASS_DEATH_KNIGHT;
SET @CLASS_ALL = @CLASS_WARRIOR + @CLASS_PALADIN + @CLASS_DEATH_KNIGHT + @CLASS_DRUID + @CLASS_WARLOCK + @CLASS_MAGE + @CLASS_SHAMAN + @CLASS_PRIEST + @CLASS_ROGUE + @CLASS_HUNTER;


DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = @CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE AND `SourceGroup` = @ITEM_ID;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`,
`ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 20774, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge"),
(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 20775, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge"),
(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 20776, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge"),
(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 20777, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge"),
(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 20778, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge"),
(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 20779, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge"),

(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 858, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge"),
(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 2455, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge"),
(@CONDITION_SOURCE_TYPE_ITEM_LOOT_TEMPLATE, @ITEM_ID, 2459, 0, 0, @CONDITION_CLASS_MASK, 0, @CLASS_ALL, 0, 0, 0, 0, 0, '', "Dignified Headmaster's Charge");
