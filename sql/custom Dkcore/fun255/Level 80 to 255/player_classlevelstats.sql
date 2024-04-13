-- Delete Record
DELETE FROM `player_classlevelstats`
WHERE `level` > 80;

-- ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @class = 1;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 2;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 3;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 4;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 5;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 6;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 7;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 8;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 9;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
SET @class = 11;
SET @basehp = (SELECT `basehp` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);
SET @basemana = (SELECT `basemana` FROM `player_classlevelstats` WHERE `class` = @class AND `level` = @startLevel);

-- Generate the rows with calculated values
INSERT IGNORE INTO `player_classlevelstats` (`class`, `level`, `basehp`, `basemana`)
SELECT @class AS `class`,
       tl.`level` AS `level`,
       (@basehp * POW(1.01, tl.`level` - @startLevel)) AS `basehp`,
       (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`
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
