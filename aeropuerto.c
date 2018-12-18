#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

/**
 * @author Juan Ramón Lastra Díaz
 * @author Héctor González Barrioluengo
 * @author Álvaro Casado García
 * @author Manuel Ugidos Fernández
 */

/*** Declaraciones Globales ***/

// Semáforos y variables condicion
pthread_mutex_t mutexLog;

pthread_mutex_t mutexFacturador;

pthread_mutex_t mutexUsuario;

pthread_mutex_t mutexSeguridad;

// Contador de usuarios
int nUsuarios;

// Lista de usuarios (10) [id,facturado,atendido,tipo]
/*struct listaUsuarios{
	usuario *cabeza;
	usuario *cola;
}listaUsuarios;*/

typedef struct usuario {
	char id[15];
	int facturado;
	int atendido;
	int tipo;

} usuario;

typedef struct usuario colaFacturacion[10];

typedef struct facturador{

	char id[15];
	int usuariosAtendidos;
	//struct facturador *siguiente;

}facturador;


// Usuario en el control

// Fichero de log
FILE *logFile;

// Declaracion de funciones
void escribirEnLog(char *id, char *msg);
void nuevoUsuario();
void accionesUsuario();
void accionesFacturador();
void accionesAgenteSeg();

/******************************/

int main(char argc, char *argv[]) {

	// 1. signal o sigaction SIGUSR1, nuevoUsuario normal
	// 2. signal o sigaction SIGUSR2, nuevoVip
	// 3. inicializar recursos
		// a) semaforos
		// b) contador de recursos
		// c) lista de usuarios (id = 0, atendido = 0, facturado = 0)
		// d) lista de facturadores
		// e) usuario en control
		// f) fichero de log
		// g) variable condicion
	// 4. crear 2 hilos facturadores
	// 5. crear el hilo agente de control
	// 6. Esperar señal SIGUSR1 o SIGUSR2 o señal de finalizacion SIGINT
	// 7. Esperar por señales de forma infinita

}

void nuevoUsuario() {

	// 1. comprobar si hay espacio en la lista de facturacion
		// a) si lo hay
			// I. se añade el usuario
			// II. incrementar contador de usuarios
			// III. nuevoUsuario.id = contadorUsuarios
			// IV. nuevoUsuario.atendido = 0
			// V. tipo = depende de la señal recibida
			// VI. crear el hilo para el usuario 
		// b) si no hay espacio
			// I. se ignora la llamada

	printf("Voy a comprobar si hay sitio para un usuario...\n");


	if (signal(SIGUSR1, SIG_IGN) == SIG_ERR) {
 
    	printf("Error: %s\n", strerror(errno));
 
  	}
/*
	pthread_mutex_lock(&mutexUsuario);

		while(nUsuarios < 10){

			nUsuarios++;

			int i;

			for(i = 0; i < colaFacturacion; i++){

				if(usuario.id == 0){

					printf("Se ha llegado a un hueco. El usuario %d se añade...\n",nUsuarios);

				}
			}
		
		}

	pthread_mutex_unlock(&mutexUsuario);
*/
}

void accionesUsuario() {

	// 1. guardar en el log la hora de entrada.
	// 2. guardar en el log el tipo de usuario.
	// 3. dormir 4 segundos
	// 4. Comprobar si está siendo atendido.
	// 5. Si no lo está, calculamos el comportamiento del usuario (si se va o si se queda) 
		// a) Si se va al baño o se cansa, se escribe en el log, se da fin al hilo
		//    de Usuario y se libera el espacio en la cola.
		// b) Sino debe dormir 4 segundos y volver al punto 4.
	// 6. Si está siendo atendido otro usuario debemos esperar a que termine.
	// 7. Cuando termine
		// a) Si ya ha sido facturado tiene que esperar por control
		// b) Si entra en control debe liberar la cola de facturaciónc. 
		// c) Se queda esperando a que pase la inspección y libera el control
		// d) Cuando acaba libera el control
		// e) Guardamos en el log que deja el control
		// f) Imprime que embarca y se guarda en el log
		// g) Libera control
	// 8. Si no ha facturado
		// a) Libera su posición en cola de facturación y se va
		// b) Escribe en el log
	// 9. Fin del hilo Usuario.

}

void accionesFacturador() {

	// 1. Buscar el primer vehículo para atender (el que mas lleve esoerando)
		/* a) Si no hay de mi tipo busco uno de la otra (si el usuario es normal y
			  facturador es de tipo vip y la cola del otro tiene más de un usuario, lo
			  atiende) */
		// b) Si no hay usuarios para atender espero un segundo y vuelvo a 1.
	// 2. Calculamos el tipo de facturación y en función de esto el tiempo de atención.
	// 3. Cambiamos el flag de atendido.
	// 4. Guardamos en el log la hora de atención.
	// 5. Dormimos el tiempo de atención.
	// 6. Guardamos en el log la hora de fin de la atención.
	// 7. Guardamos en el log el motivo del fin de la atención.
	// 8. Cambiamos el flag de atendido.
	// 9. Incrementamos el contador de usuarios atendidos.
	// 10. Mira si le toca tomar café.
	// 11. Volvemos al paso 1 y buscamos el siguiente (siempre con prioridad a su tipo). 

}

void accionesAgenteSeg() {

	// 1. Toma el mutex
	// 2. Comprueba que haya algún usuario esperando por seguridad
	// 3. Si no lo hay queda esperando a que llegue
	// 4. Si hay alguno escribe en el log y calcula la atención
	// 5. Según el tipo de atención duerme unos segundos u otros
	// 6. Cuando termina de dormir escribe en el log
	// 7. Avisa al usuario de que ha terminado la atención
	// 8. Libera el mutex

}



void escribirEnLog(char *id, char *msg){
	/*
	 * escribirEnLog en la función que se encargará de escribir en el log
	 * La finalidad de esta función es que cada una de las partes que 
	 * han de aparecer en el log sean escritar
	 * con el fin de registar lo que se pide.
	 */

	time_t actual = time(0);
	struct tm *tlocal = localtime(&actual);
	char stnow[19];
	strftime(stnow, 19, "%d/%m/%y %H:%M:%S", tlocal);

	logFile = fopen(logFileName, "Registro aeropuerto");
	fprintf(logFile, "[%s] %s: %s \n", stnow, id, msg);
	fclose(logFile);
}