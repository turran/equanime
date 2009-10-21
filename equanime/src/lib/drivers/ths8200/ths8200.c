#include "Equanime.h"
#include "equanime_private.h"

#include "ths8200.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef enum ths8200_iif
{
	THS8200_IIF_30BIT,
	THS8200_IIF_16BIT,
	THS8200_IIF_15BIT,
	THS8200_IIF_20BIT,
	THS8200_IIF_10BIT,
} ths8200_iif;

typedef enum ths8200_std
{
	THS8200_1080P,
	THS8200_1080I,
	THS8200_720P,
	THS8200_HDTV,
	THS8200_480I,
	THS8200_480P,
	THS8200_VESA_MASTER,
	THS8200_VESA_SLAVE,
	THS8200_576I,
	THS8200_GENERIC,
} ths8200_std;

typedef struct ths8200_mode
{
	Equ_Standard estd;
	ths8200_iif iif;
	ths8200_std std;
	uint16_t frame_size;
	uint16_t field_size;
	uint16_t total_pixels;
} ths8200_mode;

/* static modes */
static ths8200_mode _modes[] = {

};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool ths8200_mode_set(Equ_Hal_I2C *d, Equ_Mode *m)
{
	/* TODO get the matching internal mode */
	/* TODO set the registers to those values */
}
