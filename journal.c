/**
 * Journal


 * Credits:
 *    Webserver: Mongoose, http://cesanta.com/#docs,Mongoose.md
 *    Regex: SLRE, http://cesanta.com/#docs,SLRE.md
 *    JSON: cJSON, http://cjson.sourceforge.net/
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "cJSON.h"
#include "colour.h"
#include "journal.h"
#include "mongoose.h"

#define SERVE_ROOT "."
#define SERVE_PORT "4000"
#define OPTIONS "asdf"


int main(int argc, char **argv)
{
	if (argc == 1) {
		printf("Basic usage: journal <options> [command]\n");
		exit(1);
	}

	int c = getopt(argc, argv, OPTIONS);
	while (1) {
		if (c == -1)
			break;

		switch (c) {
			case 'h':
				printf("halp\n");
				break;
			case '?':
				printf("dafuq?\n");
				break;
		}
	}

	if (!strcmp(argv[optind],"serve")) {
		serve();
	} else if (!strcmp(argv[optind],"new")) {
		new(argv);
	} else if (!strcmp(argv[optind],"editor")) {
		char * editor;
		editor = getenv("EDITOR");
		printf("Your default editor is: '%s%s%s'\n", 
			ANSI_COLOR_MAGENTA, 
			editor, 
			ANSI_COLOR_RESET);		
	} else {
		printf("Invalid operand\n");
	}
}

void parse_options(char *options)
{
	printf("options: %s\n", options);
}

void new(char **argv)	
{
	char *wd;
	if(argv[optind+1] == NULL)
		wd = getcwd(NULL,64);
	else {
		if (!strncmp(&argv[optind+1][0],".",1)) {
			wd = argv[optind+1];
		} else {
			sprintf(wd,"./%s", argv[optind+1]);
		}
	}

	printf("Creating new journal in %s\n", wd);
}

void serve(void)
{
	printf("Launching server on port 4000, press ^C to exit.\n");

	struct mg_server *server = mg_create_server(NULL);
	mg_set_option(server, "document_root", SERVE_ROOT);
	mg_set_option(server, "listening_port", SERVE_PORT);
	
	while (1) {
		mg_poll_server(server, 1000);
	} 
	
	mg_destroy_server(&server);
}
