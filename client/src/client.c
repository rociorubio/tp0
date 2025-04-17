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

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger, "Hola! soy un log");
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config(logger);

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	valor = config_get_string_value(config, "CLAVE");
	
	// Loggeamos el valor de config
	log_info(logger, "Valor obtenido: %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0_log.log", "Cliente", 1, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL){
		perror("No se pudo inicializar el logger");
		exit(EXIT_FAILURE);
	}

	return nuevo_logger;
}

t_config* iniciar_config(t_log* logger)
{
	t_config* nuevo_config =  config_create("cliente.config");

    if (nuevo_config == NULL) {
        log_error(logger, "No se pudo cargar el archivo de configuración.");
        exit(EXIT_FAILURE);
    }

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
    // Ciclo que continúa hasta recibir una línea vacía

 	while (1) {
        leido = readline("> ");
	    // El resto, las vamos leyendo y logueando hasta recibir un string vacío

        if (strcmp(leido, "") == 0) {
            free(leido);
            break;
        }

        log_info(logger, ">> %s", leido);
		// ¡No te olvides de liberar las lineas antes de regresar!
        free(leido);
    }
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

    while (1) {
        leido = readline("> ");
        if (strcmp(leido, "") == 0) {
            free(leido);
            break;
        }
        agregar_a_paquete(paquete, leido, strlen(leido) + 1);
        free(leido);
    }

    enviar_paquete(paquete, conexion);
    eliminar_paquete(paquete);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{

	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	printf("CLIENTE CERRADO");
}
