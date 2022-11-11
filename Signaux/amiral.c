/* 
 * Programme pour processus navire-amiral :
 */

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#include <mer.h>
#include <bateaux.h>


/* 
 * VARIABLES GLOBALES (utilisees dans les handlers)
 */

char Nom_Prog[256] ;

/*
 * FONCTIONS LOCALES
 */


void tir_handler(int sig, siginfo_t *info, void *ucontext)
{
    printf("Reception d'un signal pour tirer sur un bateau");
    printf(" Pid du processus entrant %d \n",info->si_pid);
}

int test(int i, int j)
{
    return i+j;
}

/*
 * Programme Principal
 */


main( int nb_arg , char * tab_arg[] )
{
     char fich_mer[128] ;
     int fd;

    /* int (*fct)(int,int);
     fct = &test;
     int result = (*fct)(1,2);*/


     /*----------*/

     /* 
      * Capture des parametres 
      */

     if( nb_arg != 2 )
     {
	  fprintf( stderr , "Usage : %s <fichier mer> \n", 
		   tab_arg[0] );
	  exit(-1);
     }

     strcpy( Nom_Prog , tab_arg[0] );
     strcpy( fich_mer , tab_arg[1] );
     fd = open("../Fichiers/pid.txt",O_WRONLY);
     int pid;
     pid = getpid();
     printf("%d",pid);
     write(fd,&pid,sizeof(int));



     siginfo_t siginfo;



     struct sigaction sig;
     sig.sa_flags = SA_SIGINFO;
     sig.sa_sigaction = &tir_handler;


     sigemptyset(&sig.sa_mask);
     sigaddset(&sig.sa_mask,SIGUSR2);
     sigaction(SIGUSR1,&sig,NULL);









     
     /*
      * Affichage pid bateau amiral 
      */
     
     printf(" PID bateau amiral = %d\n" , getpid() ) ;

     /* Initialisation de la generation des nombres pseudo-aleatoires */
     srandom((unsigned int)getpid());
    while(1)
    {
        pause();
    }
 
     /***********/

     /***********/

     printf("\n\n\t----- Fin du jeu -----\n\n");
     
     exit(0);
}

