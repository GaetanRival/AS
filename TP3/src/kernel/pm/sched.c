/*
 * Copyright(C) 2011-2016 Pedro H. Penna   <pedrohenriquepenna@gmail.com>
 *              2015-2016 Davidson Francis <davidsondfgl@hotmail.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nanvix. If not, see <http://www.gnu.org/licenses/>.
 */

#include <nanvix/clock.h>
#include <nanvix/const.h>
#include <nanvix/hal.h>
#include <nanvix/pm.h>
#include <signal.h>


unsigned lfsr;
unsigned int sum;
unsigned int elected;
unsigned int max;
char done;
unsigned int curr;
unsigned int initialization=0;
/**
 * @brief Schedules a process to execution.
 *
 * @param proc Process to be scheduled.
 */
PUBLIC void sched(struct process *proc)
{
	proc->state = PROC_READY;
	proc->counter = 0;
}

/**
 * @brief Stops the current running process.
 */
PUBLIC void stop(void)
{
	curr_proc->state = PROC_STOPPED;
	sndsig(curr_proc->father, SIGCHLD);
	yield();
}

/**
 * @brief Resumes a process.
 *
 * @param proc Process to be resumed.
 *
 * @note The process must stopped to be resumed.
 */
PUBLIC void resume(struct process *proc)
{
	/* Resume only if process has stopped. */
	if (proc->state == PROC_STOPPED)
		sched(proc);
}

float pow(float x, int y){
	if(y<=0){
		return 1;
	}
	float r=1;
	for(int i=0;i<y;i++){
		r=r*x;
	}
	return r;
}

PUBLIC int prio_Scheduling (struct process *p,struct process *next){
	if((p->priority*10 - p->counter) < (next->priority*10 - next->counter)){
			return 1;
		}
		else if((p->priority*10 - p->counter) == (next->priority*10 - next->counter)){
			if(p->nice < next->nice){
				return 1;
			}
		}
		return 0;
}

PUBLIC int mult_Queues(struct process *p, struct process *next){
		if (p->priority/10 > next->priority/10){
			return 0;
		}
		else{
			 if (p->priority/10 < next->priority/10){
	 			return 1;
			}
			else{
					return prio_Scheduling(p,next);
				}
		 }
}

PUBLIC int lotterie(struct process *p){
		if(done||elected==0){
			return 0;
		}
		curr+=p->counter;
		if(curr>=elected){
			done=1;
			return 1;
		}
		return 0;
}



unsigned int rand2(){
	char bit;
	bit=((lfsr>>0)^(lfsr >>2)^(lfsr>>3)^(lfsr>>5))&1;
	lfsr=(lfsr>>1)|(bit<<15);
	return lfsr;
}

PUBLIC void tickets(){

	int max=0;
	sum=0;
	struct process *p;
	for(p=FIRST_PROC;p<=LAST_PROC;p++){
		if(p->state != PROC_READY){
			continue;
		}
		if(p->priority>max){
			max=p->priority;
		}
	}
	for(p=FIRST_PROC;p<= LAST_PROC;p++){
		if(p->state != PROC_READY){
			continue;
		}
		p->counter=pow(2,max-(p->priority));
		sum+=p->counter;
	}


}

/**
 * @brief Yields the processor.
 */
PUBLIC void yield(void)
{
	struct process *p;    /* Working process.     */
	struct process *next; /* Next process to run. */

	/* Re-schedule process for execution. */
	if (curr_proc->state == PROC_RUNNING)
		sched(curr_proc);

	/* Remember this process. */
	last_proc = curr_proc;


	/* Check alarm. */
	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip invalid processes. */
		if (!IS_VALID(p))
			continue;

		/* Alarm has expired. */
		if ((p->alarm) && (p->alarm < ticks))
			p->alarm = 0, sndsig(p, SIGALRM);
	}

	/* Choose a process to run next. */
	next = IDLE;


	if(!initialization){
		lfsr=CURRENT_TIME;
		initialization=1;
	}

	tickets();
	curr=0;
	done=0;
	if(sum>0){
		elected=(rand2()%sum)+1;
	}else{
		elected=0;
	}


	for (p = FIRST_PROC; p <= LAST_PROC; p++)
	{
		/* Skip non-ready process. */
		if (p->state != PROC_READY)
			continue;

		/*
		 * Process with higher
		 * waiting time found.
		 */


		 /* Ordre Robin-Round*/
		//if (p->counter > next->counter)

		/* Priority Scheduling */
		//if (prio_Scheduling(p,next))

		/* Multiple Queues */
		//if (mult_Queues(p,next))

		/* Lotterie */
		if (lotterie(p))
		{
			next->counter++;
			next = p;
		}


		/*
		 * Increment waiting
		 * time of process.
		 */
		else
			p->counter++;
	}

	/* Switch to next process. */
	next->priority = PRIO_USER;
	next->state = PROC_RUNNING;
	next->counter = PROC_QUANTUM*pow(1.5,next->priority); //Puissance pour eviter une famine
	if (curr_proc != next)
		switch_to(next);
}
