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
	reply->hosts[reply->hosts_count - 1].name = equ_host_name_get(h);
	reply->hosts[reply->hosts_count - 1].id = reply->hosts_count + 100;
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
	/* get the data */
	equ_hosts_get(_hosts_get_cb, rhg);

	return EQU_ERR_NONE;
}

EAPI Equ_Client * equ_client_new(Ecore_Con_Client *conn)
{
	Equ_Client *e;

	e = calloc(1, sizeof(Equ_Client));
	e->client = conn;

	return e;
}

EAPI Equ_Error equ_client_process(Equ_Client *c, Equ_Message_Name name, void *msg,
		void **reply)
{
	Equ_Error err;

	switch (name)
	{
		case EQU_MSG_NAME_HOSTS_GET:
		err = equ_client_hosts_get(c, msg, reply); 
		break;

		default:
		break;
	}
	return err;
}
