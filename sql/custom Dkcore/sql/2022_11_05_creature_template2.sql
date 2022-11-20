
ALTER TABLE `creature_template2` MODIFY `difficulty_entry_1` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `difficulty_entry_2` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `difficulty_entry_3` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `entry` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `gossip_menu_id` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `lootid` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `maxgold` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `mingold` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `modelid1` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `modelid2` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `modelid3` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `modelid4` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `PetSpellDataId` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `pickpocketloot` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `skinloot` int(10) unsigned NOT NULL DEFAULT '0';
ALTER TABLE `creature_template2` MODIFY `VehicleId` int(10) unsigned NOT NULL DEFAULT '0';

ALTER TABLE creature_template2 ENGINE=InnoDB ROW_FORMAT=DEFAULT;

ALTER TABLE creature_template2 CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;