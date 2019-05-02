/*SERVIDOR*/
/********/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_CONN 10     //Nº máximo de conexiones en espera
#define MAX_TAM_MENSAJE 512 //Numero de caracteres maximo del mensaje

/**********************************************************/
/* función MAIN */
/* Orden Parametros: Puerto */
/**********************************************************/

main(int argc, char *argv[]) {
  int puerto_id, coneccion_id, destino_tam;
  struct sockaddr_in origen_dir, destino_dir;
  char mensaje_entrada[MAX_TAM_MENSAJE], mensaje_salida[MAX_TAM_MENSAJE];
 
  if (argc == 2) {
    // Creamos el socket
    puerto_id = socket(AF_INET, SOCK_STREAM, 0);
    if (puerto_id != -1) {
      origen_dir.sin_family = AF_INET;
      origen_dir.sin_port = htons(atoi(argv[1]));
      origen_dir.sin_addr.s_addr = htonl(INADDR_ANY);
      //Asigna una direccion local al socket
      if( bind(puerto_id, (struct sockaddr*)&origen_dir, sizeof(origen_dir)) != -1) {
      	printf("\n\aServidor ACTIVO escuchando en el puerto: %s\n",argv[1]);
      	//Espera al establecimiento de alguna conexión
      	listen(puerto_id, MAX_CONN);
      	//Permite atender a múltiples usuarios
      	while (1) {
        	//Establece una conexión
        	coneccion_id = accept (puerto_id, (struct sockaddr*)&destino_dir, &destino_tam);
        	//Recibe el mensaje del cliente
        	if (recv(coneccion_id, mensaje_entrada, sizeof(mensaje_entrada), 0)== -1)
          	perror("Error en recv");
        	else
						printf("\nLlego esto: %s\n", mensaje_entrada);
					//Envia el mensaje al cliente
					sprintf(mensaje_salida, "\nEl servidor contesta que llego el mesaje enviado\n");
					if (send(coneccion_id, strcat(mensaje_salida,"\0"), strlen(mensaje_salida)+1, 0) == -1)
          	perror("Error en send");
        	//Cierra la conexión con el cliente actual
        	close(coneccion_id);
      	}
      	//Cierra el servidor
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