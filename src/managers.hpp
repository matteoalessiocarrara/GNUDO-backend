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

# ifndef GNUDO_ABSTRACT_MANAGERS_HPP
# define GNUDO_ABSTRACT_MANAGERS_HPP


# include <vector>
# include <string>
# include <cstdint>
# include <stdexcept>

# include <sqlite3.h>

# include "objects.hpp"


namespace gnudo
{
	namespace abstract
	{
		using std::vector;
		using std::string;
		using std::int64_t;
		
		class Db;


		namespace exceptions
		{
			class ObjectNotFoundException: public std::runtime_error
			{
				public:
					ObjectNotFoundException(const std::string& msg): std::runtime_error(msg){}
			};
		}
		

		class Manager: private Child<Db>
		{
			public:				
				Manager(Db *parentDb);
				Db	*getParentDb() const {return getParent();};

				// TODO Implementare funzione protected generica per l'ordinamento, non tutti i db hanno l'istruzione "ORDER BY"
				virtual vector<int64_t>	getIdList(int orderBy, bool ascending=false) const = 0;
				virtual void			remove(const int64_t id) = 0;

		};


		class TasksManager: public Manager
		{
			public:
				
				// getIdList
				enum Order
				{
					TITLE = 0,
					DESCRIPTION = 1,
					CREATION_TIME = 2,
					MODIFICATION_TIME = 3,
					COMPLETED = 4,
					PRIORITY = 5
				};
				
								TasksManager(Db *parentDb);
				virtual int64_t	add(const int priorityId, const string title, const string description, const time_t creationTime,
									const time_t modificationTime, const bool completed) = 0;
				virtual Task* 	getTask(const int64_t id) const = 0;
		};
		
		
		class PriorityLevelsManager: public Manager
		{
			public:
				
				// getIdList
				enum Order
				{
					NAME = 0,
					PRIORITY = 1,
					COLOR = 2
				};
								PriorityLevelsManager(Db *parentDb);
				
				// TODO cambiare priority in unsigned
				virtual int64_t		 	add(const string name, const int priority, const string color) = 0; // TODO Possono esistere due con la stessa priorità?
				virtual PriorityLevel*	getPriorityLevel(const int64_t id) const = 0;
				void 			remove(const int64_t id, int64_t moveToPriority);
		};
	}
}


# endif // ifndef GNUDO_ABSTRACT_MANAGERS_HPP
