/*SERVIDOR*/
/********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MAX_CONN 100     //Nº máximo de conexiones en espera
#define MAX_TAM_MENSAJE 512 //Numero de caracteres maximo del mensaje

/**********************************************************/
/* función MAIN                                           */
/* Orden Parametros: Puerto                               */
/**********************************************************/

int main(int argc, char *argv[]) {
  int puerto_id, coneccion_id;
  socklen_t destino_tam;
  struct sockaddr_in origen_dir, destino_dir;
  char mensaje_entrada[MAX_TAM_MENSAJE], mensaje_salida[MAX_TAM_MENSAJE];

  if (argc != 2) {
    printf("\n\n\aEl número de parámetros es incorrecto\n");
    printf("\aUse: %s <puerto>\n\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  // Creamos el socket del servidor
	puerto_id = socket(AF_INET,SOCK_STREAM,0);
	if (puerto_id == -1) {
    printf("ERROR: El socket del servidor no se ha creado correctamente!\n");
    exit(EXIT_FAILURE);
  }

  //Se prepara la dirección de la máquina servidora
  origen_dir.sin_family = AF_INET;
  origen_dir.sin_port = htons(atoi(argv[1]));
  origen_dir.sin_addr.s_addr = htonl(INADDR_ANY);

  //Asigna una direccion local al socket
	if( bind(puerto_id, (struct sockaddr*)&origen_dir, sizeof(origen_dir)) == -1) {
    printf("ERROR al unir el socket a la dirección de la máquina servidora\n");
    close(puerto_id);
    exit(EXIT_FAILURE);
  }

  //Espera al establecimiento de alguna conexión de multiples usuarios
  if( listen(puerto_id, MAX_CONN) == -1) {
    printf("ERROR al establecer la escucha de N conecciones en el servidor\n");
    close(puerto_id);
    exit(EXIT_FAILURE);
  }
  printf("\n\aServidor ACTIVO escuchando en el puerto: %s ...\n",argv[1]);

  while(1){
    //Establece una conexión
    destino_tam=sizeof(destino_dir);
    coneccion_id = accept(puerto_id, (struct sockaddr*)&destino_dir, &destino_tam);
    if(coneccion_id == -1) {
      printf("ERROR al establecer la conección del servidor con el cliente\n");
      close(puerto_id);
      exit(EXIT_FAILURE);
    }

    do {
      //Recibe el mensaje del cliente
      if (recv(coneccion_id, mensaje_entrada, sizeof(mensaje_entrada), 0) == -1) {
        perror("Error en recv");
        close(coneccion_id);
        close(puerto_id);
        exit(EXIT_SUCCESS);
      } else
			   printf("\nLlego esto: %s\n", mensaje_entrada);

      //Envia el mensaje al cliente
		  sprintf(mensaje_salida, "\nEl servidor contesta que llego el mesaje enviado %s\n",mensaje_entrada);
		  if (send(coneccion_id, strcat(mensaje_salida,"\0"), strlen(mensaje_salida)+1, 0) == -1) {
        perror("Error en send");
        close(coneccion_id);
        close(puerto_id);
        exit(EXIT_SUCCESS);
      } else
        printf("\nServidor le respondio al clientet esto: %s\n", mensaje_salida);
    }while(strcmp(mensaje_entrada,"terminar();") != 0);

    //Cierra la conexión con el cliente actual
    printf("Cerrando conección con cliente\n");
    close(coneccion_id);
  }
  //Cierra el servidor
  printf("Cerrando servicio\n");
  close(puerto_id);
  exit(EXIT_SUCCESS);
}
