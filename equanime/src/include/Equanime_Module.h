#ifndef EQUANIME_MODULE_H_
#define EQUANIME_MODULE_H_

typedef struct _Equanime_Controller_Functions
{
	int (*probe)(Equanime_Controller *c);
	void (*remove)(Equanime_Controller *c);
} Equanime_Controller_Functions;


typedef struct _Equanime_Layer_Functions
{
	int (*probe)(Equanime_Layer *l);
	void (*remove)(Equanime_Layer *l);
} Equanime_Layer_Functions;

EAPI int equanime_controller_register(Equanime_Controller_Description *cd, Equanime_Controller_Functions *cf);
EAPI void equanime_controller_unregister(Equanime_Controller_Description *cd);

EAPI void equanime_controller_data_set(Equanime_Controller *ec, void *data);
EAPI void * equanime_controller_data_get(Equanime_Controller *ec);

EAPI int equanime_layer_register(Equanime_Layer_Description *ld, Equanime_Layer_Functions *lf);
EAPI void equanime_layer_unregister(Equanime_Layer_Description *ld);


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

EAPI Equanime_Hal_Device * equanime_hal_uio_open(const char *name);
EAPI void * equanime_hal_uio_map(Equanime_Hal_Device *d, int map);
EAPI void equanime_hal_uio_close(Equanime_Hal_Device *d);
EAPI void equanime_hal_uio_dump(Equanime_Hal_Device *d);




#endif /*EQUANIME_MODULE_H_*/
