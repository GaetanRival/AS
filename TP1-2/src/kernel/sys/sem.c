#include <sys/sem.h>
#include <nanvix/const.h>
#include <nanvix/fs.h>
#include <nanvix/pm.h>
#include <errno.h>
#include <unistd.h>
// #include <nanvix/syscall.h>

EXTERN int sys_kill(pid_t pid, int sig);

Semaphore tabSem[64];
int nbSem=0;

void create (int n){
  Semaphore sem;
  sem.x=n;
  tabSem[nbSem]=sem;
  nbSem++;
}

void down(Semaphore *sem){
  if(sem->x > 0){
    sem->x --;
  }
  else{
    sleep(sem->tabWait,5);
  }
}

void up(Semaphore *sem){
  if(sem==NULL){
    if(sem->tabWait[0]!=NULL){
      wakeup(sem->tabWait);
    }
    else{
      sem->x++;
    }
  }

}

///////////////Faire un GIT//////////////////////////////

void destroy(Semaphore *sem){
  for(int i=0;i<64;i++){
    sys_kill(sem->tabWait[i]->pid,9);
  }
}

int semget(unsigned key){
  return 0;
}

int semctl(int semid, int cmd, int val){
  return 0;
}

int semop(int semid, int op){
  return 0;
}


// void addp(struct Sem s, struct process p){
//   if(s.tete==NULL)
//     s.tete->ID = &p;
//   else{
//     struct Elt* tmp = s.tete;
//   	while(tmp->suivant != NULL){
//   		tmp=tmp->suivant;
//   	}
//   	tmp->suivant->ID = &p;
//   }
// }
//
// void removep(struct Sem s){
//   if(s.tete!=NULL){
//     if(s.tete->suivant!=NULL){
//   	   struct Elt *curr = s.tete->suivant;
//   	    s.tete->suivant = curr->suivant;
//     }
//     else{
//       s.tete=NULL;
//     }
//   }
//}
