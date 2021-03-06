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

// Estructuras
struct usuario {
	
	char id[15];
	int facturado;
	int atendido;
	int tipo;
	struct usuario *siguiente;

}usuario;

struct facturador {

	char id[15];
	int usuariosAtendidos;
	int ocupado;

}facturador;

struct listaUsuarios {

  	struct usuario *primero;
  	struct usuario *ultimo;

}listaUsuarios;

struct esperaUsuarios {

  struct usuario* usuarioEsperando;
  struct esperaUsuarios* siguiente;

} esperaUsuarios;

struct listaEsperaUsuarios {

  struct esperaUsuarios* cabeza;
  struct esperaUsuarios* cola;

} listaEsperaUsuarios;

// Semáforos y variables condicion
pthread_mutex_t mutexLog;
pthread_mutex_t mutexFacturador;
pthread_mutex_t mutexUsuario;
pthread_mutex_t mutexListaUsuarios;
pthread_mutex_t mutexSeguridad;
pthread_mutex_t mutexListaEsperaUsuarios;
// Se declara la condición para el control de seguridad.
pthread_cond_t condicionControl;


// Contador de usuarios
int numeroUsuarios;

// Lista de usuarios (10) [id,facturado,atendido,tipo]


// Usuario en el control de Seguridad.
int usuarioEnControl;

// El segurata está listo para llevar a cabo el control.
int controlSegurata;

// Variable para que el usuario compruebe si ha pasado el control.
int usuarioCompruebaControl;
// int usuario
// Fichero de log
FILE *logFile;
char *logFileName = "registroAeropuerto.log";

// Otras variables
int maxUsuarios = 10;

// Declaracion de funciones
void escribirEnLog(char *id, char *msg);
void nuevoUsuario(int senal);//funcion que recibe una señal SIGUSR1 (usuario normal);
void aniadirUsuario(struct usuario* us);
void accionesUsuario();
void accionesFacturador();
void accionesSegurata();
void salir();
struct usuario* atenderUsuario();
<<<<<<< HEAD
int comprobarUsuarioControlSeguridad();
=======

>>>>>>> ff346b8797bf4100961aed47f700193fde6e237d
void eliminarUsuario(struct usuario* usuarioAEliminar);

/******************************/

int main(char argc, char *argv[]) {

	pthread_t facturador1, facturador2, segurata;

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

	if (signal(SIGUSR1, nuevoUsuario) == SIG_ERR) exit(-1);
	if (signal(SIGUSR2, nuevoUsuario) == SIG_ERR) exit(-1);
	if (signal(SIGINT, salir) == SIG_ERR) exit(-1);

	pthread_mutex_init(&mutexUsuario, NULL);
	pthread_mutex_init(&mutexListaUsuarios, NULL);
	pthread_mutex_init(&mutexFacturador, NULL);
	pthread_mutex_init(&mutexSeguridad, NULL);
	pthread_mutex_init(&mutexLog, NULL);

	numeroUsuarios = 1;

	listaUsuarios.primero = NULL;
	listaUsuarios.ultimo = NULL;
	
	usuarioEnControl = 0;

	logFile = fopen(logFileName, "w");

	/////////////////////////////////////////////
	//ESTOS DOS HILOS CREAN UN WARNING CADA UNO//
	/////////////////////////////////////////////
	pthread_create(&facturador1, NULL, accionesFacturador, NULL);
	pthread_create(&facturador2, NULL, accionesFacturador, NULL);
<<<<<<< HEAD
=======
	/*
>>>>>>> ff346b8797bf4100961aed47f700193fde6e237d
	pthread_create(&segurata, NULL, accionesSegurata, NULL);
	
	while (1) 

		pause();

}

void nuevoUsuario(int senal) {

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

	if (signal(SIGUSR1, SIG_IGN) == SIG_ERR) exit(-1);
	if (signal(SIGUSR2, SIG_IGN) == SIG_ERR) exit(-1);

	pthread_mutex_lock(&mutexUsuario);

	if (numeroUsuarios < maxUsuarios) {

		pthread_t hUsuario;
		struct usuario* us;
		char id[15];
		char numeroID[2];

		us = (struct usuario*)malloc(sizeof(struct usuario));

		sprintf(numeroID, "%d", numeroUsuarios);
		strcpy(id, "usuario_");
		strcat(id, numeroID);
		strcpy(us->id, id);

		us->facturado = 0;
		us->atendido = 0;
		if (senal == SIGUSR1) us->tipo = 0; // Usuario Normal
		if (senal == SIGUSR2) us->tipo = 1; // Usuario VIP

		if (pthread_create (&hUsuario, NULL, accionesUsuario, us) != 0) {
			printf("Error al crear el hilo. %s\n", strerror(errno));
			exit(-1);
		}
		else {
			numeroUsuarios++;
			//cantidadDeUsuariosActivos++;
			aniadirUsuario(us);
		}

		printf("Se ha creado un usuario %d\n", us->tipo);
		escribirEnLog(us->id, "Esta en cola de facturacion");

	}

	pthread_mutex_unlock(&mutexUsuario);

	if (signal(SIGUSR1, nuevoUsuario) == SIG_ERR) exit(-1);
	if (signal(SIGUSR2, nuevoUsuario) == SIG_ERR) exit(-1);

}

void aniadirUsuario(struct usuario* us) {

	pthread_mutex_lock(&mutexListaUsuarios);

	if (listaUsuarios.primero == NULL) {

		listaUsuarios.primero = us;
		listaUsuarios.ultimo = us;
		us->siguiente = NULL;

	}

	else {

		(listaUsuarios.ultimo)->siguiente = us;
		listaUsuarios.ultimo = us;
		us->siguiente = NULL;

	}

	printf("Añadido el usuario %s a la lista de ususarios\n", us->id);

	pthread_mutex_unlock(&mutexListaUsuarios);

}

void accionesUsuario(struct usuario* us) {

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
	

	//pthread_mutex_lock(&mutexLog);

struct usuario* nUsuario = (struct usuario*)us;

	pthread_mutex_lock(&mutexListaUsuarios); 
		/*
		 * Se bloquea el mutexLog, puesto que se va a escribir en el log y cada hilo tendrá
		 * que escribir sus datos correspondientes. Solo puede acceder un hilo a la vez. Al ser global es necesario el bloqueo
		 * y el desbloqueo de los mutex.
		 */

	printf("Hola! Hemos llegado a las funciones de usuario\n");

	printf("\nSoy el usuario %s \n", us->id);
	sleep(4);

	printf("%s voy a comprobar si me están atendiendo...\n", us->id);

	int usuarioIndignado = 0; //El usuario se indigna y se va porque no le atienden.
	
	int usuarioAlBano = 0; //El usuario comprueba si tiene que ir al baño.

	printf("EL NUMERO DE USUARIOS ES... %d\n", numeroUsuarios);

	if(us->atendido != 1){

		printf("No he sido atendido aún...\n");

	 	

		srand(time(NULL));

		usuarioIndignado = rand()%10+1;

		usuarioAlBano = rand()%10+1;

		if(usuarioIndignado > 8){

			printf("%s: me he cansado de esperar y me voy\n", us->id);
			/*
			 * Llegado este punto el usuario se debe eliminar.
			 */

			/*eliminarUsuario(nUsuario);
			
			numeroUsuarios--;
			
			pthread_exit(NULL);*/
		}

		/*Cada 3 segundos se comprueba si va al baño pero no se si está así bien.*/
		
		sleep(3);

		if(usuarioAlBano > 2){//el 2 es de prueba. No se si es correcto

			printf("%s: tengo que ir al baño, mi sitio queda libre...\n", us->id);
			/*
			 * Aquí se pasaría a la función para eliminar a este usuario y que su lugar se quede libre.
			 */

			/*eliminarUsuario(nUsuario);

			numeroUsuarios;

			pthread_exit(NULL);*/
		}else{

			printf("%s: voy a seguir esperando...\n",us->id);

			sleep(4);

			/*
			 * Tras esto debe comprobar de nuevo si se cansa, va al baño o es atendido. No estoy muy seguro de cómo hacerlo.
			 */
		}
	
	}else {

	printf("%s: ya estoy atendido ahora esperaré...\n", us->id);
	
	}
	
	pthread_mutex_unlock(&mutexListaUsuarios);

	//pthread_mutex_unlock(&mutexLog);
}

void accionesFacturador(struct listaUsuarios* usuario) {

	struct usuario* nUsuario;
	struct facturador* facturador_1;
	struct facturador* facturador_2;
<<<<<<< HEAD
=======

	int facturacionCorrecta = 0;
	int demasiadoPeso = 0;
	int visadoIncorrecto = 0;

>>>>>>> ff346b8797bf4100961aed47f700193fde6e237d
	// 1. Buscar el primer usuario para atender (el que mas lleve esperando)
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

	pthread_mutex_lock(&mutexFacturador);

	while(1) {

    		if (listaEsperaUsuarios.cabeza == NULL) {

      			sleep(1);

    		}else{

        		nUsuario = atenderUsuario();

			if(nUsuario->tipo==0){

				printf("Atendiendo a un usuario normal.\n");
<<<<<<< HEAD
				facturador_1->usuariosAtendidos++;
=======

				char mensajeFacturador[50];
				strcpy(mensajeFacturador, "Entra ");
        			strcat(mensajeFacturador, nUsuario->id);
        			escribirEnLog(facturador_1->id, mensajeFacturador);
>>>>>>> ff346b8797bf4100961aed47f700193fde6e237d
				
				//Cambio el flag de atendido
				nUsuario->atendido = 1;

<<<<<<< HEAD
				//Cada 5 usuarios atendidos el facturador descansa 10 segundos
				if(facturador_1->usuariosAtendidos%5 == 0){
=======
				facturador_1->usuariosAtendidos++;

				//Compruebo si la facturación es correcta para poder pasar al control de seguridad
				//ESTÁ INCOMPLETO
				srand(time(NULL));

				//facturacionCorrecta = rand()%

				//Cada 5 usuarios atendidos el facturador descansa 10 segundos
				if(facturador_1->usuariosAtendidos%5 == 0){

					printf("Soy el facturador_1 y necesito descansar 10 segundos. Ahora vuelvo.\n");

>>>>>>> ff346b8797bf4100961aed47f700193fde6e237d
					sleep(10);
				}
			}				
			if(nUsuario->tipo==1){

				printf("Atendiendo a un usuario VIP.\n");
<<<<<<< HEAD
				facturador_2->usuariosAtendidos++;

				//Cambio el flag de atendido
				nUsuario->atendido = 1;
				
				//Cada 5 usuarios atendidos el facturador descansa 10 segundos
				if(facturador_2->usuariosAtendidos%5 == 0){
=======

				//Cambio el flag de atendido
				nUsuario->atendido = 1;

				facturador_2->usuariosAtendidos++;

				//Cada 5 usuarios atendidos el facturador descansa 10 segundos
				if(facturador_2->usuariosAtendidos%5 == 0){

					printf("Soy el facturador_2 y necesito descansar 10 segundos. Ahora vuelvo.\n");

>>>>>>> ff346b8797bf4100961aed47f700193fde6e237d
					sleep(10);
				}
			}
		}

	}

	pthread_mutex_unlock(&mutexFacturador);

}

struct usuario* atenderUsuario() {

  pthread_mutex_lock(&mutexListaEsperaUsuarios);

  	struct usuario* usuarioAtendido = listaEsperaUsuarios.cabeza->usuarioEsperando;

  	listaEsperaUsuarios.cabeza = listaEsperaUsuarios.cabeza->siguiente;

  pthread_mutex_unlock(&mutexListaEsperaUsuarios);

  return usuarioAtendido;

}

<<<<<<< HEAD
/*
 * En la función accionesSegurata, se toman los usuarios que pasan al control de seguridad.
 * El segurata hace el control a un usuario
 */

void *accionesSegurata(struct listaUsuarios* usuarioAPasarPorSeguridad) {
=======
void accionesSegurata() {
>>>>>>> ff346b8797bf4100961aed47f700193fde6e237d

	// 1. Toma el mutex
	// 2. Comprueba que haya algún usuario esperando por seguridad
	// 3. Si no lo hay queda esperando a que llegue
	// 4. Si hay alguno escribe en el log y calcula la atención
	// 5. Según el tipo de atención duerme unos segundos u otros
	// 6. Cuando termina de dormir escribe en el log
	// 7. Avisa al usuario de que ha terminado la atención
	// 8. Libera el mutex

	// pthread_mutex_lock(&mutexSeguridad);

	while(1){

		struct usuario *aux;

		controlSegurata = 0;
	
	}



	// pthread_mutex_unlock(&mutexSeguridad);

}

int comprobarUsuarioControlSeguridad(){

	/*
	 * En esta función, se declara una variable que será la que indica si el usuario pasa el control de seguridad.
	 * Se lleva a cabo el mutexlock sobre el mutexSeguridad. Dentro se comprueba si se pasa el control.
	 * Si se pasa el control la variable cambia de valor. Se desbloquea el mutex.
	 * Se retorna el valor de la variable que indica si ha pasado el control de seguridad o no.
	 */

	int pasoControl = 0;

	if(controlSegurata == 1){

		pthread_mutex_lock(&mutexSeguridad);

			if(++usuarioCompruebaControl >= numeroUsuarios){

				pthread_condition_signal(&condicionControl);

				pasoControl = 1;

			}

		pthread_mutex_unlock(&mutexSeguridad);

	}

	return pasoControl;
}


void eliminarUsuario(struct usuario* usuarioAEliminar){

	/*
	 * Este método se llamará cada vez que sea necesario eliminar a un usuario.
	 * La eliminación de un usuario viene dada bien porque este se vaya al baño, bien porque se cansa...
	 */

	//=======================================================
	/*
	 * Aquí tal vez sea mejor usar un mutexListaUsuarios
	 */
	//=======================================================

	//printf("%s: pues me elimino, me he enfadado.\n", us->id);

	pthread_mutex_lock(&mutexListaUsuarios);

		struct usuario* auxiliar;

		if(listaUsuarios.primero == usuarioAEliminar){

			listaUsuarios.primero = listaUsuarios.primero->siguiente;


		}else if(listaUsuarios.ultimo == usuarioAEliminar){

			auxiliar = listaUsuarios.primero;

			while(auxiliar != usuarioAEliminar){

				auxiliar = auxiliar->siguiente;	

			}

			listaUsuarios.ultimo = auxiliar;

			auxiliar->siguiente = NULL;

		}else{

			auxiliar = listaUsuarios.primero;

			while((auxiliar->siguiente != usuarioAEliminar) && (auxiliar->siguiente != NULL)){

				auxiliar = auxiliar->siguiente;

			}

			auxiliar->siguiente = usuarioAEliminar->siguiente;

		}

	free(usuarioAEliminar);	

	pthread_mutex_unlock(&mutexListaUsuarios);
}

void salir() {

	if (signal(SIGINT,salir) == SIG_ERR) exit(-1);
    
    sleep(1);
	
	printf("He salido del programa\n");
    
    exit(0);

}

void escribirEnLog(char *id, char *msg) {
	/*
	 * escribirEnLog es la función que se encargará de escribir en el log
	 * La finalidad de esta función es que cada una de las partes que 
	 * han de aparecer en el log sean escritar
	 * con el fin de registar lo que se pide.
	 */

	pthread_mutex_lock(&mutexLog);

	// Calculamos la hora actual
	time_t actual = time(0);
	struct tm *tlocal = localtime(&actual);
	char stnow[19];
	strftime(stnow, 19, "%d/%m/%y  %H:%M:%S", tlocal);

	// Escribimos en el log
	logFile = fopen("registroAeropuerto.log", "a");
	fprintf(logFile, "[%s] %s: %s\n", stnow, id, msg);
	fclose(logFile);

	pthread_mutex_unlock(&mutexLog);

<<<<<<< HEAD
}
=======
}
>>>>>>> ff346b8797bf4100961aed47f700193fde6e237d
