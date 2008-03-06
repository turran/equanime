#ifndef EQUANIME_PRIVATE_H_
#define EQUANIME_PRIVATE_H_

#include <stdlib.h>

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
	char name[MAX_NAME_SIZE];
	char version[MAX_NAME_SIZE];
	int num;
	Equanime_Hal_Map maps[MAX_MAPS];
	int fd;
} Equanime_Hal_Device;

typedef struct _Equanime_Controller_Functions
{
	
} Equanime_Controller_Functions;

struct _Equanime_Controller_Description
{
	const char *name;
	int num_layers;
	Equanime_Controller_Functions *fncs;
};

typedef struct _Equanime_Layer_Functions
{
	
} Equanime_Layer_Functions;

struct _Equanime_Layer_Description
{
	const char *name;
	Equanime_Layer_Functions *fncs;
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
	Equanime_Layer *layers;
	const Equanime_Controller_Description *desc;
	void *data;
};

#endif /*EQUANIME_PRIVATE_H_*/
