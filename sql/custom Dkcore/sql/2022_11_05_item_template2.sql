
ALTER TABLE `item_template2` MODIFY `area` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `block` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `DisenchantID` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `displayid` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `entry` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `itemset` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `lockid` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `PageText` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `RandomSuffix` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `RequiredCityRank` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `requiredhonorrank` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `requiredspell` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `startquest` int(10) unsigned NOT NULL DEFAULT '0';


ALTER TABLE `item_template2` MODIFY `BagFamily` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `GemProperties` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `RandomProperty` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `socketBonus` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `socketContent_1` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `socketContent_2` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `socketContent_3` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `spellid_1` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `spellid_2` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `spellid_3` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `spellid_4` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `spellid_5` int(11) NOT NULL DEFAULT '0';
ALTER TABLE `item_template2` MODIFY `TotemCategory` int(11) NOT NULL DEFAULT '0';




ALTER TABLE item_template2 ENGINE=InnoDB ROW_FORMAT=DEFAULT;

ALTER TABLE item_template2 CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
