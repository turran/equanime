#include "Equ_Server.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Client
{
	Ecore_Con_Client *client;
};

typedef Equ_Error (*Equanime_Message_Cb)(Equ_Client *c, void *msg, void **reply);

static int _hosts_get_cb(void *data, void *cb_data)
{
	Equ_Host *h = data;
	Equ_Reply_Hosts_Get *reply = cb_data;

	reply->hosts_count++;
	reply->hosts = realloc(reply->hosts, reply->hosts_count * sizeof(Equ_Common_Host));
	reply->hosts[reply->hosts_count - 1].id = equ_host_id_get(h);
	reply->hosts[reply->hosts_count - 1].name = equ_host_name_get(h);

	return EINA_TRUE;
}

static int _controllers_get_cb(void *data, void *cb_data)
{
	Equ_Controller *c = data;
	Equ_Reply_Controllers_Get *reply = cb_data;

	reply->controllers_count++;
	reply->controllers = realloc(reply->controllers, reply->controllers_count * sizeof(Equ_Common_Controller));
	reply->controllers[reply->controllers_count - 1].id = equ_controller_id_get(c);
	reply->controllers[reply->controllers_count - 1].name = equ_controller_name_get(c);

	return EINA_TRUE;
}

static int _layers_get_cb(void *data, void *cb_data)
{
	Equ_Layer *l = data;
	Equ_Reply_Layers_Get *reply = cb_data;

	reply->layers_count++;
	reply->layers = realloc(reply->layers, reply->layers_count * sizeof(Equ_Layer_Info));
	reply->layers[reply->layers_count - 1].id = equ_layer_id_get(l);
	reply->layers[reply->layers_count - 1].name = equ_layer_name_get(l);

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

EAPI Equ_Error equ_client_controllers_get(Equ_Client *client, Equ_Message_Controllers_Get *m,
		Equ_Reply_Controllers_Get **reply)
{
	Equ_Reply_Controllers_Get *r;
	Equ_Host *h;

	h = equ_host_get(m->host_id);
	if (!h)
		return EQU_ERR_NEXIST;
	r = *reply = calloc(1, sizeof(Equ_Reply_Controllers_Get));
	equ_host_controllers_get(h, _controllers_get_cb, r);

	return EQU_ERR_NONE;
}

EAPI Equ_Error equ_client_layers_get(Equ_Client *client, Equ_Message_Layers_Get *m,
		Equ_Reply_Layers_Get **reply)
{
	Equ_Reply_Layers_Get *r;
	Equ_Controller *c;

	c = equ_controller_get(m->controller_id);
	if (!c)
		return EQU_ERR_NEXIST;
	r = *reply = calloc(1, sizeof(Equ_Reply_Layers_Get));
	equ_controller_layers_get(c, _layers_get_cb, r);

	return EQU_ERR_NONE;
}

EAPI Equ_Error equ_client_layer_status_get(Equ_Client *client, Equ_Message_Layer_Status_Get *m,
		Equ_Reply_Layer_Status_Get **reply)
{
	Equ_Reply_Layer_Status_Get *r;
	Equ_Layer *l;

	l = equ_layer_get(m->layer_id);
	if (!l)
		return EQU_ERR_NEXIST;
	r = *reply = calloc(1, sizeof(Equ_Reply_Layer_Status_Get));
	equ_layer_status_get(l, &r->status);

	return EQU_ERR_NONE;
}

EAPI Equ_Error equ_client_layer_caps_get(Equ_Client *client, Equ_Message_Layer_Caps_Get *m,
		Equ_Reply_Layer_Caps_Get **reply)
{
	Equ_Reply_Layer_Caps_Get *r;
	Equ_Layer *l;

	l = equ_layer_get(m->layer_id);
	if (!l)
		return EQU_ERR_NEXIST;
	r = *reply = calloc(1, sizeof(Equ_Reply_Layer_Caps_Get));
	equ_layer_caps_get(l, &r->caps);

	return EQU_ERR_NONE;
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

		case EQU_MSG_NAME_CONTROLLERS_GET:
		err = equ_client_controllers_get(c, msg, reply);
		break;

		case EQU_MSG_NAME_LAYERS_GET:
		err = equ_client_layers_get(c, msg, reply);
		break;
		
		case EQU_MSG_NAME_LAYER_STATUS_GET:
		err = equ_client_layer_status_get(c, msg, reply);
		break;
		
		case EQU_MSG_NAME_LAYER_CAPS_GET:
		err = equ_client_layer_caps_get(c, msg, reply);
		break;

		default:
		break;
	}
	return err;
}
