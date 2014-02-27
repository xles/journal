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
#include <stdbool.h>

#include "mustache.h"
#include "journal.h"
#include "frozen.h"
#include "slre.h"
#include "sds.h"

sds tpldir, ldelim, rdelim, tplext; 
struct json_token ctx[1234];

/**
 * Replaces the string `search` with `replace` in string `sds`
 * 
 * @param sds str  The string to be manipulated
 * @param sds search  The search string
 * @param char *replace  The replacement string
 *
 * @retval sds str  The manipulated string.
 */
static sds sdsreplace(sds str, sds search, char *replace) 
{
	puts("sdsreplace");
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
	puts("free buff");
	//sdsfree(buff);
	return str;
}

/**
 * Takes comment tags and replaces them with an empty string
 * 
 * @param sds tag  The tag 
 *
 * @retval sdsempty()  Empty sds string.
 */
static sds tag_comment(sds tag) 
{
	puts("tag_comment");
	printf("tag found: '%s'\n",tag);
	return sdsempty();
}

/**
 * Takes variable tags and replaces them with their value.
 * 
 * @param sds tag  The tag 
 *
 * @retval sds str  The manipulated string.
 */
static sds tag_variable(sds tag) 
{
	puts("tag_variable");
	printf("tag found: '%s'\n",tag);
//	char *re = "";
	return tag;
}

/**
 * Takes section tags and replaces them with the looped variable.
 * 
 * @param sds tag  The tag 
 *
 * @retval sds str  The manipulated string.
 */
static sds tag_section(sds str, char *tag) 
{
	puts("tag_section");
	printf("tag found: '%s'\n",tag);
//	char *re = "";
	return str;
}

/**
 * Takes inverted sections and replaces them with the looped variable.
 * 
 * @param sds tag  The tag 
 *
 * @retval sds str  The manipulated string.
 */
static sds tag_inverted(sds str, char *tag) 
{
	puts("tag_inverted");
	printf("tag found: '%s'\n",tag);
//	char *re = "";
	return str;
}

/**
 * Takes partial tags and replaces them with a parsed partial template.
 * 
 * @param sds tag  The tag 
 *
 * @retval sds buff  Partial template buffer.
 */
static sds tag_partial(sds tag) 
{
	puts("tag_partial");
	printf("tag found: '%s'\n",tag);
	sds re = sdsempty();
	sds f = sdsempty();
	sds buff = sdsempty();

	re = sdscatprintf(re, "%s>([^}]+)%s", ldelim, rdelim);
	struct slre_cap caps[1];

	if (slre_match(re, tag, strlen(tag), caps, 1) > 0) {
		f = sdscatprintf(f,"%.*s", caps[0].len, caps[0].ptr);
		sdstrim(f," ");
		sds fname = sdscatprintf(sdsempty(),"%s%s", tpldir, f);
		fname = sdscat(fname,tplext);
		//puts(fname);

		FILE * fp;
		if ((fp = fopen(fname, "r")) != NULL) {
			fclose(fp);
			buff = render_template(f);
		}
		puts("free fname");
		sdsfree(fname);
		
	} 
	puts("free f");
	sdsfree(f);
//	//sdsfree(buff);
	puts("free re");
	sdsfree(re);
	return buff;
}

/**
 * Takes delimiter tags and sets the delimiter for the parser.
 * 
 * @param sds tag  The tag 
 *
 * @retval sdsempty()  Emtpy sds string.
 */
static sds tag_delimiter(sds tag) 
{
	puts("tag_delimiter");
	printf("tag found: '%s'\n",tag);
	sds re = sdsempty();
	sds oldl = sdsdup(ldelim), oldr = sdsdup(rdelim);
	re = sdscatprintf(re, "(%s=\\S+)\\s+(\\S+=%s)", oldl, oldr);
	struct slre_cap caps[2];

	int i;

	if ((i = slre_match(re, tag, sdslen(tag), caps, 2)) > 0) {
		ldelim = sdscpylen(ldelim, caps[0].ptr, caps[0].len);
		rdelim = sdscpylen(rdelim, caps[1].ptr, caps[1].len);
				
		oldl = sdscatprintf(sdsempty(), "%s=", oldl);
		oldr = sdscatprintf(sdsempty(), "=%s", oldr);

		ldelim = sdsreplace(ldelim, oldl, NULL);
		rdelim = sdsreplace(rdelim, oldr, NULL);

		printf("ldelim: '%s'\n", ldelim);
		printf("rdelim: '%s'\n", rdelim);
	}
	printf("Matched:  %d\n",i);
	puts("free oldl); sdsfree(oldr); sdsfree(re");
	sdsfree(oldl); sdsfree(oldr); sdsfree(re);
	return sdsempty();
}

/**
 * Wrapper for slre_match that returns all matches as an array.
 * 
 * @param const char *re  The regular expression to be matched.
 * @param const char *buf  The buffer to search in.
 * @param int bl  Length of buffer.
 * @param struct slre_cap *caps  Array to store subpatterns in.
 * @param int num_caps  Number of subpatterns in regular expression.
 *
 * @retval struct slre_cap*  Array of slre_cap structures.
 */
static struct slre_cap* slre_match_all(
		const char *re, 
		const char *buf, 
		int bl,
		struct slre_cap *caps, 
		int num_caps) 
{
	puts("slre_match_all");
	
	int count = 0, i = 0, j = 0;
	
	struct slre_cap* buffer;
	buffer = (struct slre_cap*)malloc(1024*sizeof(struct slre_cap)); 

	while (j < bl && 
		(i = slre_match(re, buf+j, bl-j, caps, num_caps)) > 0) {
		
		buffer[count]=caps[0];
		count++; j += i;
	}

	struct slre_cap *res = 
		(struct slre_cap*)malloc(count*sizeof(struct slre_cap));
	
	for (i=0; i<count; i++) {
		res[i] = buffer[i];
	}

	free((void*) buffer);

	return res;
}

/**
 * Wrapper for slre_match that counts the matches.
 * 
 * @param const char *re  The regular expression to be matched.
 * @param const char *buf  The buffer to search in.
 * @param int bl  Length of buffer.
 * @param struct slre_cap *caps  Array to store subpatterns in.
 * @param int num_caps  Number of subpatterns in regular expression.
 *
 * @retval int count  The number of matches in regular expression.
 */
static int slre_match_count(
		const char *re, 
		const char *buf, 
		int bl,
		struct slre_cap *caps, 
		int num_caps) 
{
	puts("slre_match_count");
	
	int count = 0, i = 0, j = 0;
	
	while (j < bl && 
		(i = slre_match(re, buf+j, bl-j, caps, num_caps)) > 0) {
		count++;
		j += i;
	}
	return count;
}

/**
 * Parses a matched tag and routes it to the relevant function.
 *
 * @param sds tag  The matched tag.
 *
 * @retval sds tag  The parsed tag.
 */
static sds parse_tag(sds tag)
{
	puts("parse_tag");
	sds buff = sdsempty(); 
	switch (tag[(strlen(ldelim))]) {
		case '!':
			buff = tag_comment(tag);
			break;
		case '>':
			buff = tag_partial(tag);
			break;
		case '=':
			buff = tag_delimiter(tag);
			break;
		default:
			buff = tag_variable(tag);
			break;
	}

	tag = sdscpylen(tag,buff,sdslen(buff)); 
	puts("free buff");
	//sdsfree(buff); 
	return tag;
}

/**
 *
 */
static sds parse_section(sds str, char *tag, char type)
{
	puts("parse_section");
	sds buff = sdsempty(); 
	switch (type) {
		case '#':
			buff = tag_section(str, tag);
			break;
		case '^':
			buff = tag_inverted(str, tag);
			break;
	}

//	tag = sdscpylen(tag,buff,sdslen(buff)); 
	puts("free buff");
	//sdsfree(buff); 
//	return tag;


	printf("section str: '%s'\n", str);
	printf("section tag: '%s'\n", tag);
	printf("section type: '%c'\n", type);

	return str;
}


/**
 * Parses a string buffer for mustache tags.
 *
 * @param sds str  The string buffer to parse.
 *
 * @retval sds str  The parsed string buffer.
 */
static sds match_tags(sds str)
{
	puts("match_tags");
	sds re = sdsempty(), s = sdsempty(), buff = sdsempty(); 
	re = sdscatprintf(re, "(%s[\\S\\s]+?%s)", ldelim, rdelim);

	struct slre_cap caps[1];
	int i, sl = sdslen(str);

	struct slre_cap* match = slre_match_all(re, str, sl, caps, 1);
	int matches = slre_match_count(re, str, sl, caps, 1);
	
	sds remain = sdsdup(str);
	sds part = sdsempty();
	char *pos, *end, type;
	
	for (i=0; i < matches; i++) {
		s = sdsempty(); 
		s = sdscatprintf(s,"%.*s", match[i].len, match[i].ptr);
		
		if (!(pos = strstr(remain, s)))
			continue;

		part = sdscpylen(part, remain, pos-remain); 

		remain = sdscpy(remain, pos+sdslen(s)); 
	
		buff = sdscat(buff,part);

		type = s[(strlen(ldelim))];
		
		if (type == '#' || type == '^') {
			sds endtag = sdsdup(s);
			sds tag = sdscpylen( 
				sdsempty(), 
				( s + sdslen(ldelim) + 1 ),
				(
					sdslen(s) -
					sdslen(ldelim) - 
					sdslen(rdelim) - 1
				)
			);

			endtag[(sdslen(ldelim))] = '/';

			end = strstr(remain, endtag);
			printf("part: '%s'\n", part);
			
			s = sdscatlen(sdsempty(), remain, end-remain); 
			printf("search: '%s'\n", s);
			
			remain = sdscpy(remain, end+sdslen(endtag)); 
			printf("remain: '%s'\n", remain);

			//sds tmp = sdsempty();
			//tmp = match_tags(tmp);
			buff = parse_section(s, tag, type);
			//sdsfree(tmp);
			//buff = sdscat(buff, tmp);
		} else {
			buff = sdscat(buff,parse_tag(s));
			puts("foo");		
		}


	}

	buff = sdscat(buff,remain);
	str = sdscpylen(str,buff,sdslen(buff)); 
	
	puts("free part");
	sdsfree(part);
	puts("free remain");
	sdsfree(remain);
	puts("free buff");
	//sdsfree(buff);
	puts("free re");
	sdsfree(re);
	puts("free s");
//	sdsfree(s);
//	puts("return");
	return str;
}

/**
 * @todo  Switch function over to SDS strings.
 */
char *read_file(char *file)
{
	puts("read_file");
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

bool set_context(sds json)
{
	int err, ctx_size = sizeof(ctx) / sizeof(ctx[0]);
	if ((err = parse_json(json, strlen(json), ctx, ctx_size)) > 0) {
		return true;
	} else {
		switch (err) {
			case JSON_STRING_INVALID:
				fputs("JSON string invalid", stderr);
				break;
			case JSON_STRING_INCOMPLETE:
				fputs("JSON string incomplete", stderr);
				break;
			case JSON_TOKEN_ARRAY_TOO_SMALL:
				fputs("Token array too small", stderr);
				break;
		}
		return false;
	}
}

void mustache_init(char *td, char *te, char *rd, char *ld)
{
	puts("mustache_init");
	rdelim = rd ? sdsnew(rd) : sdsnew("}}");
	ldelim = ld ? sdsnew(ld) : sdsnew("{{"); 
	tpldir = td ? sdsnew(td) : sdsnew("templates/mustache/");
	tplext = te ? sdsnew(te) : sdsnew(".mustache");
}

sds render_template(sds tpl)
{
	puts("render_template");
	sds file = sdsdup(tpldir);
	file = sdscat(file, tpl);
	file = sdscat(file, tplext);

	sds buff = sdsnew(read_file(file));
	puts("free file");
	sdsfree(file);
	buff = match_tags(buff);
	return buff;
}
