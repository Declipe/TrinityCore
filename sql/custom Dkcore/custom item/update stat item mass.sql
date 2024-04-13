-- 0 ITEM_MOD_MANA
-- 1 ITEM_MOD_HEALTH
-- 3 ITEM_MOD_AGILITY
-- 4 ITEM_MOD_STRENGTH
-- 5 ITEM_MOD_INTELLECT
-- 6 ITEM_MOD_SPIRIT
-- 7 ITEM_MOD_STAMINA
-- 12 ITEM_MOD_DEFENSE_SKILL_RATING
-- 13 ITEM_MOD_DODGE_RATING
-- 14 ITEM_MOD_PARRY_RATING
-- 15 ITEM_MOD_BLOCK_RATING
-- 16 ITEM_MOD_HIT_MELEE_RATING
-- 17 ITEM_MOD_HIT_RANGED_RATING
-- 18 ITEM_MOD_HIT_SPELL_RATING
-- 19 ITEM_MOD_CRIT_MELEE_RATING
-- 20 ITEM_MOD_CRIT_RANGED_RATING
-- 21 ITEM_MOD_CRIT_SPELL_RATING
-- 22 ITEM_MOD_HIT_TAKEN_MELEE_RATING
-- 23 ITEM_MOD_HIT_TAKEN_RANGED_RATING
-- 24 ITEM_MOD_HIT_TAKEN_SPELL_RATING
-- 25 ITEM_MOD_CRIT_TAKEN_MELEE_RATING
-- 26 ITEM_MOD_CRIT_TAKEN_RANGED_RATING
-- 27 ITEM_MOD_CRIT_TAKEN_SPELL_RATING
-- 28 ITEM_MOD_HASTE_MELEE_RATING
-- 29 ITEM_MOD_HASTE_RANGED_RATING
-- 30 ITEM_MOD_HASTE_SPELL_RATING
-- 31 ITEM_MOD_HIT_RATING
-- 32 ITEM_MOD_CRIT_RATING
-- 33 ITEM_MOD_HIT_TAKEN_RATING
-- 34 ITEM_MOD_CRIT_TAKEN_RATING
-- 35 ITEM_MOD_RESILIENCE_RATING
-- 36 ITEM_MOD_HASTE_RATING
-- 37 ITEM_MOD_EXPERTISE_RATING
-- 38 ITEM_MOD_ATTACK_POWER
-- 39 ITEM_MOD_RANGED_ATTACK_POWER
-- 40 ITEM_MOD_FERAL_ATTACK_POWER (not used as of 3.3)
-- 41 ITEM_MOD_SPELL_HEALING_DONE
-- 42 ITEM_MOD_SPELL_DAMAGE_DONE
-- 43 ITEM_MOD_MANA_REGENERATION
-- 44 ITEM_MOD_ARMOR_PENETRATION_RATING
-- 45 ITEM_MOD_SPELL_POWER
-- 46 ITEM_MOD_ HEALTH_REGEN
-- 47 ITEM_MOD_SPELL_PENETRATION
-- 48 ITEM_MOD_BLOCK_VALUE

-- UPDATE `item_template` SET `stat_value1`= `stat_value1` *5 where stat_type1 in (4); -- одиночно

-- масово
UPDATE `item_template` SET `stat_value1`= `stat_value1` *5;  
UPDATE `item_template` SET `stat_value2`= `stat_value2` *5;  
UPDATE `item_template` SET `stat_value3`= `stat_value3` *5;  
UPDATE `item_template` SET `stat_value4`= `stat_value4` *5;  
UPDATE `item_template` SET `stat_value5`= `stat_value5` *5;  
UPDATE `item_template` SET `stat_value6`= `stat_value6` *5;  
UPDATE `item_template` SET `stat_value7`= `stat_value7` *5;  
UPDATE `item_template` SET `stat_value8`= `stat_value8` *5;  
UPDATE `item_template` SET `stat_value9`= `stat_value9` *5;  
UPDATE `item_template` SET `stat_value10`= `stat_value10` *5;

-- UPDATE `item_template` SET `stat_value1`= `stat_value1` /3 where stat_type1 in (35);  
-- UPDATE `item_template` SET `stat_value2`= `stat_value2` /3 where stat_type2 in (35);  
-- UPDATE `item_template` SET `stat_value3`= `stat_value3` /3 where stat_type3 in (35);  
-- UPDATE `item_template` SET `stat_value4`= `stat_value4` /3 where stat_type4 in (35);  
-- UPDATE `item_template` SET `stat_value5`= `stat_value5` /3 where stat_type5 in (35);  
-- UPDATE `item_template` SET `stat_value6`= `stat_value6` /3 where stat_type6 in (35);  
-- UPDATE `item_template` SET `stat_value7`= `stat_value7` /3 where stat_type7 in (35);  
-- UPDATE `item_template` SET `stat_value8`= `stat_value8` /3 where stat_type8 in (35);  
-- UPDATE `item_template` SET `stat_value9`= `stat_value9` /3 where stat_type9 in (35);  
-- UPDATE `item_template` SET `stat_value10`= `stat_value10` /3 where stat_type10 in (35);
-- масово