/*
 * Copyright(C) 2011-2016 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *
 * This file is part of Nanvix.
 *
 * Nanvix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nanvix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SEM_H_
#define SEM_H_


	/**
	 * @brief Comand values for semaphores.
	 */
	/**@{*/
	#define GETVAL   0 /**< Returns the value of a semaphore. */
	#define SETVAL   1 /**< Sets the value of a semaphore.    */
	#define IPC_RMID 3 /**< Destroys a semaphore.            */
	/**@}*/


	//Definir liste
	typedef struct Elt{
			struct process *ID;
			struct Elt *suivant;
	} Element;

	//Definir structure sempahore
	typedef struct Sem{
		int x;
		struct process *tabWait[64];
	} Semaphore;

	/* Forward definitions. */
	extern int sys_semget(unsigned);
	extern int sys_semctl(int, int, int);
	extern int sys_semop(int, int);

	extern void addp(struct Sem,struct process);
	extern void removep(struct Sem);



//ajouter un element en fin de liste






#endif /* SEM_H_ */
