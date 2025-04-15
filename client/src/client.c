#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	//char *current_dir = getcwd(NULL, 0);
    //printf("El directorio actual es %s\n", current_dir);

	//string_append(&current_dir, "/archivo.log");
    //free(current_dir);
	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	//log_info (logger, "Hola! Soy un Log");
	
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value (config, "IP");//dado el config anterior leemos su ip que esta contenido dentro del archivo asi,
	valor = config_get_string_value (config, "CLAVE");
	puerto = config_get_string_value (config, "PUERTO");
	//printf("la ip es: %s", ip);
	
	// Loggeamos el valor de config
	log_info (logger, "El VALOR es: %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	//abrir la condola de server y cliente. Ejecutar con el comando: ./bin/client (para el cliente) y ./bin/server (para el server).

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	//t_log* log_create (char *file, char *process_name, bool is_active_console, t_log_level level)
	nuevo_logger = log_create ("tp0.log", "Joaquin", 1, LOG_LEVEL_INFO);
	log_info (nuevo_logger, "Hola! Soy un Log");
	


	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	//t_config * 	config_create (char *path)
	char *current_dir = getcwd(NULL, 0); // guardamos en current_dir la direccion de memoria del directorio actual 
	string_append(&current_dir, "/cliente.config"); //concatenamos "/cliente23.config" con el path que contiene &current_dir
    
	nuevo_config = config_create(current_dir); // Le pasamos la ruta(o path) y se encarga de leerlo, NO se genera nada
	//free(current_dir);
	if (nuevo_config == NULL) {
		printf("no se pudo crear");
		abort();
    // ¡No se pudo crear el config!
    // Terminemos el programa
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("ingrese algo: ");
	log_info (logger, " lo que ingreso es: %s", leido);//printf("lo que se ingreso es: %s", leido);
	
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido,"")!=0){
		free(leido);// IMPORTANTE que vaya primero free(leido), xq la primera vez que ingresas hace la comparacion con la cadena vacia, si es == 0 no entra al while; si es distinto, libera el espacio de memoria (borra el primer contenido de leido) para sobreescribir ese mismo espacio de memoria con otra palabra. 
		leido = readline("ingrese algo: ");
		log_info (logger, " lo que ingreso es: %s", leido);
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("Ingrese algo: ");
	
	while(strcmp(leido,"")!= 0){
		
		agregar_a_paquete(paquete, leido, strlen(leido)+1);//se pone adentro del while, xq lo que vayamos escribiendo se va agregando al paquete. Si lo pusieramos afuera se agregaria nomás el ultimo contenido de leido (que es vacio xq salio del while).
		free(leido);
		leido = readline("ingrese algo: ");
	}
	
	free(leido);

	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger);
	free(config);
	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
