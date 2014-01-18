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
#include <string.h>
#include <errno.h>

#include "validate.h"
#include "sha.h"
#include "frozen.h"

#include "hoedown/markdown.h"
#include "hoedown/buffer.h"
#include "hoedown/html.h"
#include "hoedown/autolink.h"

int cmd_build(int arc, char **argv)
{
	hoedown_buffer *ib, *ob;
	FILE *in = stdin;

	hoedown_renderer *renderer;
	hoedown_markdown *markdown;

	in = fopen(argv[2], "r");
	if (!in) {
		if (!in) {
			fprintf(stderr, "Unable to open input file \"%s\": %s\n", argv[2], strerror(errno));
			return 1;
		}
	}


	/* reading everything */
	ib = hoedown_buffer_new(1024);
	while (!feof(in) && !ferror(in)) {
		hoedown_buffer_grow(ib, ib->size + 1024);
		ib->size += fread(ib->data + ib->size, 1, 1024, in);
	}

	if (in != stdin)
		fclose(in);

	/* performing SmartyPants parsing */
	ob = hoedown_buffer_new(64);

	
	renderer = hoedown_html_renderer_new(0, 0);
	markdown = hoedown_markdown_new(0, 16, renderer);

	hoedown_markdown_render(ob, ib->data, ib->size, markdown);

	hoedown_markdown_free(markdown);
	hoedown_html_renderer_free(renderer);


	hoedown_html_smartypants(ob, ob->data, ob->size);

	/* writing the result to stdout */
	(void)fwrite(ob->data, 1, ob->size, stdout);

	/* cleanup */
	hoedown_buffer_free(ib);
	hoedown_buffer_free(ob);

	return ferror(stdout);
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
