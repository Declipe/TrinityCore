-- Delete Record
DELETE FROM `world`.`pet_levelstats`
WHERE `level` > 80;
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 1;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 25566;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 26101;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 575;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 15352;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 417;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 8477;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 8996;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 510;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 5766;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 15438;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 15214;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 5058;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 14385;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 17252;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 26125;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 3939;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 329;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 19668;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 22362;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 3450;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 10928;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 6250;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 24476;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 24656;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 1863;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 416;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 12922;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 24815;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 25553;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 1860;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @creature_entry = 10979;

-- Fetch the base values for the starting level
SET @hp = (SELECT `hp` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @mana = (SELECT `mana` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @armor = (SELECT `armor` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @str = (SELECT `str` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @agi = (SELECT `agi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @sta = (SELECT `sta` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @inte = (SELECT `inte` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @spi = (SELECT `spi` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @min_dmg = (SELECT `min_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);
SET @max_dmg = (SELECT `max_dmg` FROM `world`.`pet_levelstats` WHERE `creature_entry` = @creature_entry AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`pet_levelstats` (`creature_entry`, `level`, `hp`, `mana`, `armor`, `str`, `agi`, `sta`, `inte`, `spi`, `min_dmg`, `max_dmg`)
SELECT @creature_entry AS `creature_entry`,
       tl.`level` AS `level`,
       (@hp * POW(1.01, tl.`level` - @startLevel)) AS `hp`,
       (@mana * POW(1.01, tl.`level` - @startLevel)) AS `mana`,
       (@armor * POW(1.01, tl.`level` - @startLevel)) AS `armor`,
       (@str * POW(1.01, tl.`level` - @startLevel)) AS `str`,
       (@agi * POW(1.01, tl.`level` - @startLevel)) AS `agi`,
       (@sta * POW(1.01, tl.`level` - @startLevel)) AS `sta`,
       (@inte * POW(1.01, tl.`level` - @startLevel)) AS `inte`,
       (@spi * POW(1.01, tl.`level` - @startLevel)) AS `spi`,
       (@min_dmg * POW(1.01, tl.`level` - @startLevel)) AS `min_dmg`,
       (@max_dmg * POW(1.01, tl.`level` - @startLevel)) AS `max_dmg`
FROM
  (SELECT @startLevel + n AS `level`
   FROM
     (SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
      FROM
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS units
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS tens
      CROSS JOIN
        (SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9) AS hundreds
      WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
     ) AS levels) AS tl;


-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------