/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * mustache.c
 *    Main source file for the journal project.
 */
#include "sds.h"

struct tags {
	struct tags *next;
	const char *key;
	const char *value;
};


extern int mustache_init(char *td, char *rd, char *ld);
extern sds render_template(char *tpl);
extern char *read_file(char *file);
