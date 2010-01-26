#include "Equ_Server.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Client
{
	Ecore_Con_Client *client;
};

typedef struct _Equ_Host_Get_Cb
{
	char *name;
	Equ_Reply_Host_Get *reply;
	Equ_Error error;
} Equ_Host_Get_Cb;

typedef Equ_Error (*Equanime_Message_Cb)(Equ_Client *c, void *msg, void **reply);

static int _hosts_get_cb(void *data, void *cb_data)
{
	Equ_Host *h = data;
	Equ_Reply_Hosts_Get *reply = cb_data;

	reply->ids_count++;
	reply->ids = realloc(reply->ids, reply->ids_count * sizeof(Equ_Common_Id));
	reply->ids[reply->ids_count - 1] = equ_host_id_get(h);

	return EINA_TRUE;
}

static int _host_get_cb(void *data, void *cb_data)
{
	Equ_Host *h = data;
	Equ_Host_Get_Cb *hgc = cb_data;

	if (!strcmp(equ_host_name_get(h), hgc->name))
	{
		hgc->reply->id = equ_host_id_get(h);
		hgc->error = EQU_ERR_NONE;
		return EINA_FALSE;
	}

	return EINA_TRUE;
}

static int _controllers_get_cb(void *data, void *cb_data)
{
	Equ_Controller *c = data;
	Equ_Reply_Controllers_Get *reply = cb_data;

	reply->ids_count++;
	reply->ids = realloc(reply->ids, reply->ids_count * sizeof(Equ_Common_Id));
	reply->ids[reply->ids_count - 1] = equ_controller_id_get(c);

	return EINA_TRUE;
}

static int _layers_get_cb(void *data, void *cb_data)
{
	Equ_Layer *c = data;
	Equ_Reply_Layers_Get *reply = cb_data;

	reply->ids_count++;
	reply->ids = realloc(reply->ids, reply->ids_count * sizeof(Equ_Common_Id));
	reply->ids[reply->ids_count - 1] = equ_layer_id_get(c);

	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Equ_Error equ_client_hosts_get(Equ_Client *client, Equ_Message_Hosts_Get *mhg,
		Equ_Reply_Hosts_Get **reply)
{
	Equ_Reply_Hosts_Get *rhg;

	rhg = *reply = calloc(1, sizeof(Equ_Reply_Hosts_Get));
	equ_hosts_get(_hosts_get_cb, rhg);

	return EQU_ERR_NONE;
}

EAPI Equ_Error equ_client_host_get(Equ_Client *client, Equ_Message_Host_Get *m,
		Equ_Reply_Host_Get **reply)
{
	Equ_Reply_Host_Get *r;
	Equ_Host_Get_Cb cb_data;

	r = *reply = calloc(1, sizeof(Equ_Reply_Host_Get));
	cb_data.reply = r;
	cb_data.name = m->name;
	cb_data.error = EQU_ERR_NEXIST;
	equ_hosts_get(_host_get_cb, &cb_data);

	if (cb_data.error)
	{
		free(r);
		*reply = NULL;
	}
	return cb_data.error;
}

EAPI Equ_Error equ_client_controllers_get(Equ_Client *client, Equ_Message_Controllers_Get *m,
		Equ_Reply_Controllers_Get **reply)
{
	Equ_Reply_Controllers_Get *r;
	Equ_Host *h;

	r = *reply = calloc(1, sizeof(Equ_Reply_Controllers_Get));
	h = equ_host_get(m->host_id);
	equ_host_controllers_get(h, _controllers_get_cb, r);

	return EQU_ERR_NONE;
}

EAPI Equ_Error equ_client_layers_get(Equ_Client *client, Equ_Message_Layers_Get *m,
		Equ_Reply_Layers_Get **reply)
{
	Equ_Reply_Layers_Get *r;
	Equ_Controller *c;

	r = *reply = calloc(1, sizeof(Equ_Reply_Layers_Get));
	c = equ_controller_get(m->controller_id);
	equ_controller_layers_get(c, _layers_get_cb, r);

	return EQU_ERR_NONE;
}

EAPI Equ_Error equ_client_controller_get(Equ_Client *client, Equ_Message_Host_Get *m,
		Equ_Reply_Host_Get **reply)
{
	Equ_Reply_Host_Get *r;
	Equ_Host_Get_Cb cb_data;

	r = *reply = calloc(1, sizeof(Equ_Reply_Host_Get));
	cb_data.reply = r;
	cb_data.name = m->name;
	cb_data.error = EQU_ERR_NEXIST;
	equ_hosts_get(_host_get_cb, &cb_data);

	if (cb_data.error)
	{
		free(r);
		*reply = NULL;
	}
	return cb_data.error;
}


EAPI Equ_Client * equ_client_new(Ecore_Con_Client *conn)
{
	Equ_Client *e;

	e = calloc(1, sizeof(Equ_Client));
	e->client = conn;

	return e;
}

/*
 * To trigger an error just set reply to NULL and return the error
 */
EAPI Equ_Error equ_client_process(Equ_Client *c, Equ_Message_Name name, void *msg,
		void **reply)
{
	Equ_Error err = EQU_ERR_NONE;

	switch (name)
	{
		case EQU_MSG_NAME_HOSTS_GET:
		err = equ_client_hosts_get(c, msg, reply);
		break;

		case EQU_MSG_NAME_HOST_GET:
		err = equ_client_host_get(c, msg, reply);
		break;

		case EQU_MSG_NAME_CONTROLLERS_GET:
		err = equ_client_controllers_get(c, msg, reply);
		break;

		case EQU_MSG_NAME_CONTROLLER_GET:
		err = equ_client_controller_get(c, msg, reply);
		break;

		case EQU_MSG_NAME_LAYERS_GET:
		err = equ_client_controllers_get(c, msg, reply);
		break;

		default:
		break;
	}
	return err;
}
