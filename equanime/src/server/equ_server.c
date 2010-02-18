#include <string.h>

#include "Equ_Server.h"
#include "Ecore.h"
#include "Ecore_Con.h"

#define ERR(...) EINA_LOG_DOM_ERR(_log_dom, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_log_dom, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_log_dom, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(_log_dom, __VA_ARGS__)

typedef struct _Equanime
{
	Eina_Array *modules;
	Ecore_Con_Server *srv;
	void *buffer;
	int length;
} Equanime;

static Eina_Array *_modules = NULL;
static Equanime _equd;
static int _log_dom = -1;
char *options = NULL;
char *module = NULL;
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

int _client_add(void *data, int type, void *event)
{
	Equ_Client *client;
	Ecore_Con_Event_Client_Add *e = event;

	client = equ_client_new(e->client);
	ecore_con_client_data_set(e->client, client);
	return 0;
}

int _client_del(void *data, int type, void *event)
{
	Equ_Client *client;
	Ecore_Con_Event_Client_Add *e = event;

	client = ecore_con_client_data_get(e->client);
	free(client);
}

int _client_data(void *data, int type, void *event)
{
	Ecore_Con_Event_Client_Data *cdata = event;
	Equ_Client *c;
	Equ_Message *m;
	void *body;
	void *reply = NULL;
	unsigned int m_length;
	Equ_Error err;

	c = ecore_con_client_data_get(cdata->client);
	if (!c) return 0;
	/* check if we got a full message */
	if (!_equd.buffer)
	{
		_equd.buffer = cdata->data;
		_equd.length = cdata->size;
		cdata->data = NULL;
	}
	else
	{
		_equd.buffer = realloc(_equd.buffer, _equd.length + cdata->size);
		memcpy(((unsigned char *)_equd.buffer) + _equd.length, cdata->data, cdata->size);
		_equd.length += cdata->size;
		cdata->data = NULL;
	}
message:
	if (_equd.length < sizeof(Equ_Message))
		return 0;
	/* ok, we have at least a message header */
	m = _equd.buffer;
	m_length = sizeof(Equ_Message) + m->size;
	if (_equd.length < m_length)
		return 0;
	/* parse the header */
	DBG("Message received of type %d with msg num %d of size %d", m->type, m->id, m->size);

	body = equ_message_decode(equ_message_name_get(m->type), (unsigned char *)m + sizeof(Equ_Message), m->size);
	if (!body)
	{
		ERR("Error Decoding\n");
		/* TODO check if the message needed a reply and if so
		 * send it the error number */
		goto shift;
	}
	/* check for the reply */
	err = equ_client_process(c, equ_message_name_get(m->type), body, &reply);
shift:
	if (equ_message_reply_has(m->type) == EINA_TRUE)
	{
		Equ_Reply r;
		Equ_Message_Name rname;
		void *rbody;

		r.id = m->id;
		r.error = err;
		equ_message_reply_name_get(m->type, &rname);
		DBG("Message encoded %d %d", rname, equ_message_name_get(m->type));
		if (reply)
			rbody = equ_message_encode(rname, reply, &r.size);
		else
			r.size = 0;
		DBG("Sending reply %d %d %d", r.id, r.error, r.size);
		ecore_con_client_send(cdata->client, &r, sizeof(Equ_Reply));
		if (r.size)
			ecore_con_client_send(cdata->client, rbody, r.size);
	}
	/* free in case we have served a complete message */
	if (_equd.length > m_length)
	{
		unsigned char *tmp;
		unsigned int n_length;

		tmp = _equd.buffer;
		n_length = _equd.length - m_length;

		_equd.buffer = malloc(n_length);
		_equd.length = n_length;
		memcpy(_equd.buffer, tmp + m_length, n_length);
		free(tmp);
		goto message;
	}
	else
	{
		free(_equd.buffer);
		_equd.buffer = NULL;
	}
	return 0;
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
	Ecore_Con_Server *srv;

	_equd.srv = ecore_con_server_add(ECORE_CON_LOCAL_USER,
			EQUANIME_NAME, EQUANIME_PORT, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_ADD, _client_add, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DEL, _client_del, NULL);
	ecore_event_handler_add(ECORE_CON_EVENT_CLIENT_DATA, _client_data, NULL);
	_equd.buffer = NULL;
	_log_dom = eina_log_domain_register("equd", NULL);
}

static void _server_shutdown(void)
{
	ecore_con_server_del(_equd.srv);
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

				default:
				goto cmd_error;
			}
		}
		else
			goto cmd_error;
	}
	/* initialize every system */
	eina_init();
	ecore_init();
	ecore_con_init();
	equ_message_init();
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
	equ_message_shutdown();
	ecore_con_shutdown();
	ecore_shutdown();
	eina_shutdown();

	return 0;

cmd_error:
	_help();
	return 0;
}
