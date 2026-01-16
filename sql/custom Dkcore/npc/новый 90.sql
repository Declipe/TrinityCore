

INSERT INTO `creature_template2` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `dmgschool`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `DamageModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `spell_school_immune_mask`, `flags_extra`, `ScriptName`, `StringId`, `VerifiedBuild`) VALUES (90144, 0, 0, 0, 0, 0, 23917, 0, 0, 0, 'Летающий Бутер', NULL, NULL, 0, 80, 80, 2, 121, 0, 1, 1.42857, 1, 0, 0, 2000, 2000, 1, 1, 1, 32768, 2048, 0, 0, 7, 0, 0, 4624, 0, 0, 0, 0, 0, '', 0, 1, 4, 600, 1, 60, 1, 0, 0, 1, 0, 0, 65536, 'npc_guard_generic', NULL, 12340);


INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Chase`, `Random`, `InteractionPauseTimer`) VALUES (90144, 2, 1, 1, 0, 0, 0, NULL);

INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `MountCreatureID`, `StandState`, `AnimTier`, `VisFlags`, `SheathState`, `PvPFlags`, `emote`, `visibilityDistanceType`, `auras`) VALUES (90144, 0, 0, 0, 0, 3, 0, 1, 0, 0, 3, NULL);
