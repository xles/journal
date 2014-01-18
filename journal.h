/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 *
 * journal.h
 *    Main header file for the journal project.
 */

#ifndef JOURNAL_H
#define JOURNAL_H

/* Dummy functions to bypass mongoose when building on windows. */
#ifdef _WIN32
struct mg_server;
struct mg_server *mg_create_server(void *foo){}
const char *mg_set_option(struct mg_server *foo, 
	const char *opt, const char *val){}
unsigned int mg_poll_server(struct mg_server *foo, int bar){}
void mg_destroy_server(struct mg_server **foo){}
#endif

struct journal_tags {
	struct journal_tags *next;
	char *name;
};

struct journal_post {
	char title[128];
	char category[128];
	char date[128];
	struct journal_tags tags;
};

/**
 * File embedder, courtesy of Sergey Lyubka.
 */
const char *find_embedded_file(const char *file_name, size_t *size);

extern int cmd_init(int argc, char **argv);
extern int cmd_serve(int arc, char **argv);
extern int cmd_new(int arc, char **argv);
extern int cmd_build(int arc, char **argv);

void parse_options(char *options);
extern int runscript(char *string);
extern int mkpage(void);
extern void usage(void);

#endif
