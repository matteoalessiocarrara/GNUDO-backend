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
# include <cstdint>
# include <sstream>
# include <iostream>
# include <stdexcept>

# include "../sqlite3pp/src/sqlite3pp/sqlite3pp.hpp"

# include "gnudo.hpp"
# include "dbdefs.hpp"
# include "managers.hpp"
# include "objects.hpp"


using namespace sqlite3pp::functions;
using namespace gnudo::objects;
using namespace gnudo::dbdefs;
using namespace gnudo;

using std::int64_t;
using std::string;
using std::time_t;
using std::cout;
using std::endl;
using std::time;


PriorityLevel::PriorityLevel(sqlite3* db, const int64_t id, PriorityLevelsManager *parentManager):
	sqlite3pp::objects::Row(db, tables::priorityLevels, id), Object(parentManager, id)
{
}


string
PriorityLevel::getName() const
{
	return (const char*)getColumnString(columns::prioritylevel::name);
}


int64_t
PriorityLevel::getLevel() const
{
	return Object<PriorityLevelsManager>::getId();
}


string
PriorityLevel::getColor() const
{
	return (const char*)getColumnString(columns::prioritylevel::color);
}


void
PriorityLevel::setName(const char *name)
{
	sqlite3_stmt *ppStmt = getUpdateStatement(columns::prioritylevel::name);

	sqlite3pp_bind_text(ppStmt, 1, name, strlen(name) + 1, SQLITE_STATIC);
	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);
}


void
PriorityLevel::setColor(const char *color)
{
	// TODO Controllare validità
	sqlite3_stmt *ppStmt = getUpdateStatement(columns::prioritylevel::color);

	sqlite3pp_bind_text(ppStmt, 1, color, strlen(color) + 1, SQLITE_STATIC);
	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);
}


// WARNING	Ogni metodo che modifica questo task deve chiamare setModificationTime()
// A parte setModificationTime ovviamente XD


Task::Task(const int64_t id, sqlite3* db, TasksManager *parentManager):
	sqlite3pp::objects::Row(db, tables::tasks, id), Object(parentManager, id)
{

}


string
Task::getTitle() const
{
	return (const char*)getColumnString(columns::task::title);
}


string
Task::getDescription() const
{
	return (const char*)getColumnString(columns::task::description);
}


PriorityLevel
Task::getPriorityLevel() const
{
	int64_t id = getColumn<sqlite3_int64>(columns::task::priority, sqlite3pp_column_int64);
	return PriorityLevel(sqlite3pp::objects::Row::getParentDb(), id, getParentManager()->getParentDb()->getPriorityLevels());
}


time_t
Task::getCreationTime() const
{
	return getColumn<sqlite3_int64>(columns::task::creationTime, sqlite3pp_column_int64);
}


time_t
Task::getModificationTime() const
{
	return getColumn<sqlite3_int64>(columns::task::modificationTime, sqlite3pp_column_int64);
}


bool 
Task::isCompleted() const
{
	return getColumn<int>(columns::task::completed, sqlite3pp_column_int);
}


void
Task::setTitle(const char *title)
{
	sqlite3_stmt *ppStmt = getUpdateStatement(columns::task::title);

	sqlite3pp_bind_text(ppStmt, 1, title, strlen(title) + 1, SQLITE_STATIC);
	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);

	setModificationTime(time(NULL));
}


void
Task::setDescription(const char *description)
{
	sqlite3_stmt *ppStmt = getUpdateStatement(columns::task::description);

	sqlite3pp_bind_text(ppStmt, 1, description, strlen(description) + 1, SQLITE_STATIC);
	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);
	
	setModificationTime(time(NULL));
}


void
Task::setPriorityLevel(const int64_t priority)
{
	sqlite3_stmt *ppStmt = getUpdateStatement(columns::task::priority);

	sqlite3pp_bind_int64(ppStmt, 1, priority);
	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);
	
	setModificationTime(time(NULL));
}


void
Task::setStatus(const bool isCompleted)
{
	sqlite3_stmt *ppStmt = getUpdateStatement(columns::task::completed);

	sqlite3pp_bind_int(ppStmt, 1, isCompleted);
	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);
	
	setModificationTime(time(NULL));
}


void
Task::setCreationTime(const time_t time)
{
	sqlite3_stmt *ppStmt = getUpdateStatement(columns::task::creationTime);

	sqlite3pp_bind_int64(ppStmt, 1, time);
	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);
}


void
Task::setModificationTime(const time_t time)
{
	sqlite3_stmt *ppStmt = getUpdateStatement(columns::task::modificationTime);

	sqlite3pp_bind_int64(ppStmt, 1, time);
	sqlite3pp_step(ppStmt);
	sqlite3_finalize(ppStmt);
}

