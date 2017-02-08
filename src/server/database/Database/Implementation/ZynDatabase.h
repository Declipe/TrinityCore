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

#ifndef _ZYNDATABASE_H
#define _ZYNDATABASE_H

#include "DatabaseWorkerPool.h"
#include "MySQLConnection.h"

enum ZynDatabaseStatements
{
	MAX_ZYNDATABASE_STATEMENTS
};

class TC_DATABASE_API ZynDatabaseConnection : public MySQLConnection
{
public:
	typedef ZynDatabaseStatements Statements;

	//- Constructors for sync and async connections
	ZynDatabaseConnection(MySQLConnectionInfo& connInfo) : MySQLConnection(connInfo) { }
	ZynDatabaseConnection(ProducerConsumerQueue<SQLOperation*>* q, MySQLConnectionInfo& connInfo) : MySQLConnection(q, connInfo) { }

	//- Loads database type specific prepared statements
	void DoPrepareStatements() override;
};

typedef DatabaseWorkerPool<ZynDatabaseConnection> ZynDatabaseWorkerPool;
//typedef DatabaseWorkerPool<ZynDatabaseConnection> ZynDatabaseWorkerPool;
#endif
