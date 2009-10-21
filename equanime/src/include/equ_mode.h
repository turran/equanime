#ifndef _EQU_MODE
#define _EQU_MODE

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

EAPI void equ_mode_standard_from(Equ_Mode *m, Equ_Standard std);

#endif
