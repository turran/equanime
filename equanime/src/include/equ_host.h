#ifndef _EQU_HOST_H
#define _EQU_HOST_H

/**
 * @defgroup Equanime_Host_Group Host
 * @{
 */
typedef struct _Equ_Host Equ_Host;

EAPI void equ_hosts_get(Equ_Cb cb, void *cb_data);

EAPI const char * equ_host_name_get(Equ_Host *h);
EAPI void equ_host_controllers_get(Equ_Host *h, Equ_Cb cb, void *cb_data);

/** @} */

#endif
