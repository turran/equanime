#ifndef MP25XXF_H_
#define MP25XXF_H_

typedef struct _MP25XXF_Device
{
	
} MP25XXF_Device;

/* Controller */
void mp25xxf_controller_init(void);
void mp25xxf_controller_exit(void);
/* Layers */
void mp25xxf_rgb_init(void);
void mp25xxf_rgb_exit(void);


#endif /*MP25XXF_H_*/
