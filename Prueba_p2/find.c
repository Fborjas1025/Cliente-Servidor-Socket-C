#include<stdio.h>
#include<stdlib.h>
#include"header.h"
#include <sys/time.h>

int main(){
    struct timeval ti, tf;
	double tiempo; 
    gettimeofday(&ti, NULL);				// Instante inicial
	
	
    /*
    Este script es para la consulta
    */

    /*Puntero FILE para leer de share.bin que el archivo que sirve para comunicar
    interface.c y este código llamado find.c*/

    FILE *fshare;
    fshare = fopen("share.bin","r+b");
    if(fshare==NULL){
        printf("Ocurrió un problema con el opening de share.bin!\n");
    }
    FILE *search;
    search = fopen("search.txt","w");
    if(search==NULL){
        printf("Ocurrió un problema con el opening de search!\n");
    }
    struct sharerow sr1;
    fread(&sr1,sizeof(struct sharerow),1,fshare);
    fclose(fshare);
    
    //Memoria dinámica en este lugar, sólo aquí
    int *np=malloc(sizeof(int));
    
    /*
    Tests
    478,627,0,1145.57
    912,667,15,1514.33
    252,435,20,1403.8
    907,1072,14,1801.23

    s = sr1.sourceid;
    d =sr1.dstid;
    h= sr1.hod;   
    */
 
   
    if(sr1.sourceid == 0 || sr1.dstid == 0){
        printf("N/A\n");
        exit(0);
    }

    

    struct row r1;
    struct hashrow hr1;   


    FILE *filehash;
    filehash = fopen("hashtable.bin","r+b");
    if(filehash==NULL){
        printf("Ocurrió un problema con el opening del FILE hashtable.bin");
        exit(0);
    }

    FILE *filedata;
    filedata = fopen("data.bin","r+b");
    if(filedata==NULL){
        printf("Ocurrió un problema con el opening del FILE data.bin");
        exit(0);
    }
    

    fseek(filehash,(sr1.sourceid-1)*sizeof(struct hashrow),SEEK_SET);
    fread(&hr1,sizeof(struct hashrow),1,filehash);
    fclose(filehash);

    //El valor para buscar ahora en data.bin
    *np = hr1.img;

    short int forWait;

    while(*np!=0){
        fseek(filedata,(*np-1)*sizeof(struct row),SEEK_SET);
        fread(&r1,sizeof(struct row),1,filedata);
        if(sr1.sourceid==r1.sourceid && sr1.dstid==r1.dstid && sr1.hod==r1.hod){
            fwrite(&r1, sizeof(r1), 1, search);
            fclose(search);
            break;
            printf("\nTiempo de viaje medio: %0.2f\n\n",r1.mean);
            printf("mpos: %d \nnpos: %d\n",r1.mpos,r1.npos);
            printf("sourceid: %d \ndstid: %d \nhod: %d\n",r1.sourceid,r1.dstid,r1.hod);
            fclose(filedata);
            free(np);
            //printf("\nEsperando el ingreso de cualquier número para terminar el proces: ");
            //scanf("%hd",&forWait);

            //gettimeofday(&tf, NULL);   				// Instante final
            //tiempo= (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0;
            //printf("La busqueda Ha tardado: %g milisegundos\n", tiempo);
            exit(0);
        }else
        {
            printf("NA\n");
        }
        
        //*np=r1.npos;
    }

    
    fclose(filedata);
    free(np);
    //printf("\nEsperando el ingreso de cualquier número para terminar el proceso: ");
    //scanf("%hd",&forWait);

    //gettimeofday(&tf, NULL);   				// Instante final
	//tiempo= (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0;
	//printf("La busqueda Ha tardado: %g milisegundos\n\n", tiempo);
    return 0;
}