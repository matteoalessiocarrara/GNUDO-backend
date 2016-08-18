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

// Oggetti che gestiscono una lista di oggetti (in objects.hpp)

# ifndef GNUDO_BACKEND_MANAGERS_HPP
# define GNUDO_BACKEND_MANAGERS_HPP

# include <stdexcept>
# include <cstdint>
# include <vector>
# include <string>
# include <ctime>

# include <sqlite3pp.hpp>

# include "objects.hpp"


namespace gnudo
{
	class Db;


	namespace managers
	{
		using std::int64_t;
		using std::vector;
		using std::string;
		using std::time_t;
		using std::time;

		using namespace objects;


		namespace exceptions
		{
			class TaskNotFoundException: public std::runtime_error
			{
				public:
					TaskNotFoundException(const std::string &msg): std::runtime_error(msg) {}
			};

			class PriorityAlreadyExistingException: public std::runtime_error
			{
				public:
					PriorityAlreadyExistingException(const std::string &msg): std::runtime_error(msg) {}
			};

			class CannotRemovePriorityException: public std::runtime_error
			{
				public:
					CannotRemovePriorityException(const std::string &msg): std::runtime_error(msg) {}
			};

			class PriorityNotFoundException: public std::runtime_error
			{
				public:
					PriorityNotFoundException(const std::string &msg): std::runtime_error(msg) {}
			};
		}


		// Gestisce una lista di oggetti
		// Il parametro del template è il tipo di oggetto gestito
		template <typename O>
		class Manager: private Child<Db>, public sqlite3pp::objects::Table
		{
			public:
				Manager(sqlite3 *sqlitedb, Db *parentDb, const string tableName, const string defaultOrderByColumn, const bool defaultListAscending=false);

				Db					*getParentDb() const;
				virtual O			getObject(const int64_t id) const = 0;
				O					operator[](size_t index) const;

			private:
				const string		__defaultOrderByColumn;
				bool				__defaultListAscending;
		};


		class TasksManager: public Manager<Task>
		{
			public:
				TasksManager(sqlite3 *db, Db *gnudoDb);


				int64_t 			add(const int64_t priority, const char *title = "Untitled", const char *description = "", const time_t creationTime = time(NULL),
										const time_t modificationTime = time(NULL), const bool isCompleted = false);
				Task				getObject(const int64_t id) const;
				Task 				getTask(const int64_t id) const;
		};


		class PriorityLevelsManager: public Manager<PriorityLevel>
		{
			public:
				PriorityLevelsManager(sqlite3 *db, Db *gnudoDb);

				int64_t 			add(const char *name, const int64_t priority, const char *color = "#00ff00");
				PriorityLevel		getObject(const int64_t id) const;
				PriorityLevel		getPriorityLevel(const int64_t id) const;
				// WARNING Utilizzare questo al posto del remove di sqlite3, prima di rimuovere un livello di priorità devono
				// essere modificati i task collegati
				void 				remove(const int64_t id, int64_t moveToPriority);
		};
	}
}


# endif // GNUDO_BACKEND_MANAGERS_HPP
