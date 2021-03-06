#ifndef _EQU_HOST_H
#define _EQU_HOST_H

/**
 * @ingroup Equanime_Client
 * @defgroup Equanime_Host_Group Host
 * @{
 */

EAPI void equ_hosts_get(Equanime *e, Equ_Cb cb, void *cb_data);
EAPI void equ_host_delete(Equ_Host *h);
EAPI Equ_Host * equ_host_get(Equanime *e, const char *name);
EAPI const char * equ_host_name_get(Equanime *e, Equ_Host *h);

EAPI void equ_host_controllers_get(Equanime *e, Equ_Host *h, Equ_Cb cb, void *cb_data);
EAPI Equ_Controller * equ_host_controller_get(Equanime *e, Equ_Host *h, const char *name);

EAPI Equ_Surface * equ_host_surface_get(Equanime *e, Equ_Host *h, uint32_t width, uint32_t height,
		Equ_Format fmt, Equ_Surface_Type type);

/** @} */

#endif
