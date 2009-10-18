#ifndef _EQU_TIMING
#define _EQU_TIMING

typedef struct _Equ_Timing Equ_Timing;

typedef enum _Equ_Standard
{
	EQU_STANDARD_CUSTOM,
} Equ_Standard;

struct _Equ_Timing
{
	Equ_Standard std;
	/* TODO the timings */
};

EAPI void equ_timing_standard_from(Equ_Timing *t, Equ_Standard std);

#endif
