#ifndef EQUANIME_MODULE_H_
#define EQUANIME_MODULE_H_

typedef struct _Equanime_Controller_Functions Equanime_Controller_Functions;
typedef struct _Equanime_Layer_Functions  Equanime_Layer_Functions;
typedef struct _Equanime_Region_Functions  Equanime_Region_Functions;

/**
 * Functions every controller module should implement based on the description
 * flags
 */
struct _Equanime_Controller_Functions
{
	int (*probe)(Equanime_Controller *c);
	void (*remove)(Equanime_Controller *c);
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
struct _Equanime_Layer_Functions
{
	int (*probe)(Equanime_Layer *l);
	void (*remove)(Equanime_Layer *l);
	int (*size_set)(Equanime_Layer *l, int w, int h);
	int (*position_set)(Equanime_Layer *l, int x, int y);
	int (*visibility_set)(Equanime_Layer *l, int show);
	void *(*ptr_get)(Equanime_Layer *l);
	/* possible functions:
	 * format_set
	 */
};
/**
 * Functions every region module should implement based on the description
 * flags
 */
struct _Equanime_Region_Functions
{
	int (*probe)(Equanime_Layer *l);
	void (*remove)(Equanime_Layer *l);
	/* possible functions:
	 * enable a layer
	 * change a layer priority
	 * if the above cases are correct then the controller should know
	 * every layer name or something.
	 */
};

EAPI int equanime_controller_register(Equanime_Controller_Description *cd, Equanime_Controller_Functions *cf);
EAPI void equanime_controller_unregister(Equanime_Controller_Description *cd);
EAPI void equanime_controller_data_set(Equanime_Controller *ec, void *data);
EAPI void * equanime_controller_data_get(Equanime_Controller *ec);

EAPI int equanime_layer_register(Equanime_Layer_Description *ld, Equanime_Layer_Functions *lf);
EAPI void equanime_layer_unregister(Equanime_Layer_Description *ld);
EAPI void equanime_layer_data_set(Equanime_Layer *el, void *data);
EAPI void * equanime_layer_data_get(Equanime_Layer *el);


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

#endif /*EQUANIME_MODULE_H_*/
