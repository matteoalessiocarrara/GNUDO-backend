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

# include <sqlite3.h>

# include "objects.hpp"


namespace gnudo
{
	namespace abstract
	{
		using std::vector;
		using std::string;
		

		class TasksManager
		{
			public:
				
				enum Order
				{
					TITLE = 0,
					DESCRIPTION = 1,
					CREATION_TIME = 2,
					MODIFICATION_TIME = 3,
					COMPLETED = 4
				};
				
                virtual sqlite3_int64 			add(const string title, const string description, const time_t creationTime,
                                                    const time_t modificationTime, const bool completed) = 0;
				virtual void 					remove(const Task* task) = 0;
				virtual Task* 					getTask(const sqlite3_int64 id) = 0;
				// TODO Implementare funzione protected generica per l'ordinamento, non tutti i db hanno l'istruzione "ORDER BY"
				virtual vector<sqlite3_int64>	getIdList(Order orderBy, bool asc) const = 0; 
		};
	}
}


# endif // ifndef GNUDO_ABSTRACT_MANAGERS_HPP
