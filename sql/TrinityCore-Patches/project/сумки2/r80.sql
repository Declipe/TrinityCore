INSERT INTO `item_template` (`entry`, `class`, `subclass`, `SoundOverrideSubclass`, `name`, `displayid`, `Quality`, `Flags`, `FlagsExtra`, `BuyCount`, `BuyPrice`, `SellPrice`, `InventoryType`, `AllowableClass`, `AllowableRace`, `ItemLevel`, `RequiredLevel`, `RequiredSkill`, `RequiredSkillRank`, `requiredspell`, `requiredhonorrank`, `RequiredCityRank`, `RequiredReputationFaction`, `RequiredReputationRank`, `maxcount`, `stackable`, `ContainerSlots`, `StatsCount`, `stat_type1`, `stat_value1`, `stat_type2`, `stat_value2`, `stat_type3`, `stat_value3`, `stat_type4`, `stat_value4`, `stat_type5`, `stat_value5`, `stat_type6`, `stat_value6`, `stat_type7`, `stat_value7`, `stat_type8`, `stat_value8`, `stat_type9`, `stat_value9`, `stat_type10`, `stat_value10`, `ScalingStatDistribution`, `ScalingStatValue`, `dmg_min1`, `dmg_max1`, `dmg_type1`, `dmg_min2`, `dmg_max2`, `dmg_type2`, `armor`, `holy_res`, `fire_res`, `nature_res`, `frost_res`, `shadow_res`, `arcane_res`, `delay`, `ammo_type`, `RangedModRange`, `spellid_1`, `spelltrigger_1`, `spellcharges_1`, `spellppmRate_1`, `spellcooldown_1`, `spellcategory_1`, `spellcategorycooldown_1`, `spellid_2`, `spelltrigger_2`, `spellcharges_2`, `spellppmRate_2`, `spellcooldown_2`, `spellcategory_2`, `spellcategorycooldown_2`, `spellid_3`, `spelltrigger_3`, `spellcharges_3`, `spellppmRate_3`, `spellcooldown_3`, `spellcategory_3`, `spellcategorycooldown_3`, `spellid_4`, `spelltrigger_4`, `spellcharges_4`, `spellppmRate_4`, `spellcooldown_4`, `spellcategory_4`, `spellcategorycooldown_4`, `spellid_5`, `spelltrigger_5`, `spellcharges_5`, `spellppmRate_5`, `spellcooldown_5`, `spellcategory_5`, `spellcategorycooldown_5`, `bonding`, `description`, `PageText`, `LanguageID`, `PageMaterial`, `startquest`, `lockid`, `Material`, `sheath`, `RandomProperty`, `RandomSuffix`, `block`, `itemset`, `MaxDurability`, `area`, `Map`, `BagFamily`, `TotemCategory`, `socketColor_1`, `socketContent_1`, `socketColor_2`, `socketContent_2`, `socketColor_3`, `socketContent_3`, `socketBonus`, `GemProperties`, `RequiredDisenchantSkill`, `ArmorDamageModifier`, `duration`, `ItemLimitCategory`, `HolidayId`, `ScriptName`, `DisenchantID`, `FoodType`, `minMoneyLoot`, `maxMoneyLoot`, `flagsCustom`, `VerifiedBuild`) VALUES
(179658, 15, 0, -1, 'Yellow Sack of Gems', 1168, 2, 4, 0, 1, 854, 213, 0, -1, -1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1, 0, '', 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, '', 0, 0, 0, 0, 0, 12340);

DELETE FROM `item_loot_template` WHERE (Entry = 17965);
DELETE FROM `item_loot_template` WHERE (Entry = 179658);
INSERT INTO `item_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(179658, 54569, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 54572, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 54573, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53127, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53134, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53488, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53490, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53489, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53486, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53133, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53132, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53126, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 53125, 0, 6, 0, 1, 0, 1, 1, NULL),
(179658, 49703, 0, 100, 0, 1, 0, 1, 1, NULL),
(17965, 1529, 0, 73.5, 0, 1, 0, 1, 3, NULL),
(17965, 3864, 0, 47.1, 0, 1, 0, 1, 3, NULL),
(17965, 7909, 0, 76.5, 0, 1, 0, 1, 3, NULL),
(17965, 7910, 0, 73.5, 0, 1, 0, 1, 3, NULL),
(17965, 7971, 0, 44.1, 0, 1, 0, 1, 1, NULL),
(17965, 12363, 0, 88.2, 0, 1, 0, 1, 1, NULL),
(17965, 13926, 0, 5.9, 0, 1, 0, 1, 1, NULL),
(179658, 45706, 0, 10, 0, 1, 0, 1, 1, NULL);

