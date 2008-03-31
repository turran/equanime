#include <limits.h>
#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "equanime_common.h"
#include "Equanime.h"
#include "Equanime_Module.h"

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
		char filename[PATH_MAX];
		
		/* name */
		if (!strcmp(entry->d_name, "name"))
		{
			snprintf(filename, PATH_MAX, "%s/%s", sysfs_dirname, entry->d_name);
			_read_file(filename, d->name);
		}
		/* version */
		else if (!strcmp(entry->d_name, "version"))
		{
			snprintf(filename, PATH_MAX, "%s/%s", sysfs_dirname, entry->d_name);
			_read_file(filename, d->version);
		}
		/* maps */
		else if (!strcmp(entry->d_name, "maps"))
		{
			DIR *maps_dir;
			struct dirent *maps_entry;
			int index = 0;
			
			snprintf(filename, PATH_MAX, "%s/%s", sysfs_dirname, entry->d_name);
			/* iterate over the maps */
			maps_dir = opendir(filename);
			if (!maps_dir)
				goto failed;
			while (maps_entry = readdir(maps_dir))
			{
				char line[32]; /* this is enough to handle a 32bit or 64bit value */
				
				if ((!strcmp(maps_entry->d_name, ".")) || (!strcmp(maps_entry->d_name, "..")))
					continue;
				/* TODO check that the name is something like mapX */
				/* get each map's size and offset */
				snprintf(filename, PATH_MAX, "%s/maps/%s/addr", sysfs_dirname, maps_entry->d_name);
				if (!_read_file(filename, line))
					continue;
				sscanf(line,"0x%lx",&d->maps[index].addr);
				
				snprintf(filename, PATH_MAX, "%s/maps/%s/size", sysfs_dirname, maps_entry->d_name);
				if (!_read_file(filename, line))
					continue;
				sscanf(line,"0x%lx",&d->maps[index].size);
				index++;
			}
			d->maps_num = index;
		}
	}
	/* open the device */
	snprintf(device, 64, "/dev/uio%d", devnum);
	d->fd = open(device, O_RDONLY);
	if (d->fd < 0)
	{
		fprintf(stderr, "The sysfs entry exist, but no the /dev entry, mknod?\n");
		goto failed;
	}
	
	return d;
	
failed:
	free(d);
	return NULL;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * 
 */
EAPI Equanime_Hal_Device * equanime_hal_uio_open(const char *name)
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
		/* TODO check that the name is something like uioX */ 
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
					
					d = _parse_uio(uiox_name);
					// for now
					if (d)
						equanime_hal_uio_dump(d);
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
EAPI void * equanime_hal_uio_map(Equanime_Hal_Device *d, int map)
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
EAPI void equanime_hal_uio_close(Equanime_Hal_Device *d)
{
	close(d->fd);
	free(d);
}

/**
 * 
 */
EAPI void equanime_hal_uio_dump(Equanime_Hal_Device *d)
{
	Equanime_Hal_Map *m;
	int i = 0;
	
	printf("Name: %s\n", d->name);
	printf("Version: %s\n", d->version);
	printf("Maps:\n");
	
	for (i = 0; i < d->maps_num; i++)
	{
		m = &d->maps[i];
		printf("\t%d: 0x%lx 0x%lx\n", i, m->addr, m->size);
	}
}
