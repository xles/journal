#include <ctype.h>

void parse_options(char *options);
void new(char **argv);
void serve(void);
void usage(void);
int test_markdown(char *file);
int new_post(void);
int val_date(char *date);

void trim(char *str)
{
	char * _str = str;
	if (strlen(_str) == 0) return;

	while (isspace(_str[strlen(_str)-1]))
		_str[strlen(_str)-1] = '\0';

	while (isspace(_str[0]))
		_str++;

	sprintf(str,"%s",_str);

	return;
}

bool empty(char *str)
{
	if (strlen(str) == 0) 
		return true;
	else
		return false;
}
