#ifndef EQUANIME_PRIVATE_H_
#define EQUANIME_PRIVATE_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct _Equanime_Controller_Functions
{
	int (*probe)(Equanime_Controller *c);
	void (*remove)(Equanime_Controller *c);
} Equanime_Controller_Functions;

struct _Equanime_Controller_Description
{
	const char *name;
	Equanime_Controller_Functions fncs;
};

typedef struct _Equanime_Layer_Functions
{
	int (*probe)(Equanime_Layer *l);
	void (*remove)(Equanime_Layer *l);
} Equanime_Layer_Functions;

struct _Equanime_Layer_Description
{
	const char *cname; /** Controller name */
	const char *name; /** Layer name */
	Equanime_Layer_Functions fncs;
};

struct _Equanime_Layer
{
	Equanime_Controller *controller;
	const Equanime_Layer_Description *desc;
	void *data;
};

struct _Equanime_Region
{
	
};

struct _Equanime_Controller
{
	Equanime_Layer **layers;
	int num_layers;
	const Equanime_Controller_Description *desc;
	void *data;
};

void equanime_controller_register(Equanime_Controller_Description *cd);
void equanime_controller_unregister(Equanime_Controller_Description *cd);

void equanime_controller_data_set(Equanime_Controller *ec, void *data);
void * equanime_controller_data_get(Equanime_Controller *ec);

void equanime_layer_register(Equanime_Layer_Description *ld);
void equanime_layer_unregister(Equanime_Layer_Description *ld);


/* for now place hal info here */
/* maybe place the uio stuff in another header ? */
#define MAX_NAME_SIZE 64
#define MAX_MAPS 5

typedef struct _Equanime_Hal_Map
{
	unsigned long addr;
	int size;
	int mmap_result;
} Equanime_Hal_Map;

typedef struct _Equanime_Hal_Device
{
	int fd;
	char name[MAX_NAME_SIZE];
	char version[MAX_NAME_SIZE];
	int number;
	Equanime_Hal_Map maps[MAX_MAPS];
	int maps_num;
	
} Equanime_Hal_Device;

Equanime_Hal_Device * equanime_hal_uio_open(const char *name);
void * equanime_hal_uio_map(Equanime_Hal_Device *d, int map);
void equanime_hal_uio_close(Equanime_Hal_Device *d);
void equanime_hal_uio_dump(Equanime_Hal_Device *d);

#endif /*EQUANIME_PRIVATE_H_*/
