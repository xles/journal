/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * mustache.c
 *    Main source file for the journal project.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sds.h"
#include "sdsjson.h"

static int level;

static sds json_indent(sds str)
{
	puts("json_indent");
	int i;
	for(i=0; i < (level); i++)
		str = sdscat(str,"\t");
	return str;
}

static sds escape_string(sds str)
{
	puts("escape_string");
	return str;
}

sds json_new(sds json, ...)
{
	puts("json_new");
	level = 1;
	va_list ap;
	sds arg = sdsempty(), tmp = sdsnew("{\n");

	tmp = sdscat(tmp,json);
	
	va_start(ap, json); 
	while ((arg = va_arg(ap, char *)) != NULL) {
		tmp = sdscat(tmp,",\n");
		tmp = sdscat(tmp,arg);
		arg = sdsempty();
	}
	tmp = sdscat(tmp,"\n}");
	va_end(ap);
	sdsfree(arg);

	return tmp;
}

sds json_add_object(sds key, ...)
{
	puts("json_add_object");
	int count=0;
	va_list ap;
	sds arg = sdsempty(), tmp = sdsempty();

	level++;

	if (key != NULL)
		tmp = sdscatprintf(tmp,"\"%s\" : ",key);
	tmp = sdscat(tmp,"{\n");
	
	va_start(ap, key); 
	while ((arg = va_arg(ap, char *)) != NULL) {
		if (count++ > 0)
			tmp = sdscat(tmp,",\n");
		level++;
//		tmp = json_indent(tmp);
		tmp = sdscat(tmp,arg);
		arg = sdsempty();
		level--;
		//level--;
	}
	tmp = sdscat(tmp,"\n");
	va_end(ap);
	sdsfree(arg);
	level--;
//	tmp = json_indent(tmp);
	tmp = sdscat(tmp,"}");

	return tmp;
}

sds json_add_array(char *key, sds *values)
{
	puts("json_add_array");
	sds tmp = sdsempty();
	int count=0;

	if (key != NULL)
		tmp = sdscatprintf(tmp,"\"%s\" : ",key);

	tmp = sdscat(tmp,"[\n");
	while (values[count] != NULL) {
		if (count > 0)
			tmp = sdscat(tmp,",\n");
		tmp = sdscatprintf(tmp,"%s",values[count]);
		count++;
	}
	tmp = sdscat(tmp,"\n]");

	return tmp;
}
sds json_add_string(char *key, sds value)
{
	puts("json_add_string");
	sds tmp = sdsempty();
	if (key != NULL)
		tmp = sdscatprintf(tmp,"\"%s\" : ",key);
	value = escape_string(value);
	tmp = sdscatprintf(tmp,"\"%s\"",value);
	return tmp;
}
sds json_add_int(char *key, int value)
{
	puts("json_add_int");
	sds tmp = sdsempty();
	if (key != NULL)
		tmp = sdscatprintf(tmp,"\"%s\" : ",key);
	tmp = sdscatprintf(tmp,"%d",value);
	return tmp;
}
sds json_add_float(char *key, float value)
{
	puts("json_add_float");
	sds tmp = sdsempty();
	if (key != NULL)
		tmp = sdscatprintf(tmp,"\"%s\" : ",key);
	tmp = sdscatprintf(tmp,"%f",value);
	return tmp;
}
