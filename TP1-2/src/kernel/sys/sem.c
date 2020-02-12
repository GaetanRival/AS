#include <sys/sem.h>
#include <nanvix/const.h>
#include <nanvix/fs.h>
#include <nanvix/pm.h>
#include <errno.h>
#include <unistd.h>
#include <nanvix/syscall.h>


Semaphore tabSem[64]; //Fixe la taille du tableau de sempahore à 64

unsigned int curkey=1;

Semaphore* create (int n){

	int i=0;
	while(tabSem[i].key!=0){
		i++;
	}
	if(i<64){
		tabSem[i].key=curkey;
		tabSem[i].val=n;
		curkey++;
		return &tabSem[i];
	}
	else{
		return NULL;
	}


}

void down(Semaphore *sem){
  if(sem->val > 0){
    sem->val--;
  }
  else{
    sleep(sem->tabWait,5);
  }
}

void up(Semaphore *sem){
  if(sem->val==0){
    if(sem->tabWait[0]!=NULL){
			sem->tabTmp[0]=sem->tabWait[0]; //Selectionne le premier processus à reveiller
			for(int y=0;y<63;y++){	//Decalage et mettre à jour le tableau des processus en attente
				sem->tabWait[y]=sem->tabWait[y+1];
			}
			sem->tabWait[63]=NULL;
      wakeup(sem->tabTmp);	// Reveille l'unique processus selectionné
    }
    else{
      sem->val++;
    }
  }

}

void destroy(Semaphore *sem){
  for(int i=0;i<64;i++){
    sem->tabWait[i]=NULL;
  }
	sem->key=0;
}

int sys_semget(unsigned key){
  int i=0;
  while(i<64){
    if(tabSem[i].key==key){
    	return i;
    }
		i++;
  }
	if(i>=64){
		Semaphore* sem= create(1);
		sem->key=key;
		int ID=0;
		while(tabSem[ID].key!=key){
			ID++;
		}
		return ID;
	}
	return -1;
}

int sys_semctl(int semid, int cmd, int val){
	switch(cmd){
		case GETVAL:
			return tabSem[semid].val;
		case SETVAL:
			tabSem[semid].val=val;
			return 0;
		case IPC_RMID:
			destroy(&tabSem[semid]);
			return 0;
		default:
			break;
	}
  return -1;
}

int sys_semop(int semid, int op){
	if(op>0){
		up(&tabSem[semid]);
		return 0;
	}
	if(op<0){
		down(&tabSem[semid]);
		return 0;
	}
	else{
		return -1;
	}
}
