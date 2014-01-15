/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 *
 * validate.h
 *    Input validation function declarations for the journal project.
 */

#include <stdbool.h>

int val_date(char *date);
void trim(char *str);
bool empty(char *str);
