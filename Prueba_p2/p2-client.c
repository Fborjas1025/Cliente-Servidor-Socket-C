#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "header.h"


#define PORT 5000
#define SERVER_ADDRESS  "127.0.0.1"


int main(int argc, char *argv[])
{
    int clientfd, serverfd, r;
    struct sockaddr_in client;
    struct hostent *he;
    char buffer[36];
    char str[10];
    struct sharerow sr2;
    struct row r1;
    //  *   menu var
    int opcion;
    int bucle = 1;
   	int sourceID = 0;
   	int destID = 0;
   	int hod = 0;
	float avt; 

    //  *   Creacion y comporbacion socket cliente
    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd < 0){
        perror("\n-->Error en socket(): ");
        exit(-1);
    }

    //  *   Configuracion del socket client
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);

    //  *   Ingreso de direccion servidor por consola
    // inet_aton(argv[1], &client.sin_addr);

    //  *   Ingreso direccion servidor por constante
    inet_aton(SERVER_ADDRESS, &client.sin_addr);


    //  *   Creacion y comporbacion connect
    r = connect(clientfd, (struct sockaddr *)&client, (socklen_t)sizeof(struct sockaddr));
    if(r < 0){
        perror("Error al configurar el socket (bind)");
        exit(-1);
    }    

    //  *   Creacion y comporbacion recive
    r = recv(clientfd, buffer, 36, 0);
    buffer[r] = 0;

    //  *   Impresion del mensaje recivido
    printf("\nMensaje Servidor: %s\n", buffer);


    //  *   Menu

    while(bucle!=0) {
    	//system("clear");
 		printf("\nBienbenido\n\n"
 			"1.  Ingresar origen\n"
 			"2.  Ingresar destino\n"
 			"3.  Ingresar hora\n"
 			"4.  Buscar tiempo de viaje medio\n"
 			"5.  Salir\n"
 			"*********************************************\n\t¿Opción?: ");
	 			
	 	scanf("%d", &opcion);

	 	switch(opcion) {
	 		case 1:
	  			system("clear");
	  			printf("\nIngrese ID del origen: ");
	  			scanf("%d", &sourceID);
                printf("%d", sourceID);
                sprintf(str, "%d", sourceID);
                  //  *   Creacion y comporbacion send
                r = send(clientfd, &str, 4, 0);
                if(r < 0){
                    perror("\n-->Error en send(): ");
                    exit(-1);
                }
                //  *   Creacion y comporbacion recive
                r = recv(clientfd, buffer, 48, 0);
                if (r > 0)
                {
                    buffer[r] = 0;
                    
                    //  *   Impresion del mensaje recivido
                    printf("\nMensaje Servidor: %s\n", buffer);
                    system("read -p 'Press Enter to continue...' var");
                } else
                {
                    perror("Error al recivir el dato");
                }
                
                

	  			bucle=-1;
	  			break;
	  			return(1);

	     	case 2:
	 			system("clear");
	  			printf("\nIngrese ID del destino: ");
	  			scanf("%d", &destID);
                sprintf(str, "%d", destID);
                  //  *   Creacion y comporbacion send
                r = send(clientfd, &str, 4, 0);
                if(r < 0){
                    perror("\n-->Error en send(): ");
                    exit(-1);
                }

                //  *   Creacion y comporbacion recive
                r = recv(clientfd, buffer, 48, 0);
                if (r > 0)
                {
                    buffer[r] = 0;
                    //  *   Impresion del mensaje recivido
                    printf("\nMensaje Servidor: %s\n", buffer);
                    system("read -p 'Press Enter to continue...' var");
                } else
                {
                    perror("Error al recivir el dato");
                }
	  			bucle=-1;
	  			break;
	  			return(1);
	  			
	     	case 3:
	  			system("clear");
	  			printf("\nIngrese hora del día: ");
	  			scanf("%d", &hod);
	  			while(hod < 0 || hod > 23){
	  			        printf("Ingrese un valor valido\n");
	  			        scanf("%d", &hod);
	  			}
                sprintf(str, "%d", hod);
                //  *   Creacion y comporbacion send
                r = send(clientfd, &str, 2, 0);
                if(r < 0){
                    perror("\n-->Error en send(): ");
                    exit(-1);
                }

                //  *   Creacion y comporbacion recive
                r = recv(clientfd, buffer, 48, 0);
                if (r > 0)
                {
                    buffer[r] = 0;
                    //  *   Impresion del mensaje recivido
                    printf("\nMensaje Servidor: %s\n", buffer);
                    system("read -p 'Press Enter to continue...' var");
                } else
                {
                    perror("Error al recivir el dato");
                }
	  			bucle=-1;
	  			break;
	  			return(1);
	  			
	     	case 4:
				system("clear");
                r = send(clientfd, "Solicitud AVT", 14, 0);
                if(r < 0){
                    perror("\n-->Error en send(): ");
                    exit(-1);
                }

                 //  *   Creacion y comporbacion recive
                r = recv(clientfd, &r1, sizeof(r1), 0);
                if (r > 0)
                {
                    buffer[r] = 0;
                    //r1.mean = atof(&buffer[0]);
                    //  *   Impresion del mensaje recivido
                    printf("\nEl tiempo medio es: %0.2f\n", r1.mean);
                    system("read -p 'Press Enter to continue...' var");
                } else
                {
                    perror("Error al recivir el dato");
                }       
				bucle=-1;
				break;
				return(1);
				
	     	case 5:
				system("clear");
				printf("\nHasta Luego!!!\n\n");
                r = send(clientfd, "", 1, 0);
                if(r < 0){
                    perror("\n-->Error en send(): ");
                    exit(-1);
                }

                r = recv(clientfd, &buffer, 12, 0);
                if (r > 0)
                {
                    buffer[r] = 0;
                    //r1.mean = atof(&buffer[0]);
                    //  *   Impresion del mensaje recivido
                    printf("\nMensaje del servidor: %s\n", buffer);
                    system("read -p 'Press Enter to continue...' var");
                    exit(0);
                } else
                {
                    perror("Error al recivir el dato");
                }

				bucle=0;
				break;
				return(1);
				
	     	default:
				system("clear");
				printf("\n¡Esa opción no es válida!\n");

	 	}
    }



    //  *   Cerrado de descriptores
    close(clientfd);

}


