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


static int tag_variable() 
{
//	char *re = "";
	return 0;
}
static int tag_section() 
{
//	char *re = "";
	return 0;
}
static int tag_comment() 
{
//	char *re = "";
	return 0;
}
static int tag_partial() 
{
//	char *re = "";
	return 0;
}
static int tag_delimiter(char *buff) 
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

static int parse_tags()
{
	tag_variable();
	tag_section();
	tag_comment();
	tag_partial();
	tag_delimiter("moo");
	return 0;
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
/*
static char *reg_replace(char *str, char *regex, char *replace, 
	struct slre_cap caps, int numcaps)
{
	char *r;
	if ((slre_match(regex, str, strlen(str), ) > 0)
		r = str_replace

}
*/
int render_template(void)
{
	parse_tags();
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
