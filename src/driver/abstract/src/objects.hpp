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

# ifndef GNUDO_ABSTRACT_OBJECTS_HPP
# define GNUDO_ABSTRACT_OBJECTS_HPP


# include <string>
# include <ctime>


namespace gnudo
{
	namespace abstract
	{
		using std::string;
		using std::time_t;
		
		
		namespace managers
		{
			template <typename O>
			class Manager;
			class TasksManager;
			class PriorityLevelsManager;
		}


		namespace objects
		{
			using namespace managers;


			template <class P>
			class Child
			{
				public:
					Child(P *parent): __parent(parent) {};
					P	*getParent() const;

				private:
					P	*__parent;
			};


			// P è il manager per questo oggetto
			template <class P>
			class Object: private Child<P>
			{
				public:
					Object(P *parentManager, int64_t id): Child<P>(parentManager), __id(id) {}
					P			*getParentManager() const;
					int64_t 	getId() const;

				private:
					const int64_t __id;

			};


			class PriorityLevel: public Object<PriorityLevelsManager>
			{
				public:
					PriorityLevel(PriorityLevelsManager *parentManager, int64_t id): Object(parentManager, id) {}

					virtual string		getName() const = 0;
					virtual int64_t		getLevel() const = 0;
					virtual string 		getColor() const = 0;

					virtual void		setName(const string name) = 0;
					virtual void		setColor(const string color) = 0;
			};


			class Task: public Object<TasksManager>
			{
				public:
					Task(TasksManager *parentManager, int64_t id): Object(parentManager, id) {}
					
					virtual string			getTitle() const = 0;
					virtual string			getDescription() const = 0;
					virtual PriorityLevel 	*getPriorityLevel() const = 0;
					virtual time_t			getCreationTime() const = 0;
					virtual time_t			getModificationTime() const = 0;
					virtual bool			isCompleted() const = 0;

					virtual void 			setTitle(const string title) = 0;
					virtual void			setDescription(const string description) = 0;
					virtual void			setPriorityLevel(const int64_t priority) = 0;
					virtual void    		setCreationTime(const time_t time) = 0;
					virtual void    		setModificationTime(const time_t time) = 0;
					virtual void 			setStatus(const bool isCompleted) = 0;
			};
		}
		
		
		// IMPLEMENTAZIONE
		
		
		template <class P> P *
		objects::Child<P>::getParent() const
		{
			return __parent;
		};
		
		template <class P> P *
		objects::Object<P>::getParentManager() const 
		{
			return this->getParent();
		}
		
		template <class P> int64_t 
		objects::Object<P>::getId() const 
		{
			return __id;
		}
	}
}







# endif // ifndef GNUDO_ABSTRACT_OBJECTS_HPP
