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

// Oggetti di base

# ifndef GNUDO_BACKEND_OBJECTS_HPP
# define GNUDO_BACKEND_OBJECTS_HPP

# include <cstdint>
# include <string>
# include <ctime>

# include "../sqlite3pp/src/sqlite3pp/sqlite3pp.hpp"


namespace gnudo
{
	namespace managers
	{
		class TasksManager;
		class PriorityLevelsManager;
	}


	namespace objects
	{
		using std::int64_t;
		using std::string;
		using std::time_t;

		using namespace managers;


		template <class P>
		class Child
		{
			public:
				Child(P *parent);
				P	*getParent() const;

			private:
				P	*__parent;
		};


		// P è il manager per questo oggetto
		template <class P>
		class Object: private Child<P>
		{
			public:
				Object(P *parentManager, int64_t id); // initlist
				P			*getParentManager() const;
				int64_t 	getId() const;

			private:
				const int64_t __id;

		};


		class PriorityLevel: public sqlite3pp::objects::Row, public Object<PriorityLevelsManager>
		{
			public:
				PriorityLevel(sqlite3 *db, const int64_t id, PriorityLevelsManager *parentManager);

				string		getName() const;
				int64_t		getLevel() const;
				string 		getColor() const;

				void		setName(const char *name);
				void		setColor(const char *color);
		};


		class Task: public sqlite3pp::objects::Row, public Object<TasksManager>
		{
			public:
				Task(const int64_t id, sqlite3 *db, TasksManager *parentManager);

				string			getTitle() const;
				string			getDescription() const;
				PriorityLevel	getPriorityLevel() const;
				time_t 			getCreationTime() const;
				time_t 			getModificationTime() const;
				bool 			isCompleted() const;

				void 			setTitle(const char *title);
				void 			setDescription(const char *description);
				void			setPriorityLevel(const int64_t priority);
				void			setCreationTime(const time_t time);
				void 			setModificationTime(const time_t time);
				void 			setStatus(const bool isCompleted);
		};


		// Implementazioni


		template <class P>
		Child<P>::Child(P *parent)
		{
			__parent = parent;
		}


		template <class P> P *
		Child<P>::getParent() const
		{
			return __parent;
		}


		template <class P>
		Object<P>::Object(P *parentManager, int64_t id): Child<P>(parentManager), __id(id)
		{

		}


		template <class P> P *
		Object<P>::getParentManager() const
		{
			return this->getParent();
		}


		template <class P> int64_t
		Object<P>::getId() const
		{
			return __id;
		}

	}
}


# endif // GNUDO_BACKEND_OBJECTS_HPP


