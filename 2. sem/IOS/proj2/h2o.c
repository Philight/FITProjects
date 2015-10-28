/*
 * h2o.c
 *
 *  Created on: 29.4.2015
 *      
sem_wait(*semaphore) - -1 ........ value(1)- decrement
sem_post(*semaphore) - +1
sem_init()

int shmget( key_t key, size_t  size, int  shmflg) - returns shmid
void *shmat(int shmid, const void *shmaddr, int shmflg); - if shmaddr == NULL - random memory segment
int shmdt(const void *shmaddr); param1 = *shmat return

filename = h2o.out

 */

#include "h2o.h"

//////////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------MAIN-------------------------------------------//
//////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    TShmem *shmemory;
    TShsem *shsemaphore;
    int shmemid;
    int shsemid;

    TArg input = save_arguments(argc, argv);
    if (input.error != E_OK)
    {
        print_error(input.error);
        exit (E_PARAMETER);
    }

    int N = input.N;
    int GO = input.GO;
    int GH = input.GH;
    srand(time(NULL));

/*******************
 * Shared memory
 */
    if ( (shmemid = shmget(IPC_PRIVATE, sizeof(TShmem), IPC_CREAT | 0666)) < 0 )
    {
        print_error(E_SHMEM);
        exit(2);
    }

    if ( (shmemory = (TShmem*) shmat(shmemid, NULL, 0)) == (void *) -1 )
    {
        shmem_free(shmemid, shmemory);

        print_error(E_SHMEM); 
        exit(2);
    }

    shmemory->B = input.B;
    shmemory->N = input.N;
    shmemory->actions = 1;
    shmemory->hydrocount = 1;
    shmemory->oxycount = 1;
    shmemory->bondingready = false;
    shmemory->beginbond = 0;

    shmemory->oxy = 0;
    shmemory->hydro = 0;
    shmemory->molecules = 0;

    shmemory->filename = "h2o.out";
    shmemory->fp = fopen(shmemory->filename,"w");
        if (shmemory->fp==NULL)
        {
           print_error(E_FILE);
           exit(2);
        }
    setbuf(shmemory->fp, NULL);
    
/**************************************************************
 * Shared semaphore
 */
    if ( (shsemid = shmget(IPC_PRIVATE, sizeof(TShsem), IPC_CREAT | 0666)) < 0 )
    {
        print_error(E_SHSEM);
        exit(2);
    }

    if ( (shsemaphore = (TShsem*) shmat(shsemid, NULL, 0)) == (void *) -1 )
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

//------------------ individual semaphore init

    if (sem_init (&shsemaphore->molcheck, 1, 1)) // 1 - shared, 1 - ready
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

    if (sem_init (&shsemaphore->oxybonding, 1, 0))
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

    if (sem_init (&shsemaphore->hydrobonding, 1, 0))
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

    if (sem_init (&shsemaphore->oxybonded, 1, 0))
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

    if (sem_init (&shsemaphore->hydrobonded, 1, 0))
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

    if (sem_init (&shsemaphore->finish, 1, 1))
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

    if (sem_init (&shsemaphore->finishsig, 1, 0))
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

    if (sem_init (&shsemaphore->startsync, 1, 1))
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

    if (sem_init (&shsemaphore->bondsync, 1, 0))
    {
        shsem_free(shsemid, shsemaphore);

        print_error(E_SHSEM); 
        exit(2);
    }

/**************************************************************
 * Main proc creates child processes
 */
    pid_t pid_main;
    pid_main=fork();

    pid_t oxy_id[N]; // waitpid for oxygen
    pid_t hydro_id[N*2]; //waitpid for hydrogen
    /*
     * Oxygen child proc generator
     */
    if (pid_main > 0)
    {
        pid_t pid_oxy;
        for (int o = 0; o < N; o++)
        {
            usleep (rand () % ((GO * 1000) + 1));
            pid_oxy = fork();
            if (pid_oxy > 0)    // store pid_oxy
                oxy_id[o]=pid_oxy;

            else if (pid_oxy == 0)      // oxygen
            {
                oxygen(shmemory, shsemaphore);
                return EXIT_SUCCESS;
            }
            else if (pid_oxy < 0) // oxy fork fail
            {
                for(int killed=0; killed < o; killed++)
                    kill(oxy_id[killed], SIGKILL);

                shmem_free(shmemid, shmemory);
                shsem_free(shsemid, shsemaphore);
                print_error(E_FORK); 
                exit(2);
            }
        }
    }
    /*
     * Hydrogen child proc generator
     */
    else if (pid_main == 0) 
    {
        pid_t pid_hydro;
        for (int h = 0; h < N*2; h++)
        {
            usleep (rand () % ((GH * 1000) + 1));
            pid_hydro = fork();
            if (pid_hydro > 0)  // store pid_hydro
                hydro_id[h] = pid_hydro;

            else if (pid_hydro == 0)        // hydrogen
            {
                hydrogen(shmemory,shsemaphore);
                return EXIT_SUCCESS;
            }
            else if (pid_hydro < 0) // hydro fork fail
            {
                for(int killed=0; killed < h; killed++)
                    kill(hydro_id[killed], SIGKILL);

                shmem_free(shmemid, shmemory);
                shsem_free(shsemid, shsemaphore);
                print_error(E_FORK); 
                exit(2);
            }
        }
    }
    else // main fork fail
    {
        shmem_free(shmemid, shmemory);
        shsem_free(shsemid, shsemaphore);
        print_error(E_FORK); 
        exit(2);
    }

    for(int hid = 0; hid < N*2; hid++)
        waitpid(hydro_id[hid], NULL, 0); // wait for hydro child processes to finish

    for(int oid = 0; oid < N; oid++)
        waitpid(oxy_id[oid], NULL, 0);  // wait for oxy child processes to finish

    fclose(shmemory->fp);
    shmem_free(shmemid, shmemory);
    shsem_free(shsemid, shsemaphore);
    return 0;
}

/**
 * Function for oxygen atoms
 */
void oxygen(TShmem *shmemory, TShsem *semaphore)
{
    sem_wait(&semaphore->startsync);
        int oxycount = shmemory->oxycount;
        int N = shmemory->N;
        FILE *fp = shmemory->fp; 

        fprintf (fp,"%d\t:O %d\t:started\n", shmemory->actions, oxycount);
        shmemory->actions++;
        shmemory->oxycount++;
    sem_post(&semaphore->startsync);

    sem_wait(&semaphore->molcheck);

        shmemory->oxy++; // oxy atom in queue
        if (shmemory->oxy >= 1 && shmemory->hydro >= 2) // all 3 atoms ready
            shmemory->bondingready = true;

        if(shmemory->bondingready == true) // last atom for bonding
        {
            fprintf (fp,"%d\t:O %d\t:ready\n",shmemory->actions, oxycount);
            shmemory->actions++;

            shmemory->bondingready = false;
            shmemory->oxy--;
            shmemory->hydro = shmemory->hydro - 2;

            sem_post(&semaphore->hydrobonding); // call other atoms 
            sem_post(&semaphore->hydrobonding);   // to start bonding

            bond(shmemory, semaphore, OXYGEN, oxycount);   // and start bonding

            sem_wait(&semaphore->hydrobonded); // wait for other atoms 
            sem_wait(&semaphore->hydrobonded);  // to be bonded
            shmemory->molecules++;  // molecule formed

            if (shmemory->molecules == N) // if last molecule created
            {
                for (int fin=0; fin < N*3; fin++)
                    sem_post(&semaphore->finishsig); // send signals to other proc to finish
                sem_wait(&semaphore->finishsig); // and wait
                finish(shmemory, semaphore, OXYGEN, oxycount);
            }
            else
            {
                sem_post(&semaphore->molcheck); // continue processes 
                sem_wait(&semaphore->finishsig);    // and wait for finish
                finish(shmemory, semaphore, OXYGEN, oxycount);
            }
        }
        else
        {
            fprintf (fp,"%d\t:O %d\t:waiting\n",shmemory->actions, oxycount);
            shmemory->actions++;

        sem_post(&semaphore->molcheck);

            sem_wait(&semaphore->oxybonding); // start bonding
            bond(shmemory, semaphore, OXYGEN, oxycount);
            sem_post(&semaphore->oxybonded); // done bonding

            sem_wait(&semaphore->finishsig);   // wait for finish
            finish(shmemory, semaphore, OXYGEN, oxycount);
        }
}

/**
 * Function for hydrogen atoms
 */
void hydrogen(TShmem *shmemory, TShsem *semaphore)
{
    sem_wait(&semaphore->startsync);
        int hydrocount = shmemory->hydrocount;
        int N = shmemory->N;
        FILE *fp = shmemory->fp; 
    
        fprintf (fp,"%d\t:H %d\t:started\n", shmemory->actions, hydrocount);
        shmemory->actions++;
        shmemory->hydrocount++;
    sem_post(&semaphore->startsync);
    
    sem_wait(&semaphore->molcheck);

        shmemory->hydro++; // hydro atom in queue
        if (shmemory->oxy >= 1 && shmemory->hydro >= 2) // all 3 atoms ready
            shmemory->bondingready = true;

        if(shmemory->bondingready == true) // last atom for bonding
        {
            fprintf (fp,"%d\t:H %d\t:ready\n",shmemory->actions, hydrocount);
            shmemory->actions++;

            shmemory->bondingready = false;
            shmemory->oxy--;
            shmemory->hydro = shmemory->hydro - 2;

            sem_post(&semaphore->oxybonding);   // call other atoms
            sem_post(&semaphore->hydrobonding);   // to start bonding

            bond(shmemory, semaphore, HYDROGEN, hydrocount); // and start bonding

            sem_wait(&semaphore->oxybonded);  // wait for other atoms 
            sem_wait(&semaphore->hydrobonded); // to be bonded
            shmemory->molecules++;  // molecule formed


            if ((shmemory->molecules) == N) // if last molecule created
            {
                for (int f=0; f < N*3; f++)
                    sem_post(&semaphore->finishsig); // send signals to other proc to finish
                sem_wait(&semaphore->finishsig); // and wait
                finish(shmemory, semaphore, HYDROGEN, hydrocount);
            }
            else
            {
                sem_post(&semaphore->molcheck); // continue processes 
                sem_wait(&semaphore->finishsig);    // and wait for finish
                finish(shmemory, semaphore, HYDROGEN, hydrocount);
            }
        }
        else
        {
            sem_wait(&semaphore->startsync);
                fprintf (fp,"%d\t:H %d\t:waiting\n",shmemory->actions, hydrocount);
                shmemory->actions++;
            sem_post(&semaphore->startsync);

            sem_post(&semaphore->molcheck); // continue

            sem_wait(&semaphore->hydrobonding); // start bonding
                bond(shmemory, semaphore, HYDROGEN, hydrocount);
            sem_post(&semaphore->hydrobonded); // done bonding

            sem_wait(&semaphore->finishsig);   // wait for finish
            finish(shmemory, semaphore, HYDROGEN, hydrocount);
        }

    
}

/**
 * Print bonded in all processes
 */
void bond(TShmem *shmemory, TShsem *semaphore, int ATOM, int atomcount)
{   
    FILE *fp = shmemory->fp; 
    char atomchar;
    if (ATOM == OXYGEN)
        atomchar = 'O';
    else if (ATOM == HYDROGEN)
        atomchar = 'H';

    
        sem_wait(&semaphore->startsync);
            fprintf (fp,"%d\t:%c %d\t:begin bonding\n",shmemory->actions, atomchar, atomcount);
            shmemory->actions++;
            shmemory->beginbond++;
            if (shmemory->beginbond == 3)
            {
                sem_post(&semaphore->bondsync);
                shmemory->beginbond = 0;
            }
        sem_post(&semaphore->startsync);

        usleep (rand () % ((shmemory->B) * 1000 + 1));

        sem_wait(&semaphore->bondsync);
        sem_post(&semaphore->bondsync);
            sem_wait(&semaphore->startsync);
                fprintf (fp,"%d\t:%c %d\t:bonded\n",shmemory->actions, atomchar, atomcount);
                shmemory->actions++;
            sem_post(&semaphore->startsync);
}

/**
 * Print finished in all processes
 */
void finish(TShmem *shmemory, TShsem *semaphore, int ATOM, int atomcount)
{
    sem_wait(&semaphore->finish);
        FILE *fp = shmemory->fp;
        char atomchar;
        if (ATOM == OXYGEN)
            atomchar = 'O';
        else if (ATOM == HYDROGEN)
            atomchar = 'H';

        fprintf (fp,"%d\t:%c %d\t:finished\n",shmemory->actions, atomchar, atomcount);
        shmemory->actions++;
    sem_post(&semaphore->finish);
}

/**
 * Save program arguments
 */
TArg save_arguments(int argc, char *argv[])
{
    TArg result;
    result.error = E_PARAMETER;
    if (argc == 5)
    {   
        for (int i=1; i<5; i++) 
            if (!isdigit(*argv[i])) // Check for integer values
                return result;
        if (atoi(argv[1]) > 0)
        {   result.N = atoi(argv[1]);

            if (atoi(argv[2]) >= 0 && atoi(argv[2]) < 5001)
            {   result.GH = atoi(argv[2]);

                if (atoi(argv[3]) >= 0 && atoi(argv[3]) < 5001)
                {   result.GO = atoi(argv[3]);

                    if (atoi(argv[4]) >= 0 && atoi(argv[4]) < 5001)
                    {   result.B = atoi(argv[4]);
                        result.error = E_OK;
                    }
                }
            }
                
        }
            
    }
    else
        result.error = E_PARAMETER;
    return result;
}

/**
 * Vypis chyb
 */
EError print_error(EError code)
{
    switch(code)
    {
        case(E_OK):
            break;
        case(E_PARAMETER):
            fprintf(stderr,"Error: Invalid params\n");
            break;
        case(E_FILE):
            fprintf(stderr,"Error: Opening file\n");
            break;
        case(E_SHMEM):
            fprintf(stderr,"Error: Shared memory init\n");
            break;
        case(E_SHSEM):
            fprintf(stderr,"Error: Shared semaphore init\n");
            break;
        case(E_FORK):
            fprintf(stderr,"Error: Fork()\n");
            break;
    }

    return code;
}

/**
 * Free shared memory
 */
void shmem_free(int shmemid, TShmem *shmemory)
{
    shmdt (shmemory); // detach memory segment
    shmctl (shmemid, IPC_RMID, NULL); // mark segment to be destroyed
}

/**
 * Destroy semaphores and free memory
 */
void shsem_free(int shsemid, TShsem *shsemaphore)
{ 
    sem_destroy(&shsemaphore->startsync);
    sem_destroy(&shsemaphore->molcheck);
    sem_destroy(&shsemaphore->oxybonding);
    sem_destroy(&shsemaphore->hydrobonding);
    sem_destroy(&shsemaphoresemaphore->bondsync);
    sem_destroy(&shsemaphore->oxybonded);
    sem_destroy(&shsemaphore->hydrobonded);
    sem_destroy(&shsemaphore->finish);
    sem_destroy(&shsemaphore->finishsig);

    shmdt (shsemaphore); // detach memory segment
    shmctl (shsemid, IPC_RMID, NULL); // mark segment to be destroyed
}