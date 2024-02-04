-- Delete Record
DELETE FROM `world`.`creature_classlevelstats`
WHERE `level` > 80;


-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @basehp0 = 5342;
SET @basehp1 = 9215;
SET @basehp2 = 12600;
SET @basemana = 0;
SET @basearmor = 9729;
SET @attackpower = 642;
SET @rangedattackpower = 103;
SET @damage_base = 47.2377;
SET @damage_exp1 = 131.186;
SET @damage_exp2 = 164.924;

-- Duplicate the data for the new levels
INSERT IGNORE INTO `world`.`creature_classlevelstats` (
    `level`,
    `class`,
    `basehp0`,
    `basehp1`,
    `basehp2`,
    `basemana`,
    `basearmor`,
    `attackpower`,
    `rangedattackpower`,
    `damage_base`,
    `damage_exp1`,
    `damage_exp2`,
    `comment`
)
SELECT
    tl.`level`,
    1 AS `class`,
    (@basehp0 * POW(1.01, tl.`level` - @startLevel)) AS `basehp0`,
    (@basehp1 * POW(1.01, tl.`level` - @startLevel)) AS `basehp1`,
    (@basehp2 * POW(1.01, tl.`level` - @startLevel)) AS `basehp2`,
    (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`,
    (@basearmor * POW(1.01, tl.`level` - @startLevel)) AS `basearmor`,
    (@attackpower * POW(1.01, tl.`level` - @startLevel)) AS `attackpower`,
    (@rangedattackpower * POW(1.01, tl.`level` - @startLevel)) AS `rangedattackpower`,
    (@damage_base * POW(1.01, tl.`level` - @startLevel)) AS `damage_base`,
    (@damage_exp1 * POW(1.01, tl.`level` - @startLevel)) AS `damage_exp1`,
    (@damage_exp2 * POW(1.01, tl.`level` - @startLevel)) AS `damage_exp2`,
    NULL AS `comment`
FROM
    (SELECT @startLevel + n AS `level`
    FROM (
        SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
        FROM (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS units
        CROSS JOIN (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS tens
        CROSS JOIN (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS hundreds
        WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
    ) AS levels) AS tl;

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @basehp0 = 4274;
SET @basehp1 = 7373;
SET @basehp2 = 12600;
SET @basemana = 3994;
SET @basearmor = 9706;
SET @attackpower = 608;
SET @rangedattackpower = 80;
SET @damage_base = 44.2013;
SET @damage_exp1 = 112.345;
SET @damage_exp2 = 164.924;

-- Duplicate the data for the new levels
INSERT IGNORE INTO `world`.`creature_classlevelstats` (
    `level`,
    `class`,
    `basehp0`,
    `basehp1`,
    `basehp2`,
    `basemana`,
    `basearmor`,
    `attackpower`,
    `rangedattackpower`,
    `damage_base`,
    `damage_exp1`,
    `damage_exp2`,
    `comment`
)
SELECT
    tl.`level`,
    2 AS `class`,
    (@basehp0 * POW(1.01, tl.`level` - @startLevel)) AS `basehp0`,
    (@basehp1 * POW(1.01, tl.`level` - @startLevel)) AS `basehp1`,
    (@basehp2 * POW(1.01, tl.`level` - @startLevel)) AS `basehp2`,
    (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`,
    (@basearmor * POW(1.01, tl.`level` - @startLevel)) AS `basearmor`,
    (@attackpower * POW(1.01, tl.`level` - @startLevel)) AS `attackpower`,
    (@rangedattackpower * POW(1.01, tl.`level` - @startLevel)) AS `rangedattackpower`,
    (@damage_base * POW(1.01, tl.`level` - @startLevel)) AS `damage_base`,
    (@damage_exp1 * POW(1.01, tl.`level` - @startLevel)) AS `damage_exp1`,
    (@damage_exp2 * POW(1.01, tl.`level` - @startLevel)) AS `damage_exp2`,
    NULL AS `comment`
FROM
    (SELECT @startLevel + n AS `level`
    FROM (
        SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
        FROM (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS units
        CROSS JOIN (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS tens
        CROSS JOIN (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS hundreds
        WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
    ) AS levels) AS tl;

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @basehp0 = 5342;
SET @basehp1 = 9215;
SET @basehp2 = 12600;
SET @basemana = 0;
SET @basearmor = 9706;
SET @attackpower = 642;
SET @rangedattackpower = 103;
SET @damage_base = 47.2377;
SET @damage_exp1 = 131.186;
SET @damage_exp2 = 164.924;

-- Duplicate the data for the new levels
INSERT IGNORE INTO `world`.`creature_classlevelstats` (
    `level`,
    `class`,
    `basehp0`,
    `basehp1`,
    `basehp2`,
    `basemana`,
    `basearmor`,
    `attackpower`,
    `rangedattackpower`,
    `damage_base`,
    `damage_exp1`,
    `damage_exp2`,
    `comment`
)
SELECT
    tl.`level`,
    4 AS `class`,
    (@basehp0 * POW(1.01, tl.`level` - @startLevel)) AS `basehp0`,
    (@basehp1 * POW(1.01, tl.`level` - @startLevel)) AS `basehp1`,
    (@basehp2 * POW(1.01, tl.`level` - @startLevel)) AS `basehp2`,
    (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`,
    (@basearmor * POW(1.01, tl.`level` - @startLevel)) AS `basearmor`,
    (@attackpower * POW(1.01, tl.`level` - @startLevel)) AS `attackpower`,
    (@rangedattackpower * POW(1.01, tl.`level` - @startLevel)) AS `rangedattackpower`,
    (@damage_base * POW(1.01, tl.`level` - @startLevel)) AS `damage_base`,
    (@damage_exp1 * POW(1.01, tl.`level` - @startLevel)) AS `damage_exp1`,
    (@damage_exp2 * POW(1.01, tl.`level` - @startLevel)) AS `damage_exp2`,
    NULL AS `comment`
FROM
    (SELECT @startLevel + n AS `level`
    FROM (
        SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
        FROM (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS units
        CROSS JOIN (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS tens
        CROSS JOIN (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS hundreds
        WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
    ) AS levels) AS tl;

-- Set the starting level and maximum level
SET @startLevel = 80;
SET @maxLevel = 255;

-- Get the base values for the starting level
SET @basehp0 = 3739;
SET @basehp1 = 6478;
SET @basehp2 = 10080;
SET @basemana = 8814;
SET @basearmor = 7822;
SET @attackpower = 287;
SET @rangedattackpower = 69;
SET @damage_base = 40.099;
SET @damage_exp1 = 129.377;
SET @damage_exp2 = 152.555;

-- Duplicate the data for the new levels
INSERT IGNORE INTO `world`.`creature_classlevelstats` (
    `level`,
    `class`,
    `basehp0`,
    `basehp1`,
    `basehp2`,
    `basemana`,
    `basearmor`,
    `attackpower`,
    `rangedattackpower`,
    `damage_base`,
    `damage_exp1`,
    `damage_exp2`,
    `comment`
)
SELECT
    tl.`level`,
    8 AS `class`,
    (@basehp0 * POW(1.01, tl.`level` - @startLevel)) AS `basehp0`,
    (@basehp1 * POW(1.01, tl.`level` - @startLevel)) AS `basehp1`,
    (@basehp2 * POW(1.01, tl.`level` - @startLevel)) AS `basehp2`,
    (@basemana * POW(1.01, tl.`level` - @startLevel)) AS `basemana`,
    (@basearmor * POW(1.01, tl.`level` - @startLevel)) AS `basearmor`,
    (@attackpower * POW(1.01, tl.`level` - @startLevel)) AS `attackpower`,
    (@rangedattackpower * POW(1.01, tl.`level` - @startLevel)) AS `rangedattackpower`,
    (@damage_base * POW(1.01, tl.`level` - @startLevel)) AS `damage_base`,
    (@damage_exp1 * POW(1.01, tl.`level` - @startLevel)) AS `damage_exp1`,
    (@damage_exp2 * POW(1.01, tl.`level` - @startLevel)) AS `damage_exp2`,
    NULL AS `comment`
FROM
    (SELECT @startLevel + n AS `level`
    FROM (
        SELECT units.a + 10 * tens.a + 100 * hundreds.a AS n
        FROM (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS units
        CROSS JOIN (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS tens
        CROSS JOIN (
            SELECT 0 AS a UNION SELECT 1 UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5 UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9
        ) AS hundreds
        WHERE (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) > @startLevel AND (@startLevel + units.a + 10 * tens.a + 100 * hundreds.a) <= @maxLevel
    ) AS levels) AS tl;
