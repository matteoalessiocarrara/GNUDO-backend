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

# include "managers.hpp"


using namespace gnudo::abstract::managers;
using namespace gnudo::abstract;


int64_t
TasksManager::add(const int64_t priority, const string title, const string description, const time_t creationTime,
							const time_t modificationTime, const bool isCompleted)
{
	bool priorityFound = false;
	
	for(const auto &pId: getParentDb()->getPriorityLevels()->getIdList())
	{
		if (pId == priority)
		{
			priorityFound = true;
			break;
		}
	}
	
	if (not priorityFound)
		throw exceptions::PriorityNotFoundException(to_string(priority));
	
	_add(priority, title, description, creationTime, modificationTime, isCompleted);
}

int64_t 
PriorityLevelsManager::add(const string name, const int64_t priority, const string color)
{
	for(const auto & id : getIdList())
		if(id == priority) throw exceptions::PriorityAlreadyExistingException(to_string(priority));
		
		return _add(name, priority, color);
}

void 
PriorityLevelsManager::remove(const int64_t id)
{
	for(const auto & taskId : getParentDb()->getTasks()->getIdList())
	{
		Task *tmp = getParentDb()->getTasks()->getTask(taskId);
		
		if(tmp->getPriorityLevel()->getId() == id)
			throw exceptions::CannotRemovePriorityException("Il task " + to_string(taskId) + " dipende da questa priorità (" +
			to_string(id) + ")");
		
		delete tmp;
	}
	
	_remove(id);
}

void 
PriorityLevelsManager::remove(const int64_t id, int64_t moveToPriority)
{
	for(const auto & taskId : getParentDb()->getTasks()->getIdList())
	{
		Task *tmp = getParentDb()->getTasks()->getTask(taskId);
		
		if(tmp->getPriorityLevel()->getId() == id)
			tmp->setPriorityLevel(moveToPriority);
		
		delete tmp;
	}
	
	_remove(id);
}




