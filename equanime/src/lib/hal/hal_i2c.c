#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Equanime.h"
#include "equanime_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
typedef struct _Equ_Hal_I2C Equ_Hal_I2C;
typedef struct _Equ_Hal_I2C_Adapter Equ_Hal_I2C_Adapter;

struct _Equ_Hal_I2C_Adapter
{
	int fd;
	Equ_Hal_I2C *curr;
};

struct _Equ_Hal_I2C
{
	Equ_Hal_I2C_Adapter *adapter;
	uint32_t address;
};

Eina_List *_adapters = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/* register every adapter found on the system */
void equ_hal_i2c_init(void)
{
	int i;
	char filename[20];
  
	for (i = 0; i < 255; i++)
	{
		Equ_Hal_I2C_Adapter *a;
		int fd;

		snprintf(filename, 19, "/dev/i2c-%d", i);
		/* check if the file exists */
		/* if so, try to open it and register the adapter */
		fd = open(filename, O_RDWR);
		if (fd < 0)
			continue;

		a = malloc(sizeof(Equ_Hal_I2C_Adapter));
		a->fd = fd;

		_adapters = eina_list_append(_adapters, a);
	}
}

/* unregister every adapter */
void equ_hal_i2c_shutdown(void)
{
	/* TODO for every adapter, clean it up, close the fd, etc */
	{
		Equ_Hal_I2C_Adapter *a;

		close(a->fd);
		free(a);
	}
}


Equ_Hal_I2C * equ_hal_i2c_get(int addr)
{
	Eina_List *l;


	{
		Equ_Hal_I2C_Adapter *a;

		if (!ioctl(a->fd, I2C_SLAVE, addr))
		{
			Equ_Hal_I2C *d;

			d = malloc(sizeof(Equ_Hal_I2C));

			d->adapter = a;
			d->address = addr;

			return d;
		}
	}
	return NULL;
}

int32_t equ_hal_i2c_read(Equ_Hal_I2C *d, uint8_t reg)
{

}

Eina_Bool equ_hal_i2c_write(Equ_Hal_I2C *d, uint8_t reg, uint32_t data)
{

}
