#ifndef EQUANIME_PRIVATE_H_
#define EQUANIME_PRIVATE_H_

typedef struct _Equ_Controller_Backend Equ_Controller_Backend;
typedef struct _Equ_Layer_Backend  Equ_Layer_Backend;
typedef struct _Equ_Output_Backend  Equ_Output_Backend;
typedef struct _Equ_Input_Backend  Equ_Input_Backend;
typedef struct _Equ_Region_Backend  Equ_Region_Backend;
typedef struct _Equ_Host_Backend Equ_Host_Backend;
typedef struct _Equ_Hal_I2C Equ_Hal_I2C;

struct _Equ_Host_Backend
{

};

struct _Equ_Input_Backend
{
	Eina_Bool (*timing_set)(Equ_Input *i, Equ_Timing *t);
	Eina_Bool (*surface_set)(Equ_Input *i, Equ_Surface *s);
};

struct _Equ_Output_Backend
{
	Eina_Bool (*timing_set)(Equ_Output *o, Equ_Timing *t);
};

/**
 * Backend every controller should implement based on the description
 * flags
 */
struct _Equ_Controller_Backend
{
	Eina_Bool (*output_set)(Equ_Controller *c, Equ_Output *o);
};

/**
 * Backend every layer should implement based on the description
 * flags
 */
struct _Equ_Layer_Backend
{
	Eina_Bool (*size_set)(Equ_Layer *l, unsigned int w, unsigned int h);
	Eina_Bool (*position_set)(Equ_Layer *l, int x, int y);
	Eina_Bool (*visibility_set)(Equ_Layer *l, Eina_Bool show);
	Eina_Bool (*surface_set)(Equ_Layer *l, Equ_Surface *s);
	//Equ_Format * (*format_get)(Equ_Layer *l, int num_formats);
	//Eina_Bool (*input_set)(Equ_Layer *l, Equ_Input *i);
};

/**
 * Backend every region should implement based on the description
 * flags
 */
struct _Equ_Region_Backend
{
};

/**
 *
 */
struct _Equ_Rotator
{
	Equ_Angle angle;
};

/**
 *
 */
struct _Equ_Scaler
{
	uint32_t sw, sh, dw, dh;
};

struct _Equ_Csc
{
	Equ_Format sf;
	Equ_Format df;
};

typedef struct _Equ_Component_Data
{
	Equ_Component_Type type;
	void *data;
} Equ_Component_Data;

typedef struct _Equ_Component_Backend
{
	Equ_Component_Data *data[EQU_COMPONENT_TYPES];
	Eina_Bool (*setup)(Equ_Component *c);
	void (*run)(Equ_Component *c);
} Equ_Component_Backend;

/**
 *
 */
struct _Equ_Layer
{
	Equ_Controller *controller;
	const Equ_Layer_Backend *backend;
	int flags; /** Layer flags */
	const int *formats; /** Supported pixel formats */
	int x;
	int y;
	int w;
	int h;
	int level;
	unsigned char hidden;
	Equ_Surface *surface;
	void *data;
};
/**
 *
 */
struct _Equ_Region
{
	Equ_Layer *layer;
	const Equ_Region_Backend *backend;
	int x;
	int y;
	int w;
	int h;
	int level;
	unsigned char hidden;
	void *data;
};
/**
 *
 */
struct _Equ_Controller
{
	const Equ_Controller_Backend *backend;
	Eina_List *layers;
	Eina_List *outputs;
	Eina_List *inputs;
	void *data;
};
/**
 *
 */
struct _Equ_Output
{
	Equ_Output_Backend *backend;
	void *data;
};
/**
 *
 */
struct _Equ_Input
{
	Equ_Input_Backend *backend;
	void *data;
};

/**
 *
 */
struct _Equ_Surface
{
	Equ_Surface_Type type;
	Equ_Format fmt;
};

void equ_controller_layer_unregister(Equ_Layer *el);

Equ_Controller * equ_controller_register(Equ_Controller_Backend *backend, void *data);
void equ_controller_unregister(Equ_Controller *c);

Equ_Layer * equ_controller_layer_register(Equ_Controller *ec, Equ_Layer_Backend *lb, void *data);
void equ_controller_output_register(Equ_Controller *ec, Equ_Output_Backend *ob, void *data);
void equ_controller_input_register(Equ_Controller *ec, Equ_Input_Backend *ib, void *data);

void equ_layer_unregister(Equ_Layer *l);
Equ_Layer * equ_layer_new(Equ_Controller *c, Equ_Layer_Backend *lb, void *data);
void * equ_layer_data_get(Equ_Layer *l);

/* for now place hal info here */
/* maybe place the uio stuff in another header ? */
#define MAX_NAME_SIZE 64
#define MAX_MAPS 5

typedef struct _Equanime_Hal_Map
{
	unsigned long addr;
	int size;
	void *ptr;
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
EAPI void equanime_hal_uio_unmap(Equanime_Hal_Device *d, int map);
EAPI void equanime_hal_uio_close(Equanime_Hal_Device *d);
EAPI void equanime_hal_uio_dump(Equanime_Hal_Device *d);

void equ_hal_i2c_init(void);
void equ_hal_i2c_shutdown(void);
Equ_Hal_I2C * equ_hal_i2c_get(int addr);

#endif /*EQUANIME_PRIVATE_H_*/
