#ifndef EQU_SERVER_H_
#define EQU_SERVER_H_

#include "Eina.h"
#include "Eet.h"

#include "equ_enums.h"
#include "equ_common.h"

typedef struct _Equ_Input Equ_Input;
typedef struct _Equ_Output Equ_Output;
typedef struct _Equ_Rotator Equ_Rotator;
typedef struct _Equ_Controller Equ_Controller;
typedef struct _Equ_Layer Equ_Layer;
typedef struct _Equ_Surface Equ_Surface;
typedef struct _Equ_Component Equ_Component;
typedef struct _Equ_Host Equ_Host;

typedef struct _Equ_Controller_Backend Equ_Controller_Backend;
typedef struct _Equ_Layer_Backend  Equ_Layer_Backend;
typedef struct _Equ_Output_Backend  Equ_Output_Backend;
typedef struct _Equ_Input_Backend  Equ_Input_Backend;
typedef struct _Equ_Region_Backend  Equ_Region_Backend;
typedef struct _Equ_Host_Backend Equ_Host_Backend;
typedef struct _Equ_Hal_I2C Equ_Hal_I2C;

typedef enum _Equ_Standard
{
	EQU_STANDARD_CUSTOM,
	EQU_STANDARD_PAL,
	EQU_STANDARD_NTSC,
	EQU_STANDARD_480P,
	EQU_STANDARD_720P,
	EQU_STANDARD_1080I,
	EQU_STANDARD_1080P,
	EQU_STANDARDS,
} Equ_Standard;

typedef enum _Equ_Video_Mode
{
	EQU_VIDEO_MODE_PROGRESSIVE,
	EQU_VIDEO_MODE_INTERLACED,
	EQU_VIDEO_MODES,
} Equ_Video_Mode;

/* the timing struct is similar to what fb.h provides */
typedef struct _Equ_Timing
{
	uint32_t pixclock;	/* pixel clock in ps (pico seconds) */
	uint32_t left_margin;	/* time from sync to picture */
	uint32_t right_margin;	/* time from picture to sync */
	uint32_t upper_margin;	/* time from sync to picture */
	uint32_t lower_margin;
	uint32_t hsync_len;	/* length of horizontal sync */
	uint32_t vsync_len;	/* length of vertical sync */
} Equ_Timing;

typedef struct _Equ_Mode
{
	Equ_Standard std;
	Equ_Timing t;
	Equ_Video_Mode vmode;
	unsigned int xres;
	unsigned int yres;
} Equ_Mode;

struct _Equ_Host_Backend
{

};

struct _Equ_Input_Backend
{
	Eina_Bool (*mode_set)(Equ_Input *i, Equ_Mode *m);
	Eina_Bool (*surface_set)(Equ_Input *i, Equ_Surface *s);
	/* TODO add a way to get the possible modes */
};

struct _Equ_Output_Backend
{
	Eina_Bool (*mode_set)(Equ_Output *o, Equ_Mode *m);
	/* TODO add a way to get the possible modes */
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
	const char *name;
	const Equ_Layer_Backend *backend;
	void *data;

	int flags; /** Layer flags */
	const int *formats; /** Supported pixel formats */
	int x;
	int y;
	int level;
	unsigned char hidden;
	Equ_Surface *surface;
};
/**
 *
 */
struct _Equ_Region
{
	Equ_Layer *layer;
	const Equ_Region_Backend *backend;
	void *data;

	int x;
	int y;
	int w;
	int h;
	int level;
	unsigned char hidden;
};
/**
 *
 */
struct _Equ_Controller
{
	const Equ_Controller_Backend *backend;
	const char *name;
	void *data;

	Equ_Host *host;
	Eina_List *layers;
	Eina_List *outputs;
	Eina_List *inputs;
};
/**
 *
 */
struct _Equ_Output
{
	Equ_Output_Backend *backend;
	const char *name;
	void *data;

	Equ_Controller *controller;
	Equ_Mode mode;
};
/**
 *
 */
struct _Equ_Input
{
	Equ_Input_Backend *backend;
	const char *name;
	void *data;

	Equ_Mode *mode;
};

/**
 *
 */
struct _Equ_Surface
{
	Equ_Surface_Type type;
	Equ_Format fmt;
	unsigned int w;
	unsigned int h;
	unsigned int pitch;
	void *data;
};

void equ_controller_layer_unregister(Equ_Layer *el);

Equ_Controller * equ_host_controller_register(Equ_Host *h,
		Equ_Controller_Backend *backend, const char *name, void *data);

void equ_controller_unregister(Equ_Controller *c);
void * equ_controller_data_get(Equ_Controller *c);

Equ_Layer * equ_controller_layer_register(Equ_Controller *ec, Equ_Layer_Backend *lb, const char *name, void *data);
void equ_controller_output_register(Equ_Controller *ec, Equ_Output_Backend *ob, const char *name, void *data);
void equ_controller_input_register(Equ_Controller *ec, Equ_Input_Backend *ib, const char *nane, void *data);

void equ_layer_unregister(Equ_Layer *l);
Equ_Layer * equ_layer_new(Equ_Controller *c, Equ_Layer_Backend *lb, const char *name, void *data);
void * equ_layer_data_get(Equ_Layer *l);

Equ_Output * equ_output_new(Equ_Controller *c, Equ_Output_Backend *ob, const char *name, void *data);

Equ_Input * equ_input_new(Equ_Controller *c, Equ_Input_Backend *ib, const char *name, void *data);

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
