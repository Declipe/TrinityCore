-- INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (40184, '|cffffffff{}|r equipped items deleted for {}', NULL, '|cffffffff{}|r objets équipés supprimés pour {}', NULL, NULL, NULL, NULL, NULL, NULL);
-- INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (40185, '|cffffffff{}|r items in equipped bags deleted for {}', NULL, '|cffffffff{}|r objets supprimés dans les sacs de {}', NULL, NULL, NULL, NULL, NULL, NULL);
-- INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (40186, '|cffffffff{}|r items in bank deleted for {}', NULL, '|cffffffff{}|r objets supprimés de la banque de {}', NULL, NULL, NULL, NULL, NULL, NULL);
-- INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (40187, '|cffffffff{}|r keys in keyring deleted for {}', NULL, '|cffffffff{}|r objets supprimés du porte-clés de {}', NULL, NULL, NULL, NULL, NULL, NULL);
-- INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (40188, '|cffffffff{}|r currencies deleted for {}', NULL, '|cffffffff{}|r types de monnaies supprimées l\'inventaire de {}', NULL, NULL, NULL, NULL, NULL, NULL);
-- INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (40189, '|cffffffff{}|r items in vendors buyback deleted for {}', NULL, '|cffffffff{}|r objets supprimés dans l\'onglet rachat des vendeurs pour {}', NULL, NULL, NULL, NULL, NULL, NULL);
-- INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (40190, 'All items were deleted for {} :\n|cffffffff{}|r items equipped\n|cffffffff{}|r items in bags\n|cffffffff{}|r items in bank\n|cffffffff{}|r keys in keyring\n|cffffffff{}|r currency types\n|cffffffff{}|r items in vendor buyback', NULL, 'Tous les objets de {} ont été supprimés :\n|cffffffff{}|r objet équipés\n|cffffffff{}|r objets dans les sacs\n|cffffffff{}|r objets en banque\n|cffffffff{}|r clés dans le porte-clés\n|cffffffff{}|r types de monnaies\n|cffffffff{}|r objets dans l\'onglet rachat des vendeurs', NULL, NULL, NULL, NULL, NULL, NULL);
-- INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) VALUES (40191, 'All items were deleted for {} (bags included):\n|cffffffff{}|r equipped\n|cffffffff{}|r items in bags\n|cffffffff{}|r items in bank\n|cffffffff{}|r keys in keyring\n|cffffffff{}|r currency types\n|cffffffff{}|r items in vendor buyback\n|cffffffff{}|r standard bags\n|cffffffff{}|r bank bags', NULL, 'Tous les objets de {} ont été supprimés (sacs y-compris):\n|cffffffff{}|r objet équipés\n|cffffffff{}|r objets dans les sacs\n|cffffffff{}|r objets en banque\n|cffffffff{}|r clés dans le porte-clés\n|cffffffff{}|r types de monnaies\n|cffffffff{}|r objets dans l\'onglet rachat des vendeurs\n|cffffffff{}|r sacs standard\n|cffffffff{}|r sacs de banque', NULL, NULL, NULL, NULL, NULL, NULL);

INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc2`)
VALUES (40184, '|cffffffff%d|r equipped items deleted for %s', '|cffffffff%d|r objets équipés supprimés pour %s'),
	   (40185, '|cffffffff%d|r items in equipped bags deleted for %s', '|cffffffff%d|r objets supprimés dans les sacs de %s'),
	   (40186, '|cffffffff%d|r items in bank deleted for %s', '|cffffffff%d|r objets supprimés de la banque de %s'),
	   (40187, '|cffffffff%d|r keys in keyring deleted for %s', '|cffffffff%d|r objets supprimés du porte-clés de %s'),
	   (40188, '|cffffffff%d|r currencies deleted for %s', '|cffffffff%d|r types de monnaies supprimées l\'inventaire de %s'),
	   (40189, '|cffffffff%d|r items in vendors buyback deleted for %s', '|cffffffff%d|r objets supprimés dans l\'onglet rachat des vendeurs pour %s'),
	   (40190, 'All items were deleted for %s :
|cffffffff%d|r items equipped
|cffffffff%d|r items in bags
|cffffffff%d|r items in bank
|cffffffff%d|r keys in keyring
|cffffffff%d|r currency types
|cffffffff%d|r items in vendor buyback', 
'Tous les objets de %s ont été supprimés :
|cffffffff%d|r objet équipés
|cffffffff%d|r objets dans les sacs
|cffffffff%d|r objets en banque
|cffffffff%d|r clés dans le porte-clés
|cffffffff%d|r types de monnaies
|cffffffff%d|r objets dans l\'onglet rachat des vendeurs'),
	   (40191, 'All items were deleted for %s (bags included):
|cffffffff%d|r equipped
|cffffffff%d|r items in bags
|cffffffff%d|r items in bank
|cffffffff%d|r keys in keyring
|cffffffff%d|r currency types
|cffffffff%d|r items in vendor buyback
|cffffffff%d|r standard bags
|cffffffff%d|r bank bags', 
'Tous les objets de %s ont été supprimés (sacs y-compris):
|cffffffff%d|r objet équipés
|cffffffff%d|r objets dans les sacs
|cffffffff%d|r objets en banque
|cffffffff%d|r clés dans le porte-clés
|cffffffff%d|r types de monnaies
|cffffffff%d|r objets dans l\'onglet rachat des vendeurs
|cffffffff%d|r sacs standard
|cffffffff%d|r sacs de banque'
);

-- Insert values :
INSERT INTO `command`(`name`,`help`)
VALUES (
-- .reset items (main command)
'reset items', 'Syntax : .reset items equipped|bags|bank|keyring|currency|vendor_buyback|all|allbags #playername
Delete items in the player inventory (equipped, bank, bags etc...) depending on the chosen option.
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.'),

-- .reset items _____  (Sub-commands)
('reset items equipped', 'Syntax : .reset items equipped #playername
Delete all items equipped on the target player.
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.'),

('reset items bags', 'Syntax : .reset items bags #playername
Delete all items in the selected player\'s bags.
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.'),

('reset items bank', 'Syntax : .reset items bank #playername
Delete all items in the selected player\'s bank.
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.'),

('reset items keyring',  'Syntax : .reset items keyring #playername
Delete all items in the selected player\'s keyring.
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.'),

('reset items currency', 'Syntax : .reset items currency #playername
Delete all items in the selected player\'s currencies list.
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.'),

('reset items vendor_buyback', 'Syntax : .reset items vendor_buyback #playername
Delete all items in the selected player\'s vendor buyback tab.
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.'),

('reset items all',  'Syntax : .reset items all #playername
Delete all items in the selected player\'s inventory (equipped, in bags, in bank, in keyring, in currency list and in vendor buy back tab).
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.'),

('reset items allbags', 'Syntax : .reset items allbags #playername
Delete all items in the selected player\'s inventory (equipped, in bags, in bank, in keyring, in currency list and in vendor buy back tab)
This command also deletes the bags.
#playername : Optional target player name (if player is online only). If not provided the command will execute on the selected target player.');
