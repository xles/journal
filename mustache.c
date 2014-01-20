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

#include "journal.h"
#include "slre.h"

int render_template(void)
{
	const char *request = " GET /index.html HTTP/1.0\r\n\r\n";
	struct slre_cap caps[4];

	if (slre_match("^\\s*(\\S+)\\s+(\\S+)\\s+HTTP/(\\d)\\.(\\d)",
			request, strlen(request), caps, 4) > 0) {
		printf("Method: [%.*s], URI: [%.*s]\n",
			caps[0].len, caps[0].ptr,
			caps[1].len, caps[1].ptr);
	} else {
		printf("Error parsing [%s]\n", request);
	}
}
