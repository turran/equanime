#ifndef _EQU_TIMING
#define _EQU_TIMING

typedef struct _Equ_Timing Equ_Timing;

typedef enum _Equ_Standard
{
	EQU_STANDARD_CUSTOM,
	EQU_STANDARD_PAL,
	EQU_STANDARD_NTSC,
	EQU_STANDARD_480P,
	EQU_STANDARD_720P,
	EQU_STANDARD_1080I,
	EQU_STANDARD_1080P,
} Equ_Standard;

struct _Equ_Timing
{
	Equ_Standard std;
	/* TODO the timings */
};

EAPI void equ_timing_standard_from(Equ_Timing *t, Equ_Standard std);

#endif
