#ifndef _EQU_CORE_H
#define _EQU_CORE_H

/**
 * @ingroup Equanime_Client
 * @defgroup Equanime_Core_Group Core
 * @{
 */
typedef struct _Equanime Equanime; /**< Opaque handler */
typedef struct _Equ_Host Equ_Host; /**< Opaque handler */
typedef struct _Equ_Controller Equ_Controller; /**< Opaque handler */
typedef struct _Equ_Layer Equ_Layer; /**< Opaque handler */
typedef struct _Equ_Rotator Equ_Rotator;
typedef struct _Equ_Scaler Equ_Scaler;
typedef struct _Equ_Csc Equ_Csc;

typedef int (*Equ_Cb)(void *data, void *user_data); /**< Callback definition */

EAPI void equ_init(void);
EAPI void equ_shutdown(void);
EAPI Equanime * equ_new(int port);
EAPI void equ_sync(Equanime *e);

/** @} */

#endif
