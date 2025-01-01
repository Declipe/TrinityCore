/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ZynDatabase.h"
#include "MySQLPreparedStatement.h"

void ZynDatabaseConnection::DoPrepareStatements()
{
    if (!m_reconnecting)
        m_stmts.resize(MAX_ZYNDATABASE_STATEMENTS);

    PrepareStatement(ZynDatabase1, "SELECT `enchant_id`, `prev_enchant_id`, `description`, `golds` FROM `world_item_upgrade`", CONNECTION_SYNCH);
    PrepareStatement(ZynDatabase2, "SELECT SpellId, SpellMask, RequiredClassMask, RequiredRaceMask, RequiredLevel, RequiredSpellId, RequiredSkillId, RequiredSkillValue FROM `world_autolearn`", CONNECTION_SYNCH);
    PrepareStatement(ZYN_SEL_CUSTOM_CONFIG, "SELECT `OptionName`, `Type`, `DefaultValue`, `CustomValue` FROM custom_config", CONNECTION_SYNCH);
    PrepareStatement(ZYN_INS_ITEM_FROM_SHOP, "INSERT INTO item_shop (id, entryID, groupName, coinCost) VALUES (?, ?, ?, ?)", CONNECTION_ASYNC);
    PrepareStatement(ZYN_DEL_ITEM_FROM_SHOP, "DELETE FROM item_shop WHERE entryID = ? AND groupName = ?", CONNECTION_ASYNC);

    PrepareStatement(ZYN_INS_PROMO_CODE, "INSERT INTO promotion_codes (id, collection, code, honor, arena, money, item_1, item_2, item_3, item_4, item_5, item_6, item_7, item_8, item_9, item_10, item_count_1, item_count_2, item_count_3, item_count_4, item_count_5, item_count_6, item_count_7, item_count_8, item_count_9, item_count_10, aura, spell_1, spell_2, spell_3, coin, count_of_exists) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_ASYNC);
    PrepareStatement(ZYN_DEL_PROMO_CODE, "DELETE FROM promotion_codes WHERE code = ?", CONNECTION_ASYNC);
    PrepareStatement(ZYN_UPD_PROMO_CODE_USED, "UPDATE promotion_codes SET count_of_exists = ? WHERE id = ?", CONNECTION_ASYNC);
    PrepareStatement(ZYN_INS_PROMO_CODE_HISTORY, "INSERT INTO promotion_codes_history (id, codeID, code, accountID, playerID, use_time, use_unix_time) VALUES (?, ?, ?, ?, ?, NOW(), ?)", CONNECTION_ASYNC);

}
ZynDatabaseConnection::ZynDatabaseConnection(MySQLConnectionInfo& connInfo) : MySQLConnection(connInfo)
{
}

ZynDatabaseConnection::ZynDatabaseConnection(ProducerConsumerQueue<SQLOperation*>* q, MySQLConnectionInfo& connInfo) : MySQLConnection(q, connInfo)
{
}

ZynDatabaseConnection::~ZynDatabaseConnection()
{
}
