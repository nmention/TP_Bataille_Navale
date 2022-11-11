#include <stdio.h>

#include <sys/types.h> /* fcntl */
#include <unistd.h>
#include <fcntl.h>
#include <errno.h> /* errno */
#include <string.h>

#include <bateau.h>
#include <mer.h>

/*
 *  Programme principal
 */

void initialiseVerrou(struct flock *verrou){
    verrou->l_type = F_UNLCK;
    verrou->l_start = SEEK_SET;
    verrou->l_len = 0;
    verrou->l_whence = 0;
}

int deverouille(int fd,struct flock *verrou){
    verrou->l_type = F_UNLCK;
    if (fcntl(fd,F_SETLKW,verrou) != 0)
    {
        printf("erreur fcntl %d\n", errno);
    }
    printf("Libération du verrou actuel : %hu\n", verrou->l_type);
}

int checkVerrou(int fd, struct flock *verrou){

    verrou->l_type = F_RDLCK;
    if (fcntl(fd,F_GETLK,verrou) != 0)
    {
        printf("erreur fcntl %d\n", errno);
    }

    if (verrou->l_type == F_RDLCK){
        printf("Verrou en lecture actuellement en place\n");
        return 1;
    }
    else if (verrou->l_type == F_WRLCK){
        printf("Verrou en écriture actuellement en place\n");
        return 2;
    }
    else if (verrou->l_type == F_UNLCK){
        printf("Fichier libre ou verrouillé par le processus courant\n");
        return 0;
    }
    else{
        printf("erreur %d\n",verrou->l_type);
        return -1;

    }

}


int mettreVerrou(int fd, char *mode, struct flock *verrou){
    if (checkVerrou(fd,verrou) != 0){
        printf("Un navire tire déjà ou la map s'affiche \n");
        return -1;
    }
    else {
        if (strcmp(mode, "lecture") == 0){
            verrou->l_type = F_RDLCK;
            printf("Pose du verrou en lecture\n");
        }
        else if (strcmp(mode, "ecriture") == 0){
            verrou->l_type = F_WRLCK;
            printf("Pose du verrou en écriture\n");
        }
        else {
            perror("Veuillez choisir entre lecture et ecriture pour mode");
            exit(-1);

        }
        if ((fcntl(fd,F_SETLKW,verrou) != 0)){
            printf("Erreur de pose de verrou %d", errno);
        }

        printf("Mise en place du verrou : %hu\n", verrou->l_type);
        if (verrou->l_type == F_WRLCK)
            printf("Verrouillage confirmé !\n");
        return 1;


    }

}
int checkDommage(int fd,struct flock *verrou){
    char lecture[256];
    if (checkVerrou(fd,verrou) !=0){
        printf("peut pas afficher les dommages \n");
        return -1;
    }
    else {
        read(fd,&lecture,sizeof(char));
        mettreVerrou(fd,"lecture",verrou);
        return 0;

    }
}
int tirSur(int fd, const coord_t cible,struct flock *verrou){

    int ret;
    do {
        ret = mettreVerrou(fd,"ecriture",verrou);
        sleep(1);
    } while (ret == -1);

    mer_bateau_cible_tirer(fd,cible);
    //write(fd,&coord,sizeof(int));
    deverouille(fd,verrou);
}






int main( int nb_arg , char * tab_arg[] )
{

    char fich_mer[128] ;
    int fd;
    bateau_t * bateau = NULL ;
    coords_t * liste_voisins = NULL ;

    struct flock verrou;
    booleen_t ok = VRAI ;
    initialiseVerrou(&verrou);


    case_t marque = MER_CASE_LIBRE ;
    char nomprog[128] ;
    float energie = 0.0 ;
    coord_t cible ;
    booleen_t acquisition ;

    /*----------*/


    strcpy( nomprog , tab_arg[0] ) ;

    if( nb_arg != 4 )
    {
        fprintf( stderr , "Usage : %s <fichier mer> <marque> <energie>\n",
                 nomprog );
        exit(-1);
    }

    if( strlen(tab_arg[2]) !=1 )
    {
        fprintf( stderr , "%s : erreur marque <%s> incorrecte \n",
                 nomprog , tab_arg[2] );
        exit(-1) ;
    }


    strcpy( fich_mer , tab_arg[1] );
    marque = tab_arg[2][0] ;
    sscanf( tab_arg[3] , "%f" , &energie );
    printf("%s\n",fich_mer);
    fd = open(fich_mer,O_RDWR);
    mer_afficher(fd);
    bateau = bateau_new( NULL ,marque ,100);
    mer_bateau_initialiser( fd,bateau);
    mer_afficher(fd);

    mer_voisins_rechercher(fd,bateau,&liste_voisins);
    mer_bateau_deplacer(fd,bateau,liste_voisins,&ok);
    mer_bateau_cible_acquerir( fd,bateau,&acquisition,&cible);


    /* Initialisation de la generation des nombres pseudo-aleatoires */
    srandom((unsigned int)getpid());

    printf( "\n\n%s : ----- Debut du bateau %c (%d) -----\n\n ",
            nomprog , marque , getpid());

    /*deverouille(fd);*/
    tirSur(fd,cible,&verrou);

    //printf("%d\n", checkVerrou(fd));








    printf( "\n\n%s : ----- Fin du navire %c (%d) -----\n\n ",
            nomprog , marque , getpid() );

    exit(0);
}