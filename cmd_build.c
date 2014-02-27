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

#include "mustache.h"
#include "validate.h"
#include "journal.h"
#include "frozen.h"
#include "sha.h"
#include "sdsjson.h"
#include "colour.h"

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
	
	mustache_init("templates/html/", NULL, NULL, NULL);

	int i, elements=3;
	sds *posts, *links;
	posts = malloc(sizeof(sds)*elements);
	links = malloc(sizeof(sds)*elements);

	posts[0] = json_add_object(NULL,
		json_add_string("anchor" , "fancy-post"),
		json_add_string("url" , "fancy-post"),
		json_add_string("title" , "Fancy Post"),
		json_add_string("datetime" ,"2014-01-15"),
		json_add_string("date" , "2014-01-15"),
		json_add_string("body" , "hoooraaaay, blog post!"),
	NULL);
	posts[1] = NULL;
	
	links[0] = json_add_object(NULL,
		json_add_string("url" , "http:\/\/github.com\/"),
		json_add_string("title" , "Github is pretty neat!"),
	NULL);
	links[1] = NULL;

	sds json = json_new(
		json_add_string("title", "fancy progmatic blog title"),
		json_add_string("description" , "fancy progmatic blog description"),
		json_add_object("meta",
			json_add_string("title", "fancy blog title"),
			json_add_string("description" , "fancy blog description"),
			json_add_string("author" , "xles"),
			json_add_string("generator" , "journal 0.1"),
			json_add_string("keywords" , "fancy,blog,and,stuff"),
		NULL),
		json_add_array("posts" , posts),
		json_add_object("archive",
			json_add_string("url" , "archive"),
		NULL),
		json_add_object("rss",
			json_add_string("url" , "feed"),
		NULL),
		json_add_array("links", links),
		json_add_object("updated",
			json_add_string("datetime", "2014-01-15"),
			json_add_string("date" , "2014-01-15"),
		NULL),
	NULL);

//	for (i = 0; i < elements; i++) {
//		sdsfree(posts[i]);
//		sdsfree(links[i]);
//	}
//	puts("fubar");
//	free(posts);
//	free(links);

	//sds tpl = sdsnew("test");
	if (set_context(json)) {
		sds foo = sdsnew(render_template(sdsnew("layout")));
		printf("\"%s\"\n", foo);
		sdsfree(foo);
	} else {
		puts("error reading context");
	}
//	sds foo = sdsnew(render_template("delimiter"));

//	puts(sdscatprintf(sdsempty(),"%s%s%s",ANSI_COLOUR_MAGENTA,json,ANSI_COLOUR_RESET));
	//puts(json);
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
