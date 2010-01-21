#include "Equ_Server.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
struct _Equ_Client
{
	Ecore_Con_Client *client;
};

static int _hosts_get_cb(void *data, void *cb_data)
{
	Equ_Host *h = data;
	Equ_Reply_Hosts_Get *reply = cb_data;

	reply->hosts_num++;
	reply->hosts = realloc(reply->hosts, reply->hosts_num);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI void equ_client_hosts_get(Equ_Client *client)
{
	Equ_Reply_Hosts_Get reply;

	/* get the data */
	reply.hosts_num = 0;
	equ_hosts_get(_hosts_get_cb, &reply);
	/* send the reply */
}
