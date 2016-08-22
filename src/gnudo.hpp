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


// Header principale

// TODO Rimuovere include/using inutili
// TODO Riutilizzare sqlite3_stmt


# ifndef GNUDO_BACKEND_GNUDO_HPP
# define GNUDO_BACKEND_GNUDO_HPP


# include <string>
# include <sqlite3pp.hpp>

# include "managers.hpp"


namespace gnudo
{
	using std::string;
	using namespace managers;


	class Db
	{
		public:
			Db(const char *filename);
			~Db();

			TasksManager			*getTasks();
			PriorityLevelsManager	*getPriorityLevels();

		private:
			void 					__createTables();

			sqlite3					*__sqlitedb;
			TasksManager			*__tasksManager;
			PriorityLevelsManager	*__priorityLevels;
	};
}


# endif // GNUDO_BACKEND_GNUDO_HPP
