



SET @GOB_Entry := 600110;
SET @GOB_Name := "must be in quotes";
SET @GOB_Display := 8111;
SET @Event_scriptID := 600110;
SET @MAP := 0;
SET @X := 3616.979980;
SET @Y := -2820.746826;
SET @Z := 177.172394;
SET @O := 1.866468;

INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `size`, `Data2`) 
VALUES (@GOB_Entry, 10, @GOB_Display, @GOB_Name, 1, @Event_scriptID);

INSERT INTO `event_scripts` (`id`, `command`, `datalong`, `x`, `y`, `z`, `o`) 
VALUES (@Event_scriptID, 6, @MAP, @X, @Y, @Z, @O);
