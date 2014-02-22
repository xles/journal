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
#include "frozen.h"
#include "slre.h"
#include "sds.h"

sds tpldir, ldelim, rdelim; 


static sds tag_variable(sds tag) 
{
//	char *re = "";
	return tag;
}
static sds tag_section(sds tag) 
{
//	char *re = "";
	return tag;
}
static sds tag_comment(sds tag) 
{
	return sdsempty();
}
static sds tag_partial(sds tag) 
{
	sds re = sdsempty();
	sds file = sdsempty();
	sds buff = sdsempty();

	re = sdscatprintf(re, "%s>([^}]+)%s", ldelim, rdelim);
	struct slre_cap caps[1];

	if (slre_match(re, tag, strlen(tag), caps, 1) > 0) {
		file = sdscatprintf(file,"%s%.*s", tpldir, caps[0].len, caps[0].ptr);

		FILE * f;
		if ((f = fopen(file, "r")) != NULL) {
			fclose(f);
			buff = render_template(file);
		} else {
			buff = sdscpy(buff,tag);
		}
		
	} 
	sdsfree(file);
//	sdsfree(buff);
	sdsfree(re);
	return buff;
}
static sds tag_delimiter(sds tag) 
{
/*
	char *re = malloc(1024), *buff = malloc(1024);
	sprintf(re, "%s=(\\S+)\\s+(\\S+)=%s", ldelim, rdelim);
	struct slre_cap caps[2];

	if (slre_match(re, tag, strlen(tag), caps, 2) > 0) {
	printf("uhm...\n");
		strncpy(ldelim, caps[0].ptr, caps[0].len);
		strncpy(rdelim, caps[1].ptr, caps[1].len);
		buff = sdsreplace(str, tag, NULL);
	}
	//strcpy(str,buff);
	str = sdscpy(str,buff);
*/	return tag;
}

static struct slre_cap* slre_match_all(const char *regexp, const char *buf, int buf_len,
               struct slre_cap *caps, int num_caps) {
	int count = 0, i = 0, j = 0;
	
	printf("\"\033[32m%s\033[m\"\n", regexp);
	printf("\"\033[31m%s\033[m\"\n", buf);

	struct slre_cap* buffer;     // this will be the array
	buffer = (struct slre_cap*)malloc(1024*sizeof(struct slre_cap));    // allocate storage for the array

	while (j < buf_len && 
		(i = slre_match(regexp, buf+j, buf_len-j, caps, num_caps)) > 0) {
		
		printf("\"\033[36m%.*s\033[m\"\n", caps[0].len, caps[0].ptr);

		buffer[count]=caps[0];

		count++;
		j += i;
	}

	struct slre_cap *res = (struct slre_cap*)malloc(count*sizeof(struct slre_cap));
	
	for (i=0; i<count; i++) {
		res[i] = buffer[i];
	}

	free((void*) buffer);

	return res;
}
static int slre_match_count(const char *regexp, const char *buf, int buf_len,
               struct slre_cap *caps, int num_caps) {
	int count = 0, i = 0, j = 0;
	
	while (j < buf_len && 
		(i = slre_match(regexp, buf+j, buf_len-j, caps, num_caps)) > 0) {
		count++;
		j += i;
	}
	return count;
}


static sds parse_tag(sds tag)
{
	sds buff = sdsempty(); 
	switch (tag[(strlen(ldelim))]) {
		case '!':
			printf("tag found: comment: \"%s\"\n",tag);
			buff = tag_comment(tag);
			break;
		case '>':
			printf("tag found: partial: \"%s\"\n",tag);
			buff = tag_partial(tag);
			break;
		case '=':
			printf("tag found: set delimiter: \"%s\"\n",tag);
			buff = tag_delimiter(tag);
			break;
		case '#':
		case '^':
		case '/':
			printf("tag found: section: \"%s\"\n",tag);
			buff = tag_section(tag);
			break;
		default:
			printf("tag found: variable: \"%s\"\n",tag);
			buff = tag_variable(tag);
			break;
	}

	tag = sdscpylen(tag,buff,sdslen(buff)); 
	sdsfree(buff); 
	return tag;
}
static sds match_tags(sds str)
{

//	char *re = malloc(1024), *search = malloc(1024);
//	sprintf(re, "(%s[\\S\\s]+?%s)", ldelim, rdelim);
	
	sds re = sdsempty(), search, buff = sdsempty(); 
	re = sdscatprintf(re, "(%s[\\S\\s]+?%s)", ldelim, rdelim);

	struct slre_cap caps[1];
	int i, j = 0, str_len = sdslen(str), count = 0;

	struct slre_cap* match = slre_match_all(re, str, str_len, caps, 1);
	int matches = slre_match_count(re, str, str_len, caps, 1);
	
	printf("matches: %d\n", matches);

	sds remain = sdsdup(str);
	sds part = sdsempty();
	char *pos;
	for (i=0; i < matches; i++) {
		search = sdsempty(); 
		search = sdscatprintf(search,"%.*s", match[i].len, match[i].ptr);
		
		pos = strstr(remain, search);

		part = sdscpylen(part, remain, pos-remain); 

		remain = sdscpy(remain, pos+sdslen(search)); 
	
		printf("part: '\033[32m%s\033[m'\n",part);
		buff = sdscat(buff,part);
		
		printf("Mathed tag: '\033[35m%s\033[m'\n", search);
		buff = sdscat(buff,parse_tag(search));
		
		printf("remain: '\033[31m%s\033[m'\n",remain);

	}
	buff = sdscat(buff,remain);
	str = sdscpylen(str,buff,sdslen(buff)); 
	
	sdsfree(part);
	sdsfree(remain);
	sdsfree(search);
	sdsfree(buff);
	sdsfree(re);
	return str;
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
static sds sdsreplace(sds str, sds search, char *replace) 
{
	sds buff = sdsempty();
	char *p;

	if (!(p = strstr(str, search)))
		return str;

	buff = sdscatlen(buff, str, p-str);
	if (replace != NULL) {
		buff = sdscat(buff, replace);
	}
	buff = sdscat(buff, p+sdslen(search));
	//sdsfree(search);

	str = sdscpylen(str,buff,sdslen(buff)); 
	sdsfree(buff);
	return str;
}

int mustache_init(char *td, char *rd, char *ld)
{
	rdelim = rd ? sdsnew(rd) : sdsnew("}}");
	ldelim = ld ? sdsnew(ld) : sdsnew("{{"); 
	tpldir = td ? sdsnew(td) : sdsnew("templates/mustache/");

	return 0;
}

/**

 */
sds render_template(char *tpl)
{

	sds buff = sdsnew(read_file(tpl));
	buff = match_tags(buff);
	return buff;
}
