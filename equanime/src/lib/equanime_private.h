#ifndef EQUANIME_PRIVATE_H_
#define EQUANIME_PRIVATE_H_

#include <stdlib.h>

#define MAX_NAME_SIZE 64
#define MAX_MAPS 5

typedef struct _Equanime_Hal_Map
{
	unsigned long addr;
	int size;
	int mmap_result;
} Equanime_Hal_Map;

struct _Equanime_Hal
{
	char name[MAX_NAME_SIZE];
	char version[MAX_NAME_SIZE];
	int num;	
	Equanime_Hal_Map maps[MAX_MAPS]
};

struct _Equanime_Layer_Description
{
	const char *name;
};

struct _Equanime_Controller_Description
{
	const char *name;
	int num_layers;
};

typedef struct _Equanime_Layer_Functions
{
	
} Equanime_Layer_Functions;

struct _Equanime_Layer
{
	Equanime_Controller *controller;
	const Equanime_Layer_Description *desc;
	Equanime_Layer_Functions *fncs;
};

struct _Equanime_Region
{
	
};

typedef struct _Equanime_Controller_Functions
{
	
} Equanime_Controller_Functions;

struct _Equanime_Controller
{
	Equanime_Layer *layers;
	const Equanime_Controller_Description *desc;
	Equanime_Controller_Functions *fncs;
};

#endif /*EQUANIME_PRIVATE_H_*/
