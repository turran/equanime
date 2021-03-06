#ifndef EQU_SERVER_H_
#define EQU_SERVER_H_

#include "Eix.h"
#include "Eina.h"
#include "Eet.h"

/**
 * @defgroup Equanime_Server Server Side API
 * @{
 */

typedef int (*Equ_Cb)(void *data, void *user_data); /**< */

typedef struct _Equ_Input Equ_Input;
typedef struct _Equ_Output Equ_Output;
typedef struct _Equ_Rotator Equ_Rotator;
typedef struct _Equ_Controller Equ_Controller;
typedef struct _Equ_Layer Equ_Layer;
typedef struct _Equ_Surface Equ_Surface;
typedef struct _Equ_Component Equ_Component;
typedef struct _Equ_Host Equ_Host;
typedef struct _Equ_Scaler Equ_Scaler;
typedef struct _Equ_Csc Equ_Csc;
typedef struct _Equ_Region Equ_Region;
typedef struct _Equ_Pool Equ_Pool;
typedef struct _Equ_Client Equ_Client;

typedef struct _Equ_Controller_Backend Equ_Controller_Backend;
typedef struct _Equ_Layer_Backend  Equ_Layer_Backend;
typedef struct _Equ_Output_Backend  Equ_Output_Backend;
typedef struct _Equ_Input_Backend  Equ_Input_Backend;
typedef struct _Equ_Region_Backend  Equ_Region_Backend;
typedef struct _Equ_Host_Backend Equ_Host_Backend;
typedef struct _Equ_Pool_Backend Equ_Pool_Backend;
typedef struct _Equ_Server_Backend Equ_Server_Backend;

typedef struct _Equ_Hal_I2C Equ_Hal_I2C;

#include "equ_enums.h"
#include "equ_common.h"

typedef enum _Equ_Option_Type
{
	EQU_OPTION_INT,
	EQU_OPTION_STRING,
	EQU_OPTION_BOOL,
} Equ_Option_Type;

typedef struct _Equ_Option
{
	const char *def;
	const char *name;
	const char *description;
} Equ_Option;

EAPI void equ_option_parse(Equ_Option *option, char *parse,
		Equ_Option_Type val, void *dest);

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
	Eina_Bool (*init)(Equ_Host *, Equ_Server_Backend *sbackend, const char *options);
	void (*shutdown)(Equ_Host *);
	Equ_Option * (*options_get)(Equ_Host *h);
	Equ_Surface * (*surface_new)(Equ_Host *h, Equ_Common_Surface *s);
	void (*surface_delete)(Equ_Host *h, Equ_Surface *s);
	void (*surface_upload)(Equ_Host *h, Equ_Surface *s, Equ_Surface_Data *data, Eina_Rectangle *rect);
	void (*surface_download)(Equ_Host *h, Equ_Surface *s, Equ_Surface_Data *data, Eina_Rectangle *rect);
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

struct _Equ_Pool_Backend
{
	void * (*alloc)(Equ_Pool *p, size_t bytes);
	void (*free)(Equ_Pool *p, void *data);
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
	Eina_Bool (*size_set)(Equ_Layer *l, uint32_t w, uint32_t h);
	Eina_Bool (*position_set)(Equ_Layer *l, int x, int y);
	Eina_Bool (*visibility_set)(Equ_Layer *l, Eina_Bool show);
	Eina_Bool (*format_set)(Equ_Layer *l, Equ_Format fmt);
	Eina_Bool (*surface_put)(Equ_Layer *l, Equ_Surface *s, int x, int y, Eina_Rectangle *rect);
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
struct _Equ_Server_Backend
{
	void (*quit)(void);
	/* TODO do we suppose that ecore will handle
	 * the main loop always? if so, we dont need
	 * to abstract different calls like timer_add
	 * fd_read, whatever, if not, define some
	 * functions here
	 */
};

Equ_Output * equ_output_new(Equ_Controller *c,
		const char *name, Equ_Output_Backend *ob);
Equ_Controller * equ_output_controller_get(Equ_Output *o);

Equ_Controller * equ_host_controller_register(Equ_Host *h,
		const char *name, Equ_Controller_Backend *backend);
const char * equ_host_name_get(Equ_Host *h);
Equ_Host * equ_host_get(Equ_Common_Id id);
void * equ_host_data_get(Equ_Host *h);
Equ_Surface * equ_host_surface_get(Equ_Host *host, uint32_t w, uint32_t h,
		Equ_Format fmt, Equ_Surface_Type type);
Equ_Option * equ_host_options_get(Equ_Host *h);

Equ_Layer * equ_controller_layer_register(Equ_Controller *c,
		const char *name, Equ_Layer_Backend *lb,
		Equ_Layer_Caps *caps, Equ_Layer_Status *status);
void equ_controller_input_register(Equ_Controller *ec,
		Equ_Input_Backend *ib, const char *nane, void *data);
Equ_Output *equ_controller_output_register(Equ_Controller *c,
		const char *name, Equ_Output_Backend *backend);
void equ_controller_unregister(Equ_Controller *c);
void * equ_controller_data_get(Equ_Controller *c);
const char * equ_controller_name_get(Equ_Controller *c);
Equ_Controller * equ_controller_get(Equ_Common_Id id);
Equ_Controller * equ_controller_new(Equ_Host *h, const char *name,
		Equ_Controller_Backend *cb);

Equ_Layer * equ_layer_new(Equ_Controller *c,
		const char *name, Equ_Layer_Backend *lb,
		Equ_Layer_Caps *caps, Equ_Layer_Status *status);
const char * equ_layer_name_get(Equ_Layer *l);
void equ_layer_unregister(Equ_Layer *l);
void * equ_layer_data_get(Equ_Layer *l);
Equ_Layer * equ_layer_get(Equ_Common_Id id);

Equ_Surface * equ_surface_get(Equ_Common_Id id);

Equ_Surface * equ_surface_new(Equ_Host *host, Equ_Common_Surface *s, void *data);
Equ_Input * equ_input_new(Equ_Controller *c, Equ_Input_Backend *ib, const char *name, void *data);

Equ_Pool * equ_pool_new(Equ_Host *h, Equ_Pool_Backend *pb, const char *name, uint32_t mask);
void * equ_pool_alloc(Equ_Pool *p, size_t bytes);

Equ_Client * equ_client_new(Eix_Client *ec);
int equ_client_process(Equ_Client *c, unsigned int type, void *msg, void **rpely);


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

/** @} */

#endif /*EQU_SERVER_H_*/
