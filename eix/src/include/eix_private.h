#ifndef _EIX_PRIVATE_H
#define _EIX_PRIVATE_H

#include <string.h>
#include "Ecore.h"
#include "Ecore_Con.h"

extern int eix_log;

#define ERR(...) EINA_LOG_DOM_ERR(eix_log, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(eix_log, __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(eix_log, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG(eix_log, __VA_ARGS__)

#endif
