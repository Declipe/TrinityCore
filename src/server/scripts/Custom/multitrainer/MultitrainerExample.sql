SET @ENTRY = (SELECT max(entry)+1 FROM creature_template2);
SET @MENU_ID = (SELECT max(menuid)+1 FROM gossip_menu_option);

INSERT INTO `creature_template2` (`entry`, `modelid1`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`) VALUES
(@ENTRY, 5080, "Rakka", "MultiTrainer", NULL, @MENU_ID, 10, 10, 0, 35, 17, 1, 1.14286, 1, 0, 0, 1500, 0, 1, 512, 2048, 8, 0, 7, 138412032, 0, 0, 0, 0, 0, 0, 0, '', 0, 1, 0, 0, 1, 0, 2, '');

INSERT INTO `gossip_menu_option` (`menuid`, `optionid`, `optionicon`, `optiontext`, `optiontype`, `optionnpcflag`, `actionmenuid`, `actionpoiid`, `boxcoded`, `boxmoney`, `boxtext`) VALUES
(@MENU_ID, 0, 3, 'TrainerTest 33684', 5, 16, 33684, 0, 0, 0, ''),
(@MENU_ID, 1, 3, 'TrainerTest 2704', 5, 16, 2704, 0, 0, 0, ''),
(@MENU_ID, 2, 3, 'Warrior', 5, 16, 60615, 0, 0, 0, ''),
(@MENU_ID, 3, 3, 'Paladin', 5, 16, 60611, 0, 0, 0, ''),
(@MENU_ID, 4, 3, 'Hunter', 5, 16, 60616, 0, 0, 0, ''),
(@MENU_ID, 5, 3, 'Rogue', 5, 16, 60617, 0, 0, 0, ''),
(@MENU_ID, 6, 3, 'Priest', 5, 16, 60613, 0, 0, 0, ''),
(@MENU_ID, 7, 3, 'Death Knight', 5, 16, 60620, 0, 0, 0, ''),
(@MENU_ID, 8, 3, 'Shaman', 5, 16, 60614, 0, 0, 0, ''),
(@MENU_ID, 9, 3, 'Mage', 5, 16, 60612, 0, 0, 0, ''),
(@MENU_ID, 10, 3, 'Warlock', 5, 16, 60618, 0, 0, 0, ''),
(@MENU_ID, 11, 3, 'Druid', 5, 16, 60619, 0, 0, 0, '');
