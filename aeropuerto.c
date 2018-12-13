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

struct usuario{

	char id[13];
	int facturacion;
	int seguridad;
	int embarcar;
 	struct usuario *siguiente; 

}usuario;


struct facturador{

	char id[13];
	int usuariosAtendidos;
	struct facturador *siguiente;

}facturador;

struct listaUsuarios{

	usuario *cabeza;
	usuario *cola;

}listaUsuarios;

pthread_mutex_t mutexLog;

pthread_mutex_t mutexFacturador;

void escribirEnLog(char *id, char *msg);

void escribirEnLog(char *id, char *msg){

	/*
	 * escribirEnLog en la función que se encargará de escribir en el log
	 * La finalidad de esta función es que cada una de las partes que han de aparecer en el log sean escritar
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