#ifndef EQUANIME_PRIVATE_H_
#define EQUANIME_PRIVATE_H_

typedef struct _Equ_Controller_Functions Equ_Controller_Functions;
typedef struct _Equ_Layer_Functions  Equ_Layer_Functions;
typedef struct _Equ_Region_Functions  Equ_Region_Functions;

/**
 * Functions every controller module should implement based on the description
 * flags
 */
struct _Equ_Controller_Functions
{
	int (*probe)(Equ_Controller *c);
	void (*remove)(Equ_Controller *c);
	/* possible functions:
	 * enable a layer
	 * change a layer priority
	 * if the above cases are correct then the controller should know
	 * every layer name or something.
	 */
};
/**
 * Functions every layer module should implement based on the description
 * flags
 */
struct _Equ_Layer_Functions
{
	int (*probe)(Equ_Layer *l);
	void (*remove)(Equ_Layer *l);
	int (*size_set)(Equ_Layer *l, int w, int h);
	int (*position_set)(Equ_Layer *l, int x, int y);
	int (*visibility_set)(Equ_Layer *l, int show);
	void *(*ptr_get)(Equ_Layer *l);
	/* possible functions:
	 * format_set
	 */
};
/**
 * Functions every region module should implement based on the description
 * flags
 */
struct _Equ_Region_Functions
{
	int (*probe)(Equ_Layer *l);
	void (*remove)(Equ_Layer *l);
	/* possible functions:
	 * enable a layer
	 * change a layer priority
	 * if the above cases are correct then the controller should know
	 * every layer name or something.
	 */
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
	Eina_Inlist list;
	Equ_Controller *controller;
	const Equ_Layer_Description *desc;
	const Equ_Layer_Functions *fncs;
	int x;
	int y;
	int w;
	int h;
	int level;
	unsigned char hidden;
	void *data;
	void *ptr; /* TODO remove this? */
	//Equ_Surface *surface;
	unsigned int surface_ref; /* number of times the surface has been get */
};
/**
 * 
 */
struct _Equ_Region
{
	Eina_Inlist list;
	Equ_Layer *layer;
	const Equ_Region_Description *desc;
	const Equ_Region_Functions *fncs;
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
	Eina_Inlist list;
	Equ_Layer **layers;
	int num_layers;
	const Equ_Controller_Description *desc;
	const Equ_Controller_Functions *fncs;
	void *data;
};
/**
 * 
 */
struct _Equ_Output
{
	
};
/**
 * 
 */
struct _Equ_Input
{
	
};

/**
 * 
 */
struct _Equ_Surface
{
	Equ_Surface_Type type;
};

void equ_controller_layer_register(Equ_Controller *ec, Equ_Layer *el);
void equ_controller_layer_unregister(Equ_Layer *el);
Equ_Controller * equ_controller_name_get_by(const char *name);


EAPI int equ_controller_register(Equ_Controller_Description *cd, Equ_Controller_Functions *cf);
EAPI void equ_controller_unregister(Equ_Controller_Description *cd);
EAPI void equ_controller_data_set(Equ_Controller *ec, void *data);
EAPI void * equ_controller_data_get(Equ_Controller *ec);

EAPI int equ_layer_register(Equ_Layer_Description *ld, Equ_Layer_Functions *lf);
EAPI void equ_layer_unregister(Equ_Layer_Description *ld);
EAPI void equ_layer_data_set(Equ_Layer *el, void *data);
EAPI void * equ_layer_data_get(Equ_Layer *el);


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

/* write, read */
static inline unsigned char readb(const volatile void *addr)
{
	return *(volatile unsigned char *) addr;
}

static inline unsigned short readw(const volatile void *addr)
{
	return *(volatile unsigned short *) addr;
}

static inline unsigned int readl(const volatile void *addr)
{
	return *(volatile unsigned int *) addr;
}

static inline void writeb(volatile void *addr, unsigned char b)
{
	*(volatile unsigned char *) addr = b;
}

static inline void writew(volatile void *addr, unsigned short s)
{
	*(volatile unsigned short *) addr = s;
}

static inline void writel(volatile void *addr, unsigned int i)
{
	*(volatile unsigned int *) addr = i;
}

#endif /*EQUANIME_PRIVATE_H_*/
