/*
	Header file h2o.h

@author Lai Nhat Tuan Truong
 */
#ifndef H2O_H
#define H2O_H

#define OXYGEN 0
#define HYDROGEN 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> // isdigit()

#include <semaphore.h>
#include <unistd.h> // fork(), usleep
#include <sys/wait.h> // waitpid()
#include <sys/types.h> // Shared
#include <sys/shm.h>    //  Memory
#include <signal.h> // SIGKILL
#include <time.h> // rand()

/**
 * Shared memory struct
 */
typedef struct
{
  int N;
  int B;
  int actions;
  int hydrocount; // hydrogen atom number
  int oxycount; // oxygen number
  char *filename;
  FILE *fp;
  bool bondingready; // trigger for bonding
  int oxy;
  int hydro;
  int molecules; // total molecules formed
  int beginbond;
} TShmem;

/**
 * Semaphores struct
 */
typedef struct
{
  sem_t molcheck;
  sem_t oxybonding;
  sem_t hydrobonding;
  sem_t oxybonded;
  sem_t hydrobonded;
  sem_t finish;
  sem_t finishsig;
  sem_t startsync;
  sem_t bondsync;
} TShsem;

/**
 * Enum for errors
 */
typedef enum
{
    E_OK=0,
    E_PARAMETER=1,
    E_FILE,
    E_SHMEM,
    E_SHSEM,
    E_FORK,
} EError;

/**
 * Input arguments struct
 */
typedef struct 
{
    int N;
    int GO;
    int GH;
    int B;
    EError error;
} TArg;

void oxygen(TShmem *shmemory, TShsem *semaphore); 
void hydrogen(TShmem *shmemory, TShsem *semaphore);
void bond(TShmem *shmemory, TShsem *semaphore, int ATOM, int atomcount); // print bonded
void finish(TShmem *shmemory, TShsem *semaphore, int ATOM, int atomcount); // print finished
TArg save_arguments(int argc, char *argv[]);  // save program arguments
EError print_error(EError code); // print error code
void shmem_free(int shmemid, TShmem *shmemory); // free memory
void shsem_free(int shsemid, TShsem *shsemaphore); // destroy semaphores


#endif