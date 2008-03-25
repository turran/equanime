#ifndef EQUANIME_PRIVATE_H_
#define EQUANIME_PRIVATE_H_

struct _Equanime_Layer
{
	Eina_Inlist list;
	Equanime_Controller *controller;
	const Equanime_Layer_Description *desc;
	const Equanime_Layer_Functions *fncs;
	int x;
	int y;
	int w;
	int h;
	int level;
	unsigned char hidden;
	void *data;
	void *ptr;
};

struct _Equanime_Region
{
	
};

struct _Equanime_Controller
{
	Eina_Inlist list;
	Equanime_Layer **layers;
	int num_layers;
	const Equanime_Controller_Description *desc;
	const Equanime_Controller_Functions *fncs;
	void *data;
};

#endif /*EQUANIME_PRIVATE_H_*/
