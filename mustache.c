/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * mustache.c
 *    Main source file for the journal project.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mustache.h"
#include "journal.h"
#include "slre.h"
#include "sds.h"

static char *tpldir = "templates/mustache/";

char *rdelim = "}}";
char *ldelim = "{{"; 

static char *str_replace(char *str, char *search, char *replace);
static char *parse_tags(char *str);

static char *tag_variable(char *str, char *search) 
{
//	char *re = "";
	return str;
}
static char *tag_section(char *str, char *search) 
{
//	char *re = "";
	return str;
}
static char *tag_comment(char *str, char *search) 
{
	char *buff = malloc(1024);
	buff = str_replace(str,search,NULL);
	//strcpy(str, buff);
	return buff;
}
static char *tag_partial(char *str, char *search) 
{
	char *re = calloc(1, 1024);
	char *file = calloc(1, 256);
	char *buff = malloc(1024);

	sprintf(re, "%s>([^}]+)%s", ldelim, rdelim);
	struct slre_cap caps[1];

	if (slre_match(re, search, strlen(search), caps, 1) > 0) {
		sprintf(file,"%s%.*s", tpldir, caps[0].len, caps[0].ptr);
		buff = read_file(file);
		buff = parse_tags(buff);
		buff = str_replace(str, search, buff);
		free(file);
	} 
	strcpy(str, buff);
	return buff;
}
static char *tag_delimiter(char *str, char *search) 
{
	char *re = malloc(1024), *buff = malloc(1024);
	sprintf(re, "%s=(\\S+)\\s+(\\S+)=%s", ldelim, rdelim);
	struct slre_cap caps[2];

	if (slre_match(re, search, strlen(search), caps, 2) > 0) {
	printf("uhm...\n");
		strncpy(ldelim, caps[0].ptr, caps[0].len);
		strncpy(rdelim, caps[1].ptr, caps[1].len);
		buff = str_replace(str, search, NULL);
	}
	//strcpy(str,buff);
	strcpy(str,buff);
	return str;
}

static char *parse_tags(char *str)
{

	char *re = malloc(1024), *search = malloc(1024);
	sprintf(re, "(%s[\\S\\s]+?%s)", ldelim, rdelim);
	struct slre_cap caps[1];
	int i, j = 0, str_len = strlen(str);

	while (j < str_len && 
		(i = slre_match(re, str+j, str_len-j, caps, 1)) > 0) {
		sprintf(search,"%.*s", caps[0].len, caps[0].ptr);
		
		switch (search[(strlen(ldelim))]) {
			case '!':
				printf("tag found: comment: \"%s\"\n",search);
				str = tag_comment(str, search);
				break;
			case '>':
				printf("tag found: partial: \"%s\"\n",search);
				str = tag_partial(str, search);
				break;
			case '=':
				printf("tag found: set delimiter: \"%s\"\n",search);
				str = tag_delimiter(str, search);
				break;
			case '#':
			case '^':
			case '/':
				printf("tag found: section: \"%s\"\n",search);
				str = tag_section(str, search);
				break;
			default:
				printf("tag found: variable: \"%s\"\n",search);
				str = tag_variable(str, search);
				break;
		}
		j += i;
	}
	return str;
}
int render_template(void)
{
	sds foo = sdsnew(read_file("templates/mustache/test"));
//	char *foo = read_file("templates/mustache/delimiter");
	foo = parse_tags(foo);
	printf("\"%s\"\n", foo);
	sdsfree(foo);
	return 0;
}
/*
 * SLRE supported syntax reference:
 *
 *   (?i)    Must be at the beginning of the regex. 
 *              Makes match case-insensitive
 *   ^       Match beginning of a buffer
 *   $       Match end of a buffer
 *   ()      Grouping and substring capturing
 *   \s      Match whitespace
 *   \S      Match non-whitespace
 *   \d      Match decimal digit
 *   +       Match one or more times (greedy)
 *   +?      Match one or more times (non-greedy)
 *   *       Match zero or more times (greedy)
 *   *?      Match zero or more times (non-greedy)
 *   ?       Match zero or once (non-greedy)
 *   x|y     Match x or y (alternation operator)
 *   \meta   Match one of the meta character: ^$().[]*+?|\
 *   \xHH    Match byte with hex value 0xHH, e.g. \x4a
 *   [...]   Match any character from set. 
 *              Ranges like [a-z] are supported
 *   [^...]  Match any character but ones from set
 */
char *read_file(char *file)
{
	puts(file);
	FILE    *inf;
	char    *buff;
	long    flen, mem = 1024;
	 
	inf = fopen(file, "r");
	 
	if (inf == NULL)
	    return NULL;
	
	 
	fseek(inf, 0L, SEEK_END);
	flen = ftell(inf);
	 
	fseek(inf, 0L, SEEK_SET);	
	
	while (mem < flen)
		mem *= 2;

	buff = (char*)calloc(mem, sizeof(char));	
	 
	if (buff == NULL)
	    return NULL;
	 
	fread(buff, sizeof(char), flen, inf);
	fclose(inf);
	 
	return buff;
}
// You must free the result if result is non-NULL.
static char *str_replace(char *str, char *search, char *replace) 
{
	char *result, *ins, *tmp;
	int ls, lr, len_front, count;

	if (!str)
		return NULL;
	if (!search)
		search = "";
	ls = strlen(search);
	if (!replace)
		replace = "";
	lr = strlen(replace);

	ins = str;
	for (count = 0; (tmp = strstr(ins, search)); ++count) {
		ins = tmp + ls;
	}

	tmp = result = malloc(strlen(str) + (lr - ls) * count + 1);

	if (!result)
		return NULL;

	while (count--) {
		ins = strstr(str, search);
		len_front = ins - str;
		tmp = strncpy(tmp, str, len_front) + len_front;
		tmp = strcpy(tmp, replace) + lr;
		str += len_front + ls;
	}
	strcpy(tmp, str);
	return result;
}

static char *sdsreplace(char *str, char *search, char *replace) 
{
	char *result, *ins, *tmp;
	int ls, lr, len_front, count;

	if (!str)
		return NULL;
	if (!search)
		search = "";
	ls = strlen(search);
	if (!replace)
		replace = "";
	lr = strlen(replace);

	ins = str;
	for (count = 0; (tmp = strstr(ins, search)); ++count) {
		ins = tmp + ls;
	}

	tmp = result = malloc(strlen(str) + (lr - ls) * count + 1);

	if (!result)
		return NULL;

	while (count--) {
		ins = strstr(str, search);
		len_front = ins - str;
		tmp = strncpy(tmp, str, len_front) + len_front;
		tmp = strcpy(tmp, replace) + lr;
		str += len_front + ls;
	}
	strcpy(tmp, str);
	return result;
}
