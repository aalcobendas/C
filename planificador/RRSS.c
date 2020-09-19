#include <stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>
#include "my_io.h"

//#include "mythread.h"
#include "interrupt.h"

#include "queue.h"

int ticks;
TCB* scheduler();
void activator();
void timer_interrupt(int sig);
void disk_interrupt(int sig);
//CREAR LISTA DE LISTOS
struct queue *lista_alta;
struct queue *lista_baja;

/* Array of state thread control blocks: the process allows a maximum of N threads */
static TCB t_state[N];

/* Current running thread */
static TCB* running;
static int current = 0;
 /* Running anterior */
 static TCB* old_running;

/* Variable indicating if the library is initialized (init == 1) or not (init == 0) */
static int init=0;

/* Thread control block for the idle thread */
static TCB idle;

static void idle_function()
{
  while(1);
}

void function_thread(int sec)
{
    //time_t end = time(NULL) + sec;
    while(running->remaining_ticks)
    {
      //do something
    }
    mythread_exit();
}


/* Initialize the thread library */
void init_mythreadlib()
{
  lista_alta = queue_new();
  lista_baja = queue_new();
  int i;
  ticks=0;
  /* Create context for the idle thread */
  if(getcontext(&idle.run_env) == -1)
  {
    perror("*** ERROR: getcontext in init_thread_lib");
    exit(-1);
  }

  idle.state = IDLE;
  idle.priority = SYSTEM;
  idle.function = idle_function;
  idle.run_env.uc_stack.ss_sp = (void *)(malloc(STACKSIZE));
  idle.tid = -1;

  if(idle.run_env.uc_stack.ss_sp == NULL)
  {
    printf("*** ERROR: thread failed to get stack space\n");
    exit(-1);
  }

  idle.run_env.uc_stack.ss_size = STACKSIZE;
  idle.run_env.uc_stack.ss_flags = 0;
  idle.ticks = QUANTUM_TICKS;
  makecontext(&idle.run_env, idle_function, 1);

  t_state[0].state = INIT;
  t_state[0].priority = LOW_PRIORITY;
  t_state[0].ticks = QUANTUM_TICKS;

  if(getcontext(&t_state[0].run_env) == -1)
  {
    perror("*** ERROR: getcontext in init_thread_lib");
    exit(5);
  }

  for(i=1; i<N; i++)
  {
    t_state[i].state = FREE;
  }

  t_state[0].tid = 0;
  running = &t_state[0];
  old_running=running;

  /* Initialize disk and clock interrupts */
  init_disk_interrupt();
  init_interrupt();
}


/* Create and intialize a new thread with body fun_addr and one integer argument */
int mythread_create (void (*fun_addr)(),int priority,int seconds)
{
  int i;

  if (!init) { init_mythreadlib(); init=1;}

  for (i=0; i<N; i++)
    if (t_state[i].state == FREE) break;

  if (i == N) return(-1);

  if(getcontext(&t_state[i].run_env) == -1)
  {
    perror("*** ERROR: getcontext in my_thread_create");
    exit(-1);
  }

  t_state[i].state = INIT;
  t_state[i].priority = priority;
  t_state[i].function = fun_addr;
  t_state[i].ticks= QUANTUM_TICKS;
  t_state[i].execution_total_ticks = seconds_to_ticks(seconds);
  t_state[i].remaining_ticks = t_state[i].execution_total_ticks;
  t_state[i].run_env.uc_stack.ss_sp = (void *)(malloc(STACKSIZE));

  if(t_state[i].run_env.uc_stack.ss_sp == NULL)
  {
    printf("*** ERROR: thread failed to get stack space\n");
    exit(-1);
  }

  t_state[i].tid = i;
  t_state[i].run_env.uc_stack.ss_size = STACKSIZE;
  t_state[i].run_env.uc_stack.ss_flags = 0;
  makecontext(&t_state[i].run_env, fun_addr,2,seconds);
  TCB *t= &t_state[i];
  if(t->priority==HIGH_PRIORITY && running->priority==LOW_PRIORITY){
    running->ticks=QUANTUM_TICKS;
    running->state=INIT;
    disable_interrupt();
    disable_disk_interrupt();
    enqueue(lista_baja,running);
    enable_disk_interrupt();
    enable_interrupt();
    old_running=running;
    running=t;
    running->state=RUNNING;
    printf("*** THREAD  %d PREEMTED: SETCONTEXT  OF %d\n",old_running->tid, running->tid);
    activator(running);
    return i;
  }
  if(t->priority==HIGH_PRIORITY && running->priority==HIGH_PRIORITY){
    if(t->remaining_ticks<running->remaining_ticks){
      running->state=INIT;
      running->execution_total_ticks=running->remaining_ticks;
      disable_interrupt();
      disable_disk_interrupt();
      sorted_enqueue(lista_alta, running, running->remaining_ticks);
      enable_disk_interrupt();
      enable_interrupt();
      old_running=running;
      running=t;
      running->state=RUNNING;
      activator(running);
      return i;
    }
  }
  if(t->priority==LOW_PRIORITY){
    disable_interrupt();
    disable_disk_interrupt();
    enqueue(lista_baja,t);
    enable_disk_interrupt();
    enable_interrupt();
  }else if(t->priority==HIGH_PRIORITY){
    disable_interrupt();
    disable_disk_interrupt();
    sorted_enqueue(lista_alta, t, t->remaining_ticks);
    enable_disk_interrupt();
    enable_interrupt();

  }
  return i;
}
/****** End my_thread_create() ******/


/* Read disk syscall */
int read_disk()
{
/*  TCB *old;
  int ret;
  ret=data_in_page_cache();
  if(ret!=0){
    //poner el proceso con estado WAITING
    running->state=WAITING;
    //insertar proceso "lista de bloqueados" --> disco

    old=running;
    running=scheduler();
    activator(old->run_env,running->run_env);
  }
  */
   return 1;
}

/* Disk interrupt  */
void disk_interrupt(int sig)
{
  //buscar en la lista de bloqueados(anterior) al proceso
  //cambio el estado de ese proceso de WAITING a READY
  //e insertar en lista de listos

}


/* Free terminated thread and exits */
void mythread_exit() {
  int tid = mythread_gettid();

  printf("*** THREAD %d FINISHED\n", tid);
  t_state[tid].state = FREE;
  free(t_state[tid].run_env.uc_stack.ss_sp);
  old_running = running;
  running = scheduler();
  activator(running);
}


void mythread_timeout(int tid) {

    printf("*** THREAD %d EJECTED\n", tid);
    t_state[tid].state = FREE;
    free(t_state[tid].run_env.uc_stack.ss_sp);
    old_running=running;
    running = scheduler();
    activator(running);
}


/* Sets the priority of the calling thread */
void mythread_setpriority(int priority)
{
  int tid = mythread_gettid();
  t_state[tid].priority = priority;
  if(priority ==  HIGH_PRIORITY){
    t_state[tid].remaining_ticks = 195;
  }
}

/* Returns the priority of the calling thread */
int mythread_getpriority(int priority)
{
  int tid = mythread_gettid();
  return t_state[tid].priority;
}


/* Get the current thread id.  */
int mythread_gettid(){
  if (!init) { init_mythreadlib(); init=1;}
  return running->tid;
}


/* SJF para alta prioridad, RR para baja*/

TCB* scheduler()
{
  if(queue_empty(lista_alta)!=1){
    TCB *q;
    disable_interrupt();
    disable_disk_interrupt();
    q=dequeue(lista_alta);
    enable_disk_interrupt();
    enable_interrupt();
    return (q);
  }
  if(queue_empty(lista_baja)!=1){
    TCB *p;
    disable_interrupt();
    disable_disk_interrupt();
    p=dequeue(lista_baja);
    enable_disk_interrupt();
    enable_interrupt();
    return (p);
}
    printf("mythread_free: No thread in the system\nExiting...\n");
    printf("*** FINISH\n");
    exit(1);
}

/* Timer interrupt */
void timer_interrupt(int sig)//PARA QUE SIRVE SIG= signal
{
  ticks++;
  running->remaining_ticks--;
  if(running->priority==LOW_PRIORITY){
    running->ticks--;
    if(running->ticks==0){
      running->state=INIT;
      running->ticks=QUANTUM_TICKS;
      //INSERTAR AL FINAL DE LISTA
      disable_interrupt();
      disable_disk_interrupt();
      enqueue(lista_baja,running);
      enable_disk_interrupt();
      enable_interrupt();
      old_running=running;
      running=scheduler();
      running->state=RUNNING;
      activator(running);
    }
  }
}

/* Activator */
void activator(TCB *next)
{
  if(old_running==running)return;
  if(old_running->state==FREE){
    printf("*** THREAD %d TERMINATED: SETCONTEXT OF %d\n", old_running->tid, next->tid);
   setcontext(&(next->run_env));
   printf("mythread_free: After setcontext, should never get here!!...\n");
  }else{
    printf("*** SWAPCONTEXT FROM %d TO %d\n",old_running->tid, running->tid);
    swapcontext(&(old_running->run_env),&(running->run_env));
  }
}
