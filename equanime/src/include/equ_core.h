#ifndef _EQU_CORE_H
#define _EQU_CORE_H

/**
 * @defgroup Equanime_Core_Group Core
 * @{
 */
typedef struct _Equanime Equanime;
typedef struct _Equ_Host Equ_Host; /**< Opaque handler */
typedef struct _Equ_Controller Equ_Controller; /**< Opaque handler */

typedef int (*Equ_Cb)(void *data, void *user_data); /**< */
EAPI void equ_init(void);
EAPI void equ_shutdown(void);
EAPI Equanime * equ_new(int port);

/** @} */

#endif
