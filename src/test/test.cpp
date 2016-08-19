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
 * MA 02110-1PRIORITY_TEST01, USA.
 */

# include <iostream>
# include <vector>

# include <gnudo-backend/dbdefs.hpp>
# include <gnudo-backend/gnudo.hpp>


using namespace gnudo;
using std::vector;
using std::cout;
using std::endl;


int main()
{	

# define PRIORITY_TEST 3

	Db db("test.db");

	db.getPriorityLevels()->add("Importante", PRIORITY_TEST, "#ff0000");
	db.getTasks()->add(PRIORITY_TEST, "Test titolo", "Test descrizione");

	cout << "Ci sono " << db.getPriorityLevels()->getIdList().size() << " livelli di priorità" << endl;
	cout << "Il primo si chiama '" << db.getPriorityLevels()->getPriorityLevel(PRIORITY_TEST).getName() << "', è di colore '" <<
			db.getPriorityLevels()->getPriorityLevel(PRIORITY_TEST).getColor() << "'' e di livello " << db.getPriorityLevels()->getPriorityLevel(PRIORITY_TEST).getLevel() << endl << endl;

	cout << "I task invece sono " << db.getTasks()->getIdList().size() << endl;
	cout << "***Primo task***" << endl;
	cout << "Titolo: " << db.getTasks()->getTask(db.getTasks()->getIdList()[0]).getTitle() << endl;
	cout << "Descrizione: " << db.getTasks()->getTask(db.getTasks()->getIdList()[0]).getDescription() << endl;
	cout << "Data di creazione: " << db.getTasks()->getTask(db.getTasks()->getIdList()[0]).getCreationTime() << endl;
	cout << "Data di modifica: " << db.getTasks()->getTask(db.getTasks()->getIdList()[0]).getModificationTime() << endl;
	cout << "Priorità: " << db.getTasks()->getTask(db.getTasks()->getIdList()[0]).getPriorityLevel().getLevel() << endl;
	cout << "Completato: " << db.getTasks()->getTask(db.getTasks()->getIdList()[0]).isCompleted() << endl << endl;

	cout << "Eliminazione in corso" << endl;

	db.getTasks()->remove(db.getTasks()->getIdList()[0]);
	db.getPriorityLevels()->sqlite3pp::objects::Table::remove(db.getPriorityLevels()->getIdList()[0]);

	cout << "Adesso ci sono " << db.getPriorityLevels()->getIdList().size() <<
			" livelli di priorità e " << db.getTasks()->getIdList().size() <<
			" tasks" << endl;

	return 0;
}
