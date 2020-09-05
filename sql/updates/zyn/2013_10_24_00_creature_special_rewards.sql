DROP TABLE IF EXISTS `creature_special_rewards`;
CREATE TABLE `creature_special_rewards` (
  `entry` int(11) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  `param1` int(11) unsigned NOT NULL,
  `param2` int(11) unsigned NOT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- There are four types
  -- CSR_TYPE_TITLE      = 0 
  -- CSR_TYPE_ITEM       = 1 killer->AddItem(reward.param1, reward.param2);
  -- CSR_TYPE_HONOR      = 2 killer->SetHonorPoints(killer->GetHonorPoints() + reward.param1);
  -- CSR_TYPE_ARENA      = 3  killer->SetArenaPoints(killer->GetArenaPoints() + reward.param1);