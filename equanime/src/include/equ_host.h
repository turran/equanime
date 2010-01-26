#ifndef _EQU_HOST_H
#define _EQU_HOST_H

/**
 * @defgroup Equanime_Host_Group Host
 * @{
 */
typedef struct _Equ_Host Equ_Host;

EAPI void equ_hosts_get(Equanime *e, Equ_Cb cb, void *cb_data);
EAPI Equ_Host * equ_host_get(Equanime *e, const char *name);

EAPI const char * equ_host_name_get(Equanime *e, Equ_Host *h);
EAPI void equ_host_controllers_get(Equanime *e, Equ_Host *h, Equ_Cb cb, void *cb_data);

/** @} */

#endif
