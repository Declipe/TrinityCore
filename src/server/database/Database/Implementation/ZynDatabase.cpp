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

    PrepareStatement(ZynDatabase1, "SELECT `enchant_id`, `prev_enchant_id`, `golds` FROM `world_item_upgrade`", CONNECTION_SYNCH);
    PrepareStatement(ZynDatabase2, "SELECT SpellId, SpellMask, RequiredClassMask, RequiredRaceMask, RequiredLevel, RequiredSpellId, RequiredSkillId, RequiredSkillValue FROM `world_autolearn`", CONNECTION_SYNCH);
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
