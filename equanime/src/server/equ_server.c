#include <string.h>

#include "Equ_Server.h"
#include "Ecore.h"
#include "Ecore_Con.h"

typedef struct _Equanime
{
	Eina_Array *modules;
	Ecore_Con_Server *srv;
} Equanime;

static Eina_Array *_modules = NULL;

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

int _client_add(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Add *e = event;
	printf("client added\n");
}

int _client_del(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Add *e = event;
	printf("client del\n");
}

int _client_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Data *cdata = event;
	printf("client data\n");
}

static void _server_init(void)
{
	Ecore_Con_Server *srv;

	srv = ecore_con_server_add(ECORE_CON_LOCAL_USER,
			EQUANIME_NAME, EQUANIME_PORT, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD, _client_add, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL, _client_del, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA, _client_data, NULL);
}

static void _server_shutdown(void)
{
	//ecore_con_server_del(srv);
}

int main(int argc, char **argv)
{
	Equanime equ;

	eina_init();
	ecore_init();
	ecore_con_init();
	// equ_message_init();
	/* initialize the modules */
	_module_init();
	_server_init();
	/* setup the system */
	/* listen to the messages */
	ecore_main_loop_begin();
	_server_shutdown();
	_module_shutdown();
	// equ_message_shutdown();
	ecore_con_shutdown();
	ecore_shutdown();
	eina_shutdown();

	return 0;
}
