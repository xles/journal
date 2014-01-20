/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * cmd_new.c
 *    Main source file for the journal project.
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#include "validate.h"
#include "journal.h"
#include "frozen.h"
#include "sha.h"

#include "hoedown/autolink.h"
#include "hoedown/markdown.h"
#include "hoedown/buffer.h"
#include "hoedown/html5.h"
#include "hoedown/html.h"

#define OPT_SMARTYPANTS 1
#define OPT_HTML5 1

int cmd_build(int argc, char **argv)
{
//	markdown("test/syntax.md", ".journal/posts/syntax.html");
	markdown("test/pants.md", ".journal/posts/pants.html");
	//copy_pages();
	return 0;
}

int markdown(char *inf, char *outf)
{
	FILE *in  = fopen(inf, "r");
	FILE *out = fopen(outf, "w");

	hoedown_buffer *ib, *ob;
	hoedown_renderer *renderer;
	hoedown_markdown *markdown;

	if (!in) {
		fprintf(stderr, "Unable to open input file "
			"\"%s\": %s\n", inf, strerror(errno));
		return 1;
	}

	/* reading everything */
	ib = hoedown_buffer_new(1024);
	while (!feof(in) && !ferror(in)) {
		hoedown_buffer_grow(ib, ib->size + 1024);
		ib->size += fread(ib->data + ib->size, 1, 1024, in);
	}

	fclose(in);

	ob = hoedown_buffer_new(64);
	

	if (OPT_HTML5)
		renderer = hoedown_html5_renderer_new(0, 0);
	else
		renderer = hoedown_html_renderer_new(0, 0);

	markdown = hoedown_markdown_new(0, 16, renderer);
	hoedown_markdown_render(ob, ib->data, ib->size, markdown);

	hoedown_markdown_free(markdown);
	
	if (OPT_HTML5)
		hoedown_html5_renderer_free(renderer);
	else
		hoedown_html_renderer_free(renderer);

	if (OPT_SMARTYPANTS) {
		ib = ob;
		ob = hoedown_buffer_new(64);
		hoedown_html_smartypants(ob, ib->data, ib->size);
	}

	(void)fwrite(ob->data, 1, ob->size, out);

	fclose(out);
	hoedown_buffer_free(ib);
	hoedown_buffer_free(ob);

	return ferror(stdout);
}

int copy_pages(void) 
{ 
	struct stat s;
	if (stat(".journal/pages",&s) == 0) {
		if (s.st_mode & S_IFDIR) {
			puts("it's a directory");
		} else if (s.st_mode & S_IFREG) {
			puts("it's a file");
		} else {
			puts("something else");
		}
	} else {
	    puts("error");
	}
	return 0;
}

int runscript(char *string)
{
	if(empty(string))
		return 1;
	void *buff; 

	fflush(stdout); 

	buff = malloc(40); 

	//SHA1_Data(string,strlen(string),buff);
	SHA1_File(string,buff);

	printf("%s :\n %s\n", string, buff);
//	system(".journal/posts/shit.sh");
	return 0;
}
