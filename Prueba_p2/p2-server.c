#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
// #include "header.h"
//  *   fecha log
#include <time.h>

#define PORT 5000
#define BACKLOG 32


int main()
{
    int serverfd, clientfd, r, opt = 1;
    struct sockaddr_in server, client; 
    socklen_t   tamano;
    char buffer[4];
    char bufferAVT[14];
    char buff[1];
    unsigned char buf[sizeof(struct in_addr)];
    char str[INET_ADDRSTRLEN];
    //  * data
    struct sharerow{
        short int sourceid,dstid,hod;
    };
    struct row{
        int mpos,npos;
        short int sourceid,dstid,hod;
        float mean;
    };
    struct row r1;

    //  *   Tiempo actual
    time_t t = time(NULL);
    struct tm tiempoLocal = *localtime(&t);

    //  *   El lugar en donde se pondrá la fecha y hora formateadas
    char fechaHora[70];
    //    *   El formato
    char *formato = "%Y-%m-%d %H:%M:%S";
    char actTime[70];

    int bytesEscritos = strftime(fechaHora, sizeof(fechaHora), formato, &tiempoLocal);

    //  *   Comprobacion
    if (bytesEscritos != 0) {
        //printf("[%s]", fechaHora);
        memcpy(actTime, fechaHora, sizeof(fechaHora));
        // actTime = fechaHora;

    } else {
        printf("Error formateando fecha");
    }

    //  *   Log
    FILE *log;
    
    char cadena[200];
    const char *aux;

    //  *   Busqueda
    FILE *fshare;
    fshare = fopen("share.bin","w+b");
    if(fshare==NULL){
        printf("Ocurrió un problema con el opening de share.bin!\n");
    }

    FILE *search;

    //  *   Instancia la estructura
    struct sharerow sr1={0,0,0};

    //  *   Creacion y comprobacion socket server
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverfd < 0){
        perror("\n-->Error en socket(): ");
        exit(-1);
    }
    
    //  *   Configuracion socket
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(server.sin_zero, 8);
    
    setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&opt, sizeof(int));
    
    //  *   Creacion y comprobacion bind
    r = bind(serverfd, (struct sockaddr*)&server, sizeof(struct sockaddr));
    if(r < 0){
        perror("\n-->Error en bind(): ");
        exit(-1);
    }

    //  *   Creacion y comprobacion listen
    r = listen(serverfd, BACKLOG);
    if(r < 0){
        perror("\n-->Error en listen(): ");
        exit(-1);
    }

    //  *   Creacion y comprobacion accept
    clientfd = accept(serverfd, (struct sockaddr *)&client, &tamano);
    if(clientfd < 0){
        perror("\n-->Error en accept(): ");
        exit(-1);
    }
    
    log = fopen("Log.txt", "w");
    if (log == NULL)
    {
        printf("Error al crear el log");
    }
    
    //  *   Creacion y comprobacion send
    r = send(clientfd, "Conexion establecida con el servidor", 36, 0);
    if(r < 0){
        perror("\n-->Error en send(): ");
        exit(-1);
    }

    
    //  *   Creacion y comprobacion recive
    r = recv(clientfd, &buffer, 4, 0);
    buffer[r] = 0;
    sr1.sourceid = (short)atoi(&buffer[0]);

    //  *   Creacion y comprobacion send
    r = send(clientfd, "Dato recibido", 14, 0);
    if(r < 0){
        perror("\n-->Error en send(): ");
        exit(-1);
    }

    // printf("[%s]", fechaHora);
    // printf("%s La cadenas es ", cadena);
    //fwrite(&cadena, sizeof(cadena), 1, log);

    //  *   Creacion y comprobacion recive
    r = recv(clientfd, &buffer, 4, 0);
    buffer[r] = 0;
    sr1.dstid = (short)atoi(&buffer[0]);

    //  *   Creacion y comprobacion send
    r = send(clientfd, "Dato recbido", 14, 0);
    if(r < 0){
        perror("\n-->Error en send(): ");
        exit(-1);
    }

    //  *   Creacion y comporbacion recive
    r = recv(clientfd, &buffer, 2, 0);
    buffer[r] = 0;
    sr1.hod = (short)atoi(&buffer[0]);

    //  *   Creacion y comprobacion send
    r = send(clientfd, "Dato recivido", 14, 0);
    if(r < 0){
        perror("\n-->Error en send(): ");
        exit(-1);
    }

    //  *   Busqueda
    fwrite(&sr1,sizeof(struct sharerow),1,fshare);
    fclose(fshare);
    system("./find");

    search = fopen("search.txt", "r");
    if(search==NULL){
        printf("Ocurrió un problema con el opening de search\n");
    }
    fread(&r1, sizeof(r1),1,search);

    //  *   Creacion y comprobacion recive
    r = recv(clientfd, &bufferAVT, 14, 0);
    bufferAVT[r] = 0;
    if (sizeof(bufferAVT )== 14)
    {
        //  *   Creacion y comprobacion send
        r = send(clientfd, &r1, sizeof(r1), 0);
        if(r < 0){
            perror("\n-->Error en send(): ");
            exit(-1);
        }
    }else{
        printf("No esta entrando\n");
    }
    
    //  *   Creacion y comprobacion recive
    r = recv(clientfd, &buff, 1, 0);
    bufferAVT[r] = 0;
    if (sizeof(buff )== 1)
    {
        //  *   Creacion y comprobacion send
        r = send(clientfd, "Desconexion", 12, 0);
        if(r < 0){
            perror("\n-->Error en send(): ");
            exit(-1);
        }
        //exit(0);
    }else{
        printf("No esta entrando\n");
    }

    if (inet_ntop(AF_INET, buf, str, INET_ADDRSTRLEN))
    {
        perror("\n Error en inet_ntop: ");
    }
    printf("%s\n", str);

    printf("[%s] Cliente[%s] [%0.2f - %d - %d]\n", actTime, str, r1.mean, r1.sourceid, r1.dstid);
    close(clientfd);
    close(serverfd);
    fclose(log);
    fclose(search);

}
