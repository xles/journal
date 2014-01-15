/**
 * Journal
 *
 *
 * Credits for embeded components:
 *    Webserver: Mongoose, http://cesanta.com/#docs,Mongoose.md
 *    Regex: SLRE, http://cesanta.com/#docs,SLRE.md
 *    JSON: cJSON, http://cjson.sourceforge.net/
 *    Markdown: Sundown, https://github.com/vmg/sundown
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <stdbool.h>

#include "journal.h"
#include "colour.h"
#include "validate.h"
 
#include "slre.h"
#include "frozen.h"
#include "sundown/markdown.h"
#include "sundown/buffer.h"
#include "sundown/html.h"

#ifdef _WIN32
 #define NIX 0
 #define getcwd(a,b) _getcwd(a,b)
#else
 #include "mongoose.h"
 #define NIX 1
#endif

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
		if(NIX)
			serve();
		else 
			puts("Webserver not supported on windows");
	} else if (!strcmp(argv[optind],"test")) {
//		test_markdown(argv[optind+1]);
//		system("pwd");
		new_post();
	} else if (!strcmp(argv[optind],"init")) {
		init(argv);
	} else if (!strcmp(argv[optind],"editor")) {
		char * editor;
		editor = getenv("EDITOR");
		printf("Your default editor is: '%s%s%s'\n", 
			ANSI_COLOR_MAGENTA, 
			editor, 
			ANSI_COLOR_RESET);		
	} else {
		printf("Invalid operand\n");
		usage();
	}
}


int new_post(void)
{
	char title[128] = "";
	char category[64] = "";
	char date[64] = "";
	char fnow[64];
	char tags[128];
/*
	printf("Creating a new journal entry\n");
	
	while (empty(title)) {
		printf("Title: ");
		fgets(title, sizeof(title), stdin);
		trim(title);
	}
	
	time_t t = time(NULL);
	struct tm *now = localtime(&t);

	strftime(fnow, sizeof(fnow), "%Y-%m-%d", now);

	while (val_date(date)) {
		printf("Publish date [%s]: ",fnow);
		fgets(date, sizeof(date), stdin);
		trim(date);
		if (empty(date)) {
			sprintf(date, "%s", fnow);
		}
	}

	printf("Category [Uncategorized]: ");
	fgets(category, sizeof(category), stdin);
	trim(category);
	if (empty(category)) {
		sprintf(category, "%s", "Uncategorized");
	}

//	printf("category: \"%s\"\n", category);
*/
	printf("Tags (separate by commas): ");
	fgets(tags, sizeof(tags), stdin);

	char *tmp = tags, *tok = NULL, *foo[128];
	int i = 0, noftags = 0;
	
	tok = strtok(&tmp[0], ",");
	while (tok) {
		trim(tok);
		foo[i] = tok;
		tok = strtok(NULL, ",");
		i++;
	}
	noftags = i;
	printf("foo is: %lu\n",sizeof(foo));
	for (i = 0; i < noftags; i++) {
		printf("%d : %s\n",i, foo[i]);
	}

/*
	$atom_id = 'tag:'.$this->config['syndication']['url'];
	$atom_id += ','.$date.':'.md5($title);

	$json = ['title'    => $title,
		 'category' => $category,
		 'tags'     => $tags,
		 'pubdate'  => $date,
		 'slug'     => $this->slug($title), 
		 'layout'   => 'post',
		 'atom_id'  => $atom_id];

	$md  = "Post goes here\n\n";
	$md .= "---EOF---\n";
	$md .= json_encode($json, JSON_PRETTY_PRINT);
		
	$filename = 'posts/'.$date.'_'.$json['slug'].'.md';
	file_put_contents($filename, $md);
*/
	printf("Opening journal entry with default editor...");
	char syscall[128], *filename = "test/syntax.md";

	sprintf(syscall, "$EDITOR %s", filename);
	system(syscall);
	puts("done.");

	printf("Journal entry saved as ./journal/posts/%s\n", filename);
	return 0;
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


void init(char **argv)	
{
	char *cwd;
	cwd = getcwd(NULL,64);
	if (argv[optind+1] != NULL) {
		if (!strncmp(&argv[optind+1][0],"/",1)) {
			cwd = argv[optind+1];
		} else {
			if (!strncmp(&argv[optind+1][1],"/",1)) {
				sprintf(cwd,"%s/%s",cwd,argv[optind+1]+2);
			} else {
				sprintf(cwd,"%s/%s",cwd,argv[optind+1]);
			}
		}
	}

	DIR *dir = opendir(".journal");
	if (dir) {
		printf("A .journal exists in directory '%s'\n", cwd);
		return;
	}
	printf("Initializing new journal in '%s'\n", cwd);
}

void serve(void)
{
	if(!NIX)
		return;
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
	printf(
	"usage: journal [-BPSXeiknqrstv] [-C directory] [-D variable]\n"
	"\t[-d flags] [-E variable] [-f makefile] [-I directory]\n"
	"\t[-j max_jobs] [-m directory] [-V variable]\n"
	"\t[variable=value] [target ...]\n"
	);
//	printf("Basic usage: journal <options> [command]\n");
}
