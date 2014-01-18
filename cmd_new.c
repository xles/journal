/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * cmd_new.c
 *    Main source file for the journal project.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "journal.h"
#include "validate.h"

int cmd_new(int arc, char **argv)
{

	struct journal_post post;
	char buff[512] = "";
/*
	char fnow[64];

	printf("Creating a new journal entry\n");
	
	while (empty(buff)) {
		printf("Title: ");
		fgets(buff, sizeof(buff), stdin);
		trim(buff);
	}

	strcpy(post.title, buff);
	

	time_t t = time(NULL);
	struct tm *now = localtime(&t);

	strftime(fnow, sizeof(fnow), "%Y-%m-%d", now);

	while (val_date(buff)) {
		printf("Publish date [%s]: ",fnow);
		fgets(buff, sizeof(buff), stdin);
		trim(buff);
		if (empty(buff)) {
			sprintf(buff, "%s", fnow);
		}
	}
	strcpy(post.date, buff);
*/


	printf("Category [Uncategorized]: ");
	fgets(buff, sizeof(buff), stdin);
	trim(buff);
	if (empty(buff)) {
		sprintf(buff, "%s", "Uncategorized");
	}
	strcpy(post.category, buff);
	printf("post.category: \"%s\"\n", post.category);


/*
	printf("Tags (separate by commas): ");
	fgets(buff, sizeof(buff), stdin);

	char *tmp = buff, *tok = NULL, *foo[128];
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
		printf("%d : \"%s\"\n",i, foo[i]);
	}
	strcpy(post.tags, buff);
*/

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
	char syscall[128], *filename = "test/syntax.md";
	if (getenv("EDITOR") == NULL && getenv("VISUAL") == NULL) {
		puts("Unable to detect default editor.\n"
			"Are EDITOR or VISUAL set?");
		printf("Journal entry saved as ./journal/posts/%s\n", 
			filename);
	} else {
		char *editor;
		if (getenv("EDITOR") != NULL)
			editor = getenv("EDITOR");
		else
			editor = getenv("VISUAL");

		printf("Opening journal entry with default editor...");
		fflush(stdout);
		sprintf(syscall, "%s %s", getenv("EDITOR"), filename);
		system(syscall);
		puts("done.");
		printf("Journal entry saved as ./journal/posts/%s\n", 
			filename);
	}

	return 0;
}
