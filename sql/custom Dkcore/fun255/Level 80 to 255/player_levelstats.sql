-- Delete Record
DELETE FROM `world`.`player_levelstats`
WHERE `level` > 80;
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 2;
SET @class = 9;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 6;
SET @class = 3;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 7;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 6;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 11;
SET @class = 5;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 1;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 3;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 7;
SET @class = 8;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 5;
SET @class = 4;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 10;
SET @class = 4;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 11;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 1;
SET @class = 2;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 4;
SET @class = 4;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 3;
SET @class = 2;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 8;
SET @class = 8;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 7;
SET @class = 9;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 8;
SET @class = 7;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 7;
SET @class = 4;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 8;
SET @class = 3;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 10;
SET @class = 5;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 1;
SET @class = 4;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 4;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 5;
SET @class = 5;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 8;
SET @class = 4;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 1;
SET @class = 5;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 3;
SET @class = 3;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 3;
SET @class = 5;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 3;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 4;
SET @class = 5;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 6;
SET @class = 7;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 1;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 11;
SET @class = 3;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 10;
SET @class = 3;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 1;
SET @class = 8;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 10;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 1;
SET @class = 9;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 11;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 3;
SET @class = 4;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 2;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 4;
SET @class = 3;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 11;
SET @class = 2;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 5;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 6;
SET @class = 11;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 8;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 10;
SET @class = 8;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 2;
SET @class = 3;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 5;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 5;
SET @class = 8;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 2;
SET @class = 4;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 10;
SET @class = 9;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 5;
SET @class = 9;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 8;
SET @class = 5;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 11;
SET @class = 7;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 6;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 2;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 11;
SET @class = 8;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 7;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 2;
SET @class = 7;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 8;
SET @class = 6;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 10;
SET @class = 2;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 4;
SET @class = 1;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @race = 4;
SET @class = 11;
SET @str = (SELECT `str` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @agi = (SELECT `agi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @sta = (SELECT `sta` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @inte = (SELECT `inte` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);
SET @spi = (SELECT `spi` FROM `world`.`player_levelstats` WHERE `level` = @startLevel AND `race` = @race AND `class` = @class);

-- Generate the rows with calculated values
INSERT IGNORE INTO `world`.`player_levelstats` (`race`, `class`, `level`, `str`, `agi`, `sta`, `inte`, `spi`)
SELECT @race AS `race`,
       @class AS `class`,
       tl.`level` AS `level`,
       ROUND((@str * POW(1.01, tl.`level` - @startLevel)), 2) AS `str`,
       ROUND((@agi * POW(1.01, tl.`level` - @startLevel)), 2) AS `agi`,
       ROUND((@sta * POW(1.01, tl.`level` - @startLevel)), 2) AS `sta`,
       ROUND((@inte * POW(1.01, tl.`level` - @startLevel)), 2) AS `inte`,
       ROUND((@spi * POW(1.01, tl.`level` - @startLevel)), 2) AS `spi`
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
-- ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
