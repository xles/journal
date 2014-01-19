/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * cmd_new.c
 *    Main source file for the journal project.
 */

#ifndef _WIN32

#include <stdio.h>
#include "journal.h"
#include "colour.h"

#define _WIN32 0
#include "mongoose.h"

#define SERVE_ROOT "."
#define SERVE_PORT "4000"

int cmd_serve(int arc, char **argv)
{
	if(_WIN32) {
		puts("Webserver not supported on windows");
		return 1;
	}
	printf("Launching server on port 4000, press "
		ANSI_COLOUR_BOLD "^C" ANSI_COLOUR_RESET " to exit.\n");

	struct mg_server *server = mg_create_server(NULL);
	mg_set_option(server, "document_root", SERVE_ROOT);
	mg_set_option(server, "listening_port", SERVE_PORT);
	
	while (1) {
		mg_poll_server(server, 1000);
	} 
	
	mg_destroy_server(&server);
	return 0;
}

#endif
