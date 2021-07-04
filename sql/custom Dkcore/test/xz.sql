-- Self Stun removes the stun visual when it expires
DELETE FROM `spell_linked_spell` WHERE `spell_effect`=-18970;
INSERT INTO `spell_linked_spell` (`spell_trigger`,`spell_effect`,`type`,`comment`) VALUES
(-65256,-18970,0,"Self Stun removal removes Self Stun - (Visual Only)");