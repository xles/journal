/**
 * Journal
 *
 *
 * Credits:
 *    Webserver: Mongoose, http://cesanta.com/#docs,Mongoose.md
 *    Regex: SLRE, http://cesanta.com/#docs,SLRE.md
 *    JSON: cJSON, http://cjson.sourceforge.net/
 *    Markdown: Sundown, https://github.com/vmg/sundown
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "colour.h"
#include "slre.h"
#include "cJSON.h"
#include "journal.h"
#include "mongoose.h"
#include "sundown/markdown.h"
#include "sundown/buffer.h"
#include "sundown/html.h"

#define SERVE_ROOT "."
#define SERVE_PORT "4000"
#define OPTIONS "asdf"


int main(int argc, char **argv)
{
	if (argc == 1) {
		usage();
		return 1;
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
	} else if (!strcmp(argv[optind],"test")) {
		test_markdown(argv[optind+1]);
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

int test_markdown(char *file)
{
	struct buf *ib, *ob;
	int ret;
	FILE *in = stdin;

	struct sd_callbacks callbacks;
	struct html_renderopt options;
	struct sd_markdown *markdown;

	in = fopen(file, "r");
	if (!in) {
		if(file == NULL) {
			fprintf(stderr, "No file specified\n");
		} else {
			fprintf(stderr, "Unable to open input file: "
					"\"%s\"\n", file);
		}
		return 1;
	}

	/* reading everything */
	ib = bufnew(1024);
	bufgrow(ib, 1024);
	while ((ret = fread(ib->data + ib->size, 1, ib->asize - ib->size, in)) > 0) {
		ib->size += ret;
		bufgrow(ib, ib->size + 1024);
	}

	if (in != stdin)
		fclose(in);

	/* performing markdown parsing */
	ob = bufnew(64);

	sdhtml_renderer(&callbacks, &options, 0);
	markdown = sd_markdown_new(0, 16, &callbacks, &options);

	sd_markdown_render(ob, ib->data, ib->size, markdown);
	sd_markdown_free(markdown);

	/* writing the result to stdout */
	ret = fwrite(ob->data, 1, ob->size, stdout);

	/* cleanup */
	bufrelease(ib);
	bufrelease(ob);

	return (ret < 0) ? -1 : 0;
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

void usage(void)
{
	printf("Basic usage: journal <options> [command]\n");
	printf( "foo "
		"bar "
		"baz\n"
	);
}
