/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 *
 * validate.c
 *    Input validation function definitions for the journal project.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "validate.h"

int val_date(char *date)
{
	int year, month, day;
	time_t t = time(NULL);
	struct tm *now = localtime(&t);

	if (sscanf(date, "%d-%d-%d", &year, &month, &day) == 3) {
		printf("Validating date \"%s\"\n",date);
		if ((year - 1900) <= now->tm_year) {
			puts("year is current or old");
			if ((month-1) <= now->tm_mon) {
				puts("month is current or old");
				if (day < now->tm_mday) {
					puts("Warning: date is in the past");
				}
			} 
//			return 1;
		}
		puts ("date isn't in the past");

		if (month > 12) {
			puts("Too many months");
			return 1;
		}

		puts ("month isn't imaginary");
		
		switch (month) {
			case 2: 
				if (((year % 4 == 0) && (year % 100 != 0))
					|| (year % 400 == 0)) {
					if (day > 29) {
						puts("Too many days");
						return 1;
					}
					
				} else {
					if (day > 28) {
						puts("Too many days");
						return 1;
					}
				}
				break;
			case 3:
			case 5:
			case 9:
			case 11:
				if (day > 30) {
					puts("Too many days");
					return 1;
				}
				break;
			default:
				if (day > 31) {
					puts("Too many days");
					return 1;
				}
				break;
		}
		puts("Date formatted ok");
		return 0;
	} else {
		puts("Warning: Invalid date format.");
		return 1;
	}
}
