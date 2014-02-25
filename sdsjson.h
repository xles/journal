/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * sdsjson.c
 *    JSON creation using SDS.
 */
#include <stdarg.h>
#include "sds.h"

extern sds json_new(sds json, ...);
extern sds json_add_object(sds str, ...);
extern sds json_add_array(char *key, sds *values);
extern sds json_add_string(char *key, sds value);
extern sds json_add_int(char *key, int value);
extern sds json_add_float(char *key, float value);
