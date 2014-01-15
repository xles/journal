/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 *
 * journal.h
 *    Main header file for the journal project.
 */

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

void parse_options(char *options);
void init(char **argv);
void serve(void);
void usage(void);
int test_markdown(char *file);
int new_post(void);
int val_date(char *date);

/*
In file included from journal.c:19:0:
journal.h:5:18: error: expected identifier or '(' before '=' token
 struct mg_server = {};
		  ^
journal.h:7:27: error: expected '{' before '*' token
 void mg_set_option(mg_server baz, char *foo, int bar){}
			   ^
journal.h:9:28: error: expected '{' before '*' token
 void mg_poll_server( *mg_server, int foo){}
			    ^
journal.c: In function 'init':
journal.c:215:2: warning: implicit declaration of function 'getcwd' [-Wimplicit-
function-declaration]
  cwd = getcwd(NULL,64);
  ^
journal.c:215:6: warning: assignment makes pointer from integer without a cast [
enabled by default]
  cwd = getcwd(NULL,64);
      ^
journal.c: In function 'serve':
journal.c:242:9: error: too many arguments to function 'mg_create_server'
  struct mg_server *server = mg_create_server(NULL);
	 ^
In file included from journal.c:19:0:
journal.h:6:6: note: declared here
 void mg_create_server(void){}
      ^
journal.c:242:29: error: void value not ignored as it ought to be
  struct mg_server *server = mg_create_server(NULL);
			     ^
journal.c:243:2: warning: passing argument 1 of 'mg_set_option' from incompatibl
e pointer type [enabled by default]
  mg_set_option(server, "document_root", SERVE_ROOT);
  ^
In file included from journal.c:19:0:
journal.h:7:6: note: expected 'int *' but argument is of type 'struct mg_server
*'
 void mg_set_option(struct *mg_server, char *foo, int bar){}
      ^
journal.c:243:2: warning: passing argument 3 of 'mg_set_option' makes integer fr
om pointer without a cast [enabled by default]
  mg_set_option(server, "document_root", SERVE_ROOT);
  ^
In file included from journal.c:19:0:
journal.h:7:6: note: expected 'int' but argument is of type 'char *'
 void mg_set_option(struct *mg_server, char *foo, int bar){}
      ^
journal.c:244:2: warning: passing argument 1 of 'mg_set_option' from incompatibl
e pointer type [enabled by default]
  mg_set_option(server, "listening_port", SERVE_PORT);
  ^
In file included from journal.c:19:0:
journal.h:7:6: note: expected 'int *' but argument is of type 'struct mg_server
*'
 void mg_set_option(struct *mg_server, char *foo, int bar){}
      ^
journal.c:244:2: warning: passing argument 3 of 'mg_set_option' makes integer fr
om pointer without a cast [enabled by default]
  mg_set_option(server, "listening_port", SERVE_PORT);
  ^
In file included from journal.c:19:0:
journal.h:7:6: note: expected 'int' but argument is of type 'char *'
 void mg_set_option(struct *mg_server, char *foo, int bar){}
      ^
journal.c:247:3: warning: passing argument 1 of 'mg_poll_server' from incompatib
le pointer type [enabled by default]
   mg_poll_server(server, 1000);
   ^
In file included from journal.c:19:0:
journal.h:9:6: note: expected 'int *' but argument is of type 'struct mg_server
*'
 void mg_poll_server(struct *mg_server, int foo){}
      ^
journal.c:250:2: warning: implicit declaration of function 'mg_destroy_server' [
-Wimplicit-function-declaration]
  mg_destroy_server(&server);
  ^
make: *** [compile] Error 1
*/
