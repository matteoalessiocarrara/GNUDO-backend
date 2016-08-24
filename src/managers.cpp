/*
 * Copyright 2016 Matteo Alessio Carrara <sw.matteoac@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

# include <ctime>
# include <string>
# include <cstring>
# include <cstdint>
# include <iostream>
# include <stdexcept>

# include "../lib/sqlite3pp/lib/sqlite3/src/sqlite3.h"
# include "../lib/sqlite3pp/src/sqlite3pp.hpp"

# include "gnudo.hpp"
# include "dbdefs.hpp"
# include "objects.hpp"
# include "managers.hpp"


using namespace gnudo::managers::exceptions;
using namespace sqlite3pp::functions;
using namespace gnudo::managers;
using namespace gnudo::objects;
using namespace gnudo::dbdefs;
using namespace gnudo;

using std::to_string;
using std::int64_t;
using std::string;
using std::vector;
using std::strlen;
using std::time;


TasksManager::TasksManager(sqlite3 *db, Db *gnudoDb): Manager(db, gnudoDb, tables::tasks, columns::task::priority)
{
}



int64_t
TasksManager::add(const int64_t priority, const char *title, const char *description, const time_t creationTime,
							const time_t modificationTime, const bool isCompleted)
{
	sqlite3_stmt	*ppStmt;
	const string	sql	=	"INSERT INTO " + tables::tasks + " "
							"("
							+ columns::task::title + ", "
							+ columns::task::description + ", "
							+ columns::task::creationTime + ", "
							+ columns::task::modificationTime + ", "
							+ columns::task::priority + ", "
							+ columns::task::completed +
							")"
							"VALUES (?, ?, ?, ?, ?, ?);";


	// Se non esiste solleverà un eccezione
	Manager<Task>::getParentDb()->getPriorityLevels()->getPriorityLevel(priority);


	sqlite3pp_prepare_v2(sqlite3pp::objects::Table::getParentDb(), sql.c_str(), sql.size() + 1, &ppStmt, NULL);

	sqlite3pp_bind_text(ppStmt, 1, title, strlen(title) + 1, SQLITE_STATIC);
	sqlite3pp_bind_text(ppStmt, 2, description, strlen(description) + 1, SQLITE_STATIC);
	sqlite3pp_bind_int(ppStmt, 3, creationTime);
	sqlite3pp_bind_int(ppStmt, 4, modificationTime);
	sqlite3pp_bind_int(ppStmt, 5, priority);
	sqlite3pp_bind_int(ppStmt, 6, isCompleted);

	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);


	return sqlite3pp_last_insert_rowid(sqlite3pp::objects::Table::getParentDb());
}


Task
TasksManager::getObject(const int64_t id) const
{
	if (not isValidId(id))
		throw TaskNotFoundException(to_string(id));

	return Task(id, sqlite3pp::objects::Table::getParentDb(), (TasksManager*)this);
}


Task
TasksManager::getTask(const int64_t id) const
{
	return getObject(id);
}


PriorityLevelsManager::PriorityLevelsManager(sqlite3* db, Db *gnudoDb):
	Manager<PriorityLevel>(db, gnudoDb, tables::priorityLevels, columns::prioritylevel::priority)
{
}


int64_t
PriorityLevelsManager::add(const char *name, const int64_t priority, const char *color)
{
	sqlite3_stmt	*ppStmt;
	const string	sql	=	"INSERT INTO " + tables::priorityLevels + " "
							"("
							+ columns::prioritylevel::name + ", "
							+ columns::prioritylevel::priority + ", "
							+ columns::prioritylevel::color +
							") "
							"VALUES (?, ?, ?);";

	try
	{
		getObject(priority);
		throw exceptions::PriorityAlreadyExistingException(to_string(priority));
	}
	catch (exceptions::PriorityNotFoundException &e) // Ok
	{
		sqlite3pp_prepare_v2(sqlite3pp::objects::Table::getParentDb(), sql.c_str(), sql.size() + 1, &ppStmt, NULL);

		sqlite3pp_bind_text(ppStmt, 1, name, strlen(name) + 1, SQLITE_STATIC);
		sqlite3pp_bind_int64(ppStmt, 2, priority);
		sqlite3pp_bind_text(ppStmt, 3, color, strlen(color) + 1, SQLITE_STATIC);

		sqlite3pp_step(ppStmt);
		sqlite3_finalize(ppStmt);
	}

	return sqlite3pp_last_insert_rowid(sqlite3pp::objects::Table::getParentDb());
}


PriorityLevel
PriorityLevelsManager::getObject(const int64_t id) const
{
	if (not isValidId(id))
		throw PriorityNotFoundException(to_string(id));
	
	return PriorityLevel(sqlite3pp::objects::Table::getParentDb(), id, (PriorityLevelsManager*)this);
}


PriorityLevel
PriorityLevelsManager::getPriorityLevel(const int64_t id) const
{
	return getObject(id);
}


void
PriorityLevelsManager::remove(const int64_t id, int64_t moveToPriority)
{
	for(const auto &taskId: getParentDb()->getTasks()->getIdList())
	{
		Task tmp = getParentDb()->getTasks()->getTask(taskId);

		if(tmp.getPriorityLevel().Object<PriorityLevelsManager>::getId() == id)
			tmp.setPriorityLevel(moveToPriority);
	}

	sqlite3pp::objects::Table::remove(id);
}


