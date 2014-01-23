/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * mustache.c
 *    Main source file for the journal project.
 */

struct tags {
	struct tags *next;
	const char *key;
	const char *value;
};


extern int render_template(void);
extern char *read_file(char *file);
