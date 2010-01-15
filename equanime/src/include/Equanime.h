#ifndef _EQUANIME_H
#define _EQUANIME_H

#include <stdlib.h>
#include <stdio.h>

#include "Eina.h"

/**
 * @mainpage Equanime
 * @section intro Introduction
 * Equanime aims to be a simple hardware based media library for embedded
 * devices.
 *
 * Host
 * +-----> Controller
 *         +-----> Layer
 *                 +-----> Region
 * +-----> Component
 *
 * Drivers:
 * - mp25xxf: MagicEyes (www.mesdigital.com) based SoC. Note that you *need*
 * the code.google.com/p/gp2x-linux26 kernel :)
 * - dm320: Texas Instruments DM320 based SoC.
 * - dummy: UIO dummy driver.
 *
 * @file
 * @brief Equanime API
 * @defgroup Equanime_Group API
 * @{
 *
 * @todo
 * - Support for outputs and inputs. The controller will have one output
 * in case it can only display the layer information on one crt/tv/panel/whatever
 * The inputs are useful for layers that can receive data from several sources,
 * like a video ccd block, etc.
 * - Define color-keying
 * - Define alpha blending between layers
 * - Define the concept of regions, layers with no regions in hw means one region
 *   in sw with the size and properties of the layer but can't change anything?
 * - Support for differnt hw blocks: Colorspace converter, Resizer, Rotator, etc
 * Add support for vidoe events, like vsync, signal status, etc
 */

#include "equ_core.h"
#include "equ_enums.h"
#include "equ_surface.h"
#include "equ_host.h"
#include "equ_mode.h"
#include "equ_output.h"
#include "equ_input.h"
#include "equ_controller.h"
#include "equ_layer.h"
#include "equ_region.h"
#include "equ_component.h"

typedef struct _Equ_Rotator Equ_Rotator;
typedef struct _Equ_Scaler Equ_Scaler;
typedef struct _Equ_Csc Equ_Csc;


/**
 * @}
 * @}
 */
#endif /*_EQUANIME_H*/
