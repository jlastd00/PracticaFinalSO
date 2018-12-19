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

#define NUM_USUARIOS 10
#define NUM_FACTURADORES 2
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
pthread_mutex_t mutexSeguridad;
pthread_mutex_t mutexUsuario;
pthread_mutex_t mutexSeguridad;

// Contador de usuarios
int nUsuarios;

int nUsuariosColaFacturacion;


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

//int listaUsuarios[10];

typedef struct facturador{

	char id[15];
	int usuariosAtendidos;
	int facturadorOcupado;
	int cafe;
	//struct facturador *siguiente;

}facturador;

// Lista de usuarios (10) [id,facturado,atendido,tipo]
usuario colaFacturacion[10];

usuario listaUsuarios[NUM_USUARIOS];

facturador listaFacturadores[NUM_FACTURADORES];

// Usuario en el control
int usuarioEnControl; // 0 si está y 1 si no está en el control de seguridad.


// Fichero de log
FILE *logFile;
char *logFileName = "registroAeropuerto.log";

// Declaracion de funciones
void escribirEnLog(char *id, char *msg);
void nuevoUsuario(int senal);//funcion que recibe una señal SIGUSR1 (usuario normal);
void nuevoVip(int senal);//funcion que recibe una señal SIGUSR2 (usuario vip);
void accionesUsuario();
void accionesFacturador();
void accionesAgenteSeg();
void salir();
/******************************/

int main(char argc, char *argv[]) {

	int i;

	pthread_t facturador1;
	pthread_t facturador2;

	// 1. signal o sigaction SIGUSR1, nuevoUsuario normal
	if(signal(SIGUSR1,nuevoUsuario)==SIG_ERR)//si la señal es sigusr1 se mete a la funcion nuevoUsuario
		exit(-1);
		
	if(signal(SIGINT,salir)==SIG_ERR)//si la señal es sigint entra en exit
		exit(-1);
	// 2. signal o sigaction SIGUSR2, nuevoVip

	if(signal(SIGUSR2,nuevoVip)==SIG_ERR)//si la señal es sigusr2 se mete a la funcion nuevoVip
		exit(-1);
	// 3. inicializar recursos
		// a) semaforos
			pthread_mutex_init(&mutexUsuario, NULL);
			pthread_mutex_init(&mutexSeguridad, NULL);
			pthread_mutex_init(&mutexFacturador, NULL);
			pthread_mutex_init(&mutexLog, NULL);
		// b) contador de recursos
			nUsuariosColaFacturacion = 0;
			nUsuarios = 0;
		// c) lista de usuarios (id = 0, atendido = 0, facturado = 0)
			for (i = 0; i < NUM_USUARIOS; i++){

				sprintf(listaUsuarios[i].id, 0);
				listaUsuarios[i].facturado = 0;
				listaUsuarios[i].atendido = 0;
				listaUsuarios[i].tipo = 0;
			}


		// d) lista de facturadores
			
			for(i = 0; i < NUM_FACTURADORES; i++){

				sprintf(listaFacturadores[i].id, 0);
				listaFacturadores[i].usuariosAtendidos = 0;
				listaFacturadores[i].facturadorOcupado = 0;
				listaFacturadores[i].cafe = 0;

			}
			
		// e) usuario en control
			usuarioEnControl = 0;
		// f) fichero de log
			logFile = fopen(logFileName, "w");
		// g) variable condicion

	// 4. crear 2 hilos facturadores
			pthread_create(&facturador1, NULL, accionesFacturador, NULL);
			pthread_create(&facturador2, NULL, accionesFacturador, NULL);

	// 5. crear el hilo agente de control

	// 6. Esperar señal SIGUSR1 o SIGUSR2 o señal de finalizacion SIGINT
	// 7. Esperar por señales de forma infinita
	while(1)
		pause();

}

void nuevoUsuario(int senal) {

	if(signal(SIGUSR1,nuevoUsuario)==SIG_ERR)//si la señal es sigusr1 se mete a la funcion nuevoUsuario
		exit(-1);
	
	printf("He creado un nuevo usuario normal\n");
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


}

void nuevoVip(int senal) {

	if(signal(SIGUSR2,nuevoVip)==SIG_ERR)//si la señal es sigusr2 se mete a la funcion nuevoVip
		exit(-1);
	
	printf("He creado un nuevo usuario vip\n");
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

	pthread_mutex_lock(&mutexUsuario);



	pthread_mutex_unlock(&mutexUsuario);

	printf("Voy a comprobar si hay sitio para un usuario...\n");


	if (signal(SIGUSR1, SIG_IGN) == SIG_ERR) {
 
    	printf("Error: %s\n", strerror(errno));
 
  	}

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

	// 1. Buscar el primer usuario para atender (el que mas lleve esoerando)
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

void accionesSegurata() {

	// 1. Toma el mutex
	// 2. Comprueba que haya algún usuario esperando por seguridad
	// 3. Si no lo hay queda esperando a que llegue
	// 4. Si hay alguno escribe en el log y calcula la atención
	// 5. Según el tipo de atención duerme unos segundos u otros
	// 6. Cuando termina de dormir escribe en el log
	// 7. Avisa al usuario de que ha terminado la atención
	// 8. Libera el mutex

}

void salir(){

	if(signal(SIGINT,salir)==SIG_ERR)
		exit(-1);
    
    sleep(1);
		printf("He salido del programa\n");
    exit(0);

}

void escribirEnLog(char *id, char *msg) {
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

	logFile = fopen(logFileName, "a");
	fprintf(logFile, "[%s] %s: %s \n", stnow, id, msg);
	fclose(logFile);
}
