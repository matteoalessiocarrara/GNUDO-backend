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

# include "gnudo.hpp"
# include "managers.hpp"


using namespace gnudo::abstract;


void
PriorityLevelsManager::remove(const int64_t id, int64_t moveToPriority)
{

	vector<int64_t> tasks = gnudo::abstract::Manager::getParentDb()->getTasks()->getIdList(gnudo::abstract::TasksManager::Order::CREATION_TIME);

	for(vector<int64_t>::iterator i = tasks.begin(); i != tasks.end(); i++)
	{	Task *tmp = gnudo::abstract::Manager::getParentDb()->getTasks()->getTask(*i);

		if (tmp->getPriorityLevel()->getId() == id)
			tmp->setPriorityLevel(moveToPriority);

		delete tmp;
	}

	gnudo::abstract::Manager::remove(id);
}
