#include <stdio.h>
#include <commun.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <mer.h>

/*--------------------* 
 * Main demon 
 *--------------------*/
int
main( int nb_arg , char * tab_arg[] )
{
     char fich_mer[128] ;
     char nomprog[256] ;
     int fd;
     struct stat fich;
 
     /*----------*/

     if( nb_arg != 2 )
     {
	  fprintf( stderr , "Usage : %s <fichier mer>\n", 
		   tab_arg[0] );
	  exit(-1);
     }

     strcpy( nomprog , tab_arg[0] );
     strcpy( fich_mer , tab_arg[1] );
     fd = open(fich_mer,O_RDWR);
     stat(fich_mer,&fich);


     printf("\n%s : ----- Debut de l'affichage de la mer ----- \n", nomprog );
    /* if (fich.st_mtim){

     }*/
    printf("%d",fich.st_mtim);


     printf("\n%s : --- Arret de l'affichage de la mer ---\n", nomprog );

     exit(0);
}
