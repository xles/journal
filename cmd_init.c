/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * cmd_init.c
 *    Main source file for the journal project.
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "journal.h"

#ifdef _WIN32
 #define getcwd(a,b) _getcwd(a,b)
 #define _mkdir(a,b) mkdir(a)
#else
 #define _WIN32 0
 #define _mkdir(a,b) mkdir(a,b)
#endif


int mkpage(void)
{
	size_t size;
	const char *data = find_embedded_file("test/syntax.md", &size);	

	FILE *fp = fopen(".journal/posts/syntax.md", "w+");

	fputs(data, fp);

	fclose(fp);

//	printf("file:\n%s\n", data);
	return 0;
}

int cmd_init(int argc, char **argv)
{
	char *cwd;
	cwd = getcwd(NULL,64);
	if (argv[optind+1] != NULL) {
		if (!strncmp(&argv[optind+1][0],"/",1)) {
			cwd = argv[optind+1];
		} else {
			if (!strncmp(&argv[optind+1][1],"/",1)) {
				sprintf(cwd,"%s/%s",cwd,argv[optind+1]+2);
			} else {
				sprintf(cwd,"%s/%s",cwd,argv[optind+1]);
			}
		}
	}
	printf("Initializing new journal in '%s'\n", cwd);
	char target[strlen(cwd)+10];
	strncpy(target,cwd,sizeof(target));
	snprintf(target,sizeof(target),"%s/%s",target,".journal");
	int result = _mkdir(target, 0755);
	if (result < 0) {
	 	printf("Error: %s in '%s'\n", strerror(errno), cwd);
		return 1;
	}
	FILE *fp = fopen(".journal/.htaccess", "w+");

	fputs( "order deny,allow\n"
		"deny from all\n"
		"allow from 127.0.0.1\n",fp);
	fclose(fp);

	_mkdir(".journal/posts", 0755);
	mkpage();
	puts("Successfully initialized a new journal.");
	return 0;
}
