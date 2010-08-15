#include <string.h>

#include "Equ_Server.h"
#include "Ecore.h"
#include "Eshm.h"

#define EQU_LOG_DOM _log_dom
static int _log_dom = -1;

static Eix_Server *srv = NULL;
static Eina_Array *_modules = NULL;
char *options = NULL;
char *module = NULL;
int debug = 0;
Eina_Bool options_help = EINA_FALSE;
Eina_Bool modules_list = EINA_FALSE;

static void _help(void)
{
	printf("equanime\n");
	printf("-m <module>             : The system module to load\n");
	printf("                          Use 'list' to list the available modules\n");
	printf("-p <port>               : The server port to listen connections on\n");
	printf("-n                      : Dont detach from the console\n");
	printf("-o <options>            : Module options\n");
	printf("                          Use 'help' to list the module's options\n");
	printf("-d                      : Enable debug\n");
	printf("-h                      : This help screen\n");
}

static void _quit(void)
{
	/* TODO send the message to quit every client */
	/* wait until all clients have finished */
	/* quit the main loop */
	printf("quittttting\n");
	ecore_main_loop_quit();
}

static Equ_Server_Backend _backend = {
	.quit = _quit,
};

static void _module_init(void)
{
	char *mpath;
	char *path;

	path = getenv("EQUANIME_DIR");
	if (!path)
	{
		path = strdup("/usr/local/lib/equanime");
	}
	_modules = eina_module_list_get(_modules, path, 1, NULL, NULL);
	eina_module_list_load(_modules);
}

static void _module_shutdown(void)
{
	eina_module_list_free(_modules);
}

static int _client_add(void *data, int type, void *event)
{
	Equ_Client *client;
	Eix_Event_Client_Add *e = event;

	if (eix_client_server_get(e->client) != srv)
		return ECORE_CALLBACK_RENEW;
	client = equ_client_new(e->client);
	eix_client_data_set(e->client, client);
	return ECORE_CALLBACK_RENEW;
}

static int _client_del(void *data, int type, void *event)
{
	Equ_Client *client;
	Eix_Event_Client_Add *e = event;

	if (eix_client_server_get(e->client) != srv)
		return ECORE_CALLBACK_RENEW;
	client = eix_client_data_get(e->client);
	free(client);
	return ECORE_CALLBACK_RENEW;
}

static int _server_process(Eix_Client *c, unsigned int type, void *msg,
		void **reply)
{
	Equ_Client *ec;

	ec = eix_client_data_get(c);
	return equ_client_process(ec, type, msg, reply);
}

static Eina_Bool _hosts_cb(Equ_Host *h, const char *name)
{
	if (modules_list)
		printf("%s\n", equ_host_name_get(h));
	else if (!strcmp(equ_host_name_get(h), name))
	{
		if (options_help)
		{
			Equ_Option *op;
			op = equ_host_options_get(h);
			if (!op) return;
			while (op->name != NULL)
			{
				printf("%s\t: %s (%s)\n", op->name, op->description, op->def);
				op++;
			}
		}
		else
		{
			equ_host_init(h, &_backend, options);
		}
	}
}


static void _server_init(void)
{
	srv = eix_new(EQUANIME_NAME, EQUANIME_PORT, _server_process);
	equ_common_server_setup(srv);
	ecore_event_handler_add(EIX_EVENT_CLIENT_ADD, _client_add, NULL);
	ecore_event_handler_add(EIX_EVENT_CLIENT_DEL, _client_del, NULL);
	_log_dom = eina_log_domain_register("equd", NULL);
	if (debug)
	{
		eina_log_domain_level_set("eshm", 5);
		eina_log_domain_level_set("equd", 5);
	}
}

static void _server_shutdown(void)
{
	eix_server_del(srv);
}

static void _server_setup(void)
{
	/* parse the cmd line options */
	equ_hosts_get(_hosts_cb, module);
}

int main(int argc, char **argv)
{
	int i;
	int parameter = 0;
	char option = 0;

	if (argc < 2) goto cmd_error;
	for (i = 1; i < argc; i++)
	{
		char *v = argv[i];

		if (parameter)
		{
			if (*v == '-')
				goto cmd_error;
			switch (option)
			{
				case 'o':
				if (!strcmp(v, "help"))
					options_help = EINA_TRUE;
				else
					options = strdup(v);
				break;

				case 'm':
				if (!strcmp(v, "list"))
					modules_list = EINA_TRUE;
				else
					module = strdup(v);
				break;
			}
			parameter = 0;
		}
		else if (*v == '-')
		{
			option = *(v + 1);
			switch (option)
			{
				case 'o':
				parameter = 1;
				break;

				/* the module to use */
				case 'm':
				parameter = 1;
				break;

				case 'd':
				debug = 1;
				break;

				default:
				goto cmd_error;
			}
		}
		else
			goto cmd_error;
	}
	/* initialize every system */
	if (!eshm_init())
		return 0;
	equ_common_init();
	_module_init();
	_server_init();
	/* setup the system */
	_server_setup();
	if (modules_list || options_help)
		goto module_exit;
	/* listen to the messages */
	ecore_main_loop_begin();
	/* shutdown every system */
module_exit:
	_server_shutdown();
	_module_shutdown();
	equ_common_shutdown();

	return 0;

cmd_error:
	_help();
	return 0;
}
