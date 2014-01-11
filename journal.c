/**
 * Journal
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include "colour.h"
#include "journal.h"
#include "mongoose.h"

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("Basic usage: journal [command]\n");
		exit(1);
	} else {
		switch ((int) argv[1][0]) {
			case '-':
				parse_options(argv[1]);
				break;
			default:
				printf("Sorry, no such option.\n");
				break;
		}
	}

}

/*
				serve();
		char * editor;
		editor = getenv("EDITOR");
		printf("Your default editor is: '%s%s%s'\n", 
			ANSI_COLOR_MAGENTA, 
			editor, 
			ANSI_COLOR_RESET);
*/

void parse_options(char *options)
{
	printf("%s\n", options);
}

void serve(void)
{
	printf("Launching server on port 4000, press ^C to exit.\n");
	struct mg_server *server = mg_create_server(NULL);
	mg_set_option(server, "document_root", ".");
	mg_set_option(server, "listening_port", "4000");
	for (;;) 
		mg_poll_server(server, 1000);  // Infinite loop, Ctrl-C to stop
	mg_destroy_server(&server);
	printf("Server halted\n");
}
