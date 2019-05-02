/*CLIENTE*/
/*********/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_TAM_MENSAJE 512 //Numero de caracteres maximo del mensaje
 
/**********************************************************/
/* función MAIN */
/* Orden Parametros: IP destino, puerto , mensaje */
/**********************************************************/
main(int argc, char *argv[]) {
	int puerto_id;
	struct sockaddr_in origen_dir, destino_dir;
	char respuesta[MAX_TAM_MENSAJE];

 	if (argc == 4) {
		// Creamos el socket
		puerto_id = socket(AF_INET,SOCK_STREAM,0);
		if (puerto_id != -1) {
			origen_dir.sin_family = AF_INET;
			origen_dir.sin_port = htons(0); 								//Asigna un puerto disponible dela máquina
			origen_dir.sin_addr.s_addr = htonl(INADDR_ANY); //Asigna una IP de la máquina
 
			//Asigna una direccion local al socket
			if( bind(puerto_id, (struct sockaddr*)&origen_dir, sizeof(origen_dir)) != -1) {
				//Se prepara el nombre de la máquina remota
				destino_dir.sin_family = AF_INET;
				destino_dir.sin_addr.s_addr = inet_addr(argv[1]);
				destino_dir.sin_port = htons(atoi(argv[2]));
 
				//Establece la conexión con la máquina remota
				connect(puerto_id, (struct sockaddr*)&destino_dir, sizeof(destino_dir));
 
				//Envía el mensaje
				send(puerto_id,argv[3], strlen(argv[3])+1, 0);
				printf("\n\n->Enviando: %s, a: %s en el puerto: %s \n",argv[3], argv[1], argv[2]);
				
				//Recibe la respuesta
				recv(puerto_id, respuesta, sizeof(respuesta), 0);
				printf("%s\n", respuesta);
				
				//Se cierra la conexión (socket)
				close(puerto_id);
			} else {
    		printf("ERROR al nombrar el socket\n");
  		}
  	} else {
    	printf("ERROR: El socket no se ha creado correctamente!\n");
  	}
	} else {
  	printf("\n\n\aEl número de parámetros es incorrecto\n\n");
 	}
}