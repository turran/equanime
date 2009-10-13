#ifndef _EQU_REGION_H
#define _EQU_REGION_H

typedef struct _Equ_Region Equ_Region; /**< Opaque handler */
typedef struct _Equ_Region_Description Equ_Region_Description; /**< Opaque handler */

/**
 *
 */
typedef enum
{
	EQU_REGION_VISIBILITY 	= (1 << 0),
	EQU_REGION_POSITION 	= (1 << 1),
	EQU_REGION_SIZE	 	= (1 << 2),
	EQU_REGION_LEVEL		= (1 << 3),
	EQU_REGION_COLORKEY	= (1 << 4),
	EQU_REGION_BLEND		= (1 << 5),
} Equ_Region_Flags;

/**
 *
 */
struct _Equ_Region_Description
{
	const char *cname; /** Controller name */
	const char *name; /** Layer name */
	int flags; /** Layer flags */
};

#endif
