#ifndef ESHM_H_
#define ESHM_H_

#include <Eina.h>

/**
 * @mainpage Eshm
 * @section intro Introduction
 * Eshm is a library/daemon that serves as a shared memory
 * controller. Clients can connect to the daemon, request
 * segments, read and write to those segments, etc.
 * @section dependencies Dependencies
 * - Eina
 * - Eet
 * - Ecore
 * - Ecore_Ipc
 * @file
 * @brief Eshm API
 * @defgroup Eshm_Core_Group Core
 * @{
 */

extern int ESHM_ERROR_ACCESS;  /**< Not enough permission for such request */
extern int ESHM_ERROR_EXIST;   /**< a request for new segment that already exists */
extern int ESHM_ERROR_NEXIST;  /**< A request for a segment that no longer exists */
extern int ESHM_ERROR_CODEC;   /**< Bad encoding or decoding */
extern int ESHM_ERROR_TIMEOUT; /**< Timeout error */

EAPI int eshm_init(void);
EAPI void eshm_shutdown(void);

/**
 * @}
 * @defgroup Eshm_Segment_Group Segment
 * @{
 */
typedef struct _Eshm_Segment Eshm_Segment; /**< Segment opaque handler */

EAPI Eshm_Segment * eshm_segment_new(const char *id, size_t size);
EAPI Eshm_Segment * eshm_segment_get(const char *id, size_t size, Eina_Bool create);
EAPI size_t eshm_segment_size_get(Eshm_Segment *s);
EAPI Eina_Bool eshm_segment_lock(Eshm_Segment *s, Eina_Bool write);
EAPI void eshm_segment_unlock(Eshm_Segment *s);
EAPI void * eshm_segment_data_get(Eshm_Segment *s);

/**
 * @}
 */
#endif /*ESHM_H_*/
