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

# include <string>
# include <stdexcept>

# include "../lib/sqlite3pp/src/sqlite3pp.hpp"

# include "dbdefs.hpp"
# include "gnudo.hpp"


using namespace sqlite3pp::functions;
using namespace gnudo::managers;
using namespace gnudo::dbdefs;
using namespace gnudo;

using std::string;


Db::Db(const char *filename)
{
	sqlite3pp_open(filename, &__sqlitedb);

	__createTables();

	__tasksManager = new TasksManager(__sqlitedb, this);
	__priorityLevels = new PriorityLevelsManager(__sqlitedb, this);
}


Db::~Db()
{
	sqlite3pp_close(__sqlitedb);

	delete __tasksManager;
	delete __priorityLevels;
}


TasksManager *
Db::getTasks()
{
	return __tasksManager;
}


PriorityLevelsManager *
Db::getPriorityLevels()
{
	return __priorityLevels;
}


void
Db::__createTables()
{
	char *err = NULL;
	const string sql = \
			"BEGIN TRANSACTION;"
			"PRAGMA foreign_keys = ON;"

			"CREATE TABLE  IF NOT EXISTS " + tables::priorityLevels +
			"("
			+ columns::prioritylevel::priority + " INTEGER PRIMARY KEY,"
			+ columns::prioritylevel::name + "	TEXT NOT NULL,"
			+ columns::prioritylevel::color + " TEXT NOT NULL"
			");"

			"CREATE TABLE  IF NOT EXISTS " + tables::tasks +
			"("
			+ columns::task::id + " INTEGER PRIMARY KEY,"
			+ columns::task::title	+ "	TEXT NOT NULL,"
			+ columns::task::description + " TEXT,"
			+ columns::task::creationTime + " INTEGER NOT NULL,"
			+ columns::task::modificationTime + " INTEGER NOT NULL,"
			+ columns::task::completed + " INTEGER NOT NULL,"
			+ columns::task::priority + " INTEGER NOT NULL,"
			"FOREIGN KEY(" + columns::task::priority + ") REFERENCES "  + tables::priorityLevels + "(" + columns::prioritylevel::priority + ") ON DELETE RESTRICT"
			");"

			"END TRANSACTION;";

	sqlite3pp_exec(__sqlitedb, sql.c_str(), NULL, NULL, &err);

	if(err != NULL)
		throw std::runtime_error(err);
}




