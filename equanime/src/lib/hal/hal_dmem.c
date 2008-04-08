#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI Equanime_Hal_Device * equanime_hal_dmem_open(void)
{
	Equanime_Hal_Device *d;
		
	/* create the new device */
	d = calloc(1, sizeof(Equanime_Hal_Device));
	if ((d->fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
		return NULL;
	return d;
}

/**
 * 
 */
EAPI void * equanime_hal_dmem_map(Equanime_Hal_Device *d, unsigned long offset, unsigned long length)
{
	void *addr;
#if 0
	addr = mmap(0, , PROT_READ | PROT_WRITE, MAP_SHARED, d->fd, target & ~MAP_MASK);
	if (addr == MAP_FAILED)
		return NULL;
#endif
	return addr;
}

/**
 * 
 */
EAPI void equanime_hal_dmem_close(Equanime_Hal_Device *d)
{
	close(d->fd);
	free(d);
}
