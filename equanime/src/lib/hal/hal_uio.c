#include "Equanime.h"
#include "equanime_private.h"

#include <limits.h>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static int _read_file(char *filename, char *line)
{
	char *s;
	int i;
	FILE *file;
	
	file = fopen(filename, "r");
	if (!file)
		return 0;

	s = fgets(line, PATH_MAX, file);
	if (!s)
	{
		fclose(file);
		return 0;
	}

	for (i = 0; (*s) && (i < PATH_MAX); i++)
	{
		if (*s == '\n')
			*s = 0;
		s++;
	}
	fclose(file);
	return 1;
}

static Equanime_Hal_Device * _parse_uio(const char *sysfs_dirname)
{
	DIR *dir;
	struct dirent *entry;
	int fd;
	char device[64];
	int devnum;
	Equanime_Hal_Device *d;
	
	/* create the new device */
	d = calloc(1, sizeof(Equanime_Hal_Device));
	/* fill the device information */
	dir = opendir(sysfs_dirname);
	if (!dir)
		goto failed;
	/* get the device number */
	sscanf(sysfs_dirname, "/sys/class/uio/uio%d", &devnum);
	while (entry = readdir(dir))
	{
		/* name */
		/* version */
		//if (!strcmp(entry-))
		/* iterate over the maps */
	}
	/* open the device */
	snprintf(device, 64, "/dev/uio%d", devnum);
	d->fd = open(device, O_RDONLY);
	if (d->fd < 0)
		goto failed;
	
	return d;
	
failed:
	free(d);
	return NULL;
}

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * 
 */
Equanime_Hal_Device * equanime_hal_uio_open(const char *name)
{
	DIR *uio;
	struct dirent *uio_de;

	uio = opendir("/sys/class/uio/");
	if (!uio) return NULL;
	
	/* get the different uioX devices */
	while (uio_de = readdir(uio))
	{
		DIR *uiox;
		struct dirent *uiox_de;
		char uiox_name[PATH_MAX];
		
		if ((!strcmp(uio_de->d_name, ".")) || (!strcmp(uio_de->d_name, "..")))
			continue;
		snprintf(uiox_name, PATH_MAX, "/sys/class/uio/%s", uio_de->d_name); 
		uiox = opendir(uiox_name);
		if (!uiox) continue;
		
		/* now go get the name */
		while (uiox_de = readdir(uiox))
		{
			if (!strcmp(uiox_de->d_name, "name"))
			{
				char filename[PATH_MAX];
				char line[PATH_MAX];
								
				snprintf(filename, PATH_MAX, "%s/%s", uiox_name, uiox_de->d_name);
				if (!_read_file(filename, line))
					return NULL;
				/* is it our name? */
				if (!strcmp(line, name))
				{
					Equanime_Hal_Device *d;
					
					printf("ok!!! uio found %s\n", name);
					d = _parse_uio(uiox_name);
					return d;
				}
			}
		}
	}
	/* should never reach here */
	return NULL;
}

/**
 * 
 */
void * equanime_hal_uio_map(Equanime_Hal_Device *d, int map)
{
	void *addr;
	
	addr = mmap(NULL, d->maps[map].size, PROT_READ, MAP_SHARED, d->fd, map*getpagesize());
	if (addr == MAP_FAILED)
		return NULL;
	return addr;
}

/**
 * 
 */
void equanime_hal_uio_close(Equanime_Hal_Device *d)
{
	close(d->fd);
	free(d);
}
