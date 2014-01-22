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

#include "journal.h"
#include "slre.h"

static char *rdelim = "}}";
static char *ldelim = "{{";


static char *tag_variable(char *str) 
{
//	char *re = "";
	return str;
}
static char *tag_section(char *str) 
{
//	char *re = "";
	return str;
}
static char *tag_comment(char *str) 
{
	char *re = malloc(0), *search;
	sprintf(re, "(%s![\\S\\s]+%s)", ldelim, rdelim);
	struct slre_cap caps[2];
	int i, j = 0, str_len = strlen(str);

	while (j < str_len && 
		(i = slre_match(re, str+j, str_len-j, caps, 2)) > 0) {
		sprintf(search,"%.*s", caps[0].len, caps[0].ptr)
		str = str_replace(str, search,"fpp");
		j += i;
	}
	return str;
}
static char *tag_partial(char *str) 
{
//	char *re = "";
	return str;
}
static char *tag_delimiter(char *str) 
{
	char *re = malloc(NULL);
	sprintf(re, "%s=(\\S+)\\s+(\\S+)=%s", ldelim, rdelim);
	struct slre_cap caps[2];

	if (slre_match(re, buff, strlen(buff), caps, 2) > 0) {
		ldelim = strncpy(buff, caps[0].ptr, caps[0].len);
		rdelim = strncpy(buff, caps[1].ptr, caps[1].len);
	}
	return 0;
}

static char *parse_tags(char *str)
{
	str = tag_comment(str);
	str = tag_partial(str);
	str = tag_section(str);
	str = tag_variable(str);
	str = tag_delimiter(str);
	return str;
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
char *replace_str(char *str, char *orig, char *rep)
{
	static char buffer[4096];
	char *p;

	if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
		return str;

	strncpy(buffer, str, p-str); // Copy characters from 'str' start to 'orig' st$
	buffer[p-str] = '\0';

	sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

	return buffer;
}
int find_urls(void) {
	static const char *str =
		"<img src=\"HTTPS://FOO.COM/x?b#c=tab1\"/> "
		"  <a href=\"http://cesanta.com\">some link</a>";

	static const char *regex = "(?i)((https?://)[^\\s/'\"<>]+/?[^\\s'\"<>]*)";
	struct slre_cap caps[2];
	int i, j = 0, str_len = strlen(str);

	while (j < str_len && (i = slre_match(regex, str + j, str_len - j, caps, 2, NULL)) > 0) {
		printf("Found URL: [%.*s]\n", caps[0].len, caps[0].ptr);
		j += i;
	}
}

static char *reg_replace(char *str, char *regex, struct tags replace, 
	struct slre_cap caps, int numcaps)
{
	if ((slre_match(regex, str, strlen(str), caps, numcaps) > 0) {
		str = str_replace(str, sprintf("%.*s", caps[0].len, caps[0].ptr),"fpp");
	}
}

int render_template(void)
{
	parse_tags("Everything's foo today");
	char *str = "Everything's foo today";
	char *search = "foo";
	char *replace = "bar";
	char *out;
	str = str_replace(str,search,replace); 
	puts(str);
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
