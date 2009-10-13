#ifndef _EQU_CORE_H
#define _EQU_CORE_H

/**
 * @defgroup Equanime_Core_Group Core
 * @{
 */
typedef int (*Equ_Cb)(void *data, void *user_data); /**< */
EAPI void equ_init(void);
EAPI void equ_shutdown(void);

/** @} */

#endif
