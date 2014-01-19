/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * journal.c
 *    Main source file for the journal project.
 *
 * Credits for embeded components:
 *    SHA: libmd, http://freebsd.org/
 *    Webserver: Mongoose, http://cesanta.com/#docs,Mongoose.md
 *    Regex: SLRE, http://cesanta.com/#docs,SLRE.md
 *    JSON: cJSON, http://cjson.sourceforge.net/
 *    Markdown: Hoedown, https://github.com/hoedown/hoedown
 * See respective project for further copyright and license information.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "journal.h"
 
#include "slre.h"

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
		cmd_serve(argc, argv);
	} else if (!strcmp(argv[optind],"test")) {
//		test_markdown(argv[optind+1]);
//		system("pwd");
//		mkpage();
		runscript(argv[optind+1]);
	} else if (!strcmp(argv[optind],"build")) {
		cmd_build(argc, argv);
	} else if (!strcmp(argv[optind],"new")) {
		cmd_new(argc, argv);
	} else if (!strcmp(argv[optind],"init")) {
		cmd_init(argc, argv);
	} else if (!strcmp(argv[optind],"editor")) {
		char * editor;
		editor = getenv("EDITOR");
		printf("Your default editor is: '%s'\n", editor);
	} else {
		printf("Invalid operand\n");
		usage();
	}
}


void usage(void)
{
	printf(
	"usage: journal [-BPSXeiknqrstv] [-C directory] [-D variable]\n"
	"\t[-d flags] [-E variable] [-f makefile] [-I directory]\n"
	"\t[-j max_jobs] [-m directory] [-V variable]\n"
	"\t[variable=value] [target ...]\n"
	);
//	printf("Basic usage: journal <options> [command]\n");
}
