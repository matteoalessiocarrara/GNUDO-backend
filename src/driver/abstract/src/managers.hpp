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


# include <ctime>
# include <vector>
# include <string>
# include <cstdint>
# include <stdexcept>

# include "gnudo.hpp"
# include "objects.hpp"


namespace gnudo
{
	namespace abstract
	{
		using std::time;
		using std::time_t;
		using std::vector;
		using std::string;
		using std::int64_t;
		using std::size_t;
		using std::to_string;
		

		namespace exceptions
		{
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


		namespace managers
		{
			using namespace objects;


			// Gestisce una lista di oggetti
			// Il parametro del template è il tipo di oggetto gestito
			template <typename O>
			class Manager: private Child<Db>
			{
				public:
					// defaultListOrder e defualtListAscending verranno usati come parametri di getIdList in operator[]()
					Manager(Db *parentDb, const int defaultListOrder, const bool defaultListAscending);
					Db							*getParentDb() const;
					vector<int64_t> 			getIdList();
					
					// TODO Implementare funzione generica per l'ordinamento, non tutti i db hanno l'istruzione "ORDER BY"
					virtual vector<int64_t>		getIdList(const int orderBy, const bool ascending) const = 0;
					virtual O					*getObject(const int64_t id) const = 0;
					virtual void				remove(const int64_t id) = 0;

					O 				*operator[](size_t index) const;

				private:
					int __defaultListOrder;
					bool __defaultListAscending;
			};


			class TasksManager: public Manager<Task>
			{
				public:

					// Ordinamento della lista dei task
					enum Order
					{
					    TITLE = 0,
					    DESCRIPTION = 1,
					    CREATION_TIME = 2,
					    MODIFICATION_TIME = 3,
					    COMPLETED = 4,
					    PRIORITY = 5,
					    ID = 6
					};

					TasksManager(Db *parentDb): Manager<Task>(parentDb, Order::PRIORITY, false) {};
					int64_t 			add(const int64_t priority, const string title = "Untitled", const string description = "", const time_t creationTime = time(NULL),
											const time_t modificationTime = time(NULL), const bool isCompleted = false);
					Task 				*getTask(const int64_t id) const {return getObject(id);}
					
				protected:
					virtual int64_t		_add(const int64_t priority, const string title, const string description, const time_t creationTime,
											const time_t modificationTime, const bool isCompleted) = 0;

			};


			class PriorityLevelsManager: public Manager<PriorityLevel>
			{
				public:

					// Ordinamento della lista delle priorità
					enum Order
					{
					    NAME = 0,
					    PRIORITY = 1, // Priorità e id sono sinonimi
					    COLOR = 2
					};

					PriorityLevelsManager(Db *parentDb): Manager<PriorityLevel>(parentDb, Order::PRIORITY, false) {};
					int64_t 			add(const string name, const int64_t priority, const string color = "#00ff00");
					PriorityLevel		*getPriorityLevel(const int64_t id) const {return getObject(id);};

					// WARNING Questo metodo deve essere usato solo quando si è sicuri che non ci siano task dipendenti dal livello di priorità
					void 				remove(const int64_t id);
					void 				remove(const int64_t id, int64_t moveToPriority);

				protected:
					virtual int64_t		_add(const string name, const int64_t priority, const string color) = 0;
					virtual void		_remove(const int64_t id) = 0;
			};
		}
		
		
		// IMPLEMENTAZIONE

		
		template <typename O>
		managers::Manager<O>::Manager(Db *parentDb, const int defaultListOrder, const bool defaultListAscending): Child<Db>(parentDb)
		{
			__defaultListOrder = defaultListOrder;
			__defaultListAscending = defaultListAscending;
		}
		
		template <typename O> Db *							
		managers::Manager<O>::getParentDb() const
		{
			return getParent();
		}
		
		template <typename O> vector<int64_t> 
		managers::Manager<O>::getIdList() 
		{
			return getIdList(__defaultListOrder, __defaultListAscending);	
		}
		
		template <typename O> O *
		managers::Manager<O>::operator[](size_t index) const
		{
			return getObject(getIdList(__defaultListOrder, __defaultListAscending)[index]);
		}

	}
}


# endif // ifndef GNUDO_ABSTRACT_MANAGERS_HPP
