/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 *
 * colour.h
 *    ANSI terminal colour macros, "inspired by" and/or stolen from
 *    color.h in the [git project](http://git-scm.com/).
 */

/*  2 + (2 * num_attrs) + 8 + 1 + 8 + 'm' + NUL */
/* "\033[1;2;4;5;7;38;5;2xx;48;5;2xxm\0" */
/*
 * The maximum length of ANSI color sequence we would generate:
 * - leading ESC '['            2
 * - attr + ';'                 2 * 8 (e.g. "1;")
 * - fg color + ';'             9 (e.g. "38;5;2xx;")
 * - fg color + ';'             9 (e.g. "48;5;2xx;")
 * - terminating 'm' NUL        2
 *
 * The above overcounts attr (we only use 5 not 8) and one semicolon
 * but it is close enough.
 */

#define COLOR_MAXLEN 40

#define ANSI_COLOR_NORMAL   ""
#define ANSI_COLOR_START    "\033["
#define ANSI_COLOR_END      "m"
#define ANSI_COLOR_RESET    "\033[m"
#define ANSI_COLOR_BOLD     "\033[1m"

#define ANSI_COLOR_RED           "\033[31m"
#define ANSI_COLOR_GREEN         "\033[32m"
#define ANSI_COLOR_YELLOW        "\033[33m"
#define ANSI_COLOR_BLUE          "\033[34m"
#define ANSI_COLOR_MAGENTA       "\033[35m"
#define ANSI_COLOR_CYAN          "\033[36m"

#define ANSI_COLOR_BOLD_RED      "\033[1;31m"
#define ANSI_COLOR_BOLD_GREEN    "\033[1;32m"
#define ANSI_COLOR_BOLD_YELLOW   "\033[1;33m"
#define ANSI_COLOR_BOLD_BLUE     "\033[1;34m"
#define ANSI_COLOR_BOLD_MAGENTA  "\033[1;35m"
#define ANSI_COLOR_BOLD_CYAN     "\033[1;36m"

#define ANSI_COLOR_BG_RED        "\033[41m"
#define ANSI_COLOR_BG_GREEN      "\033[42m"
#define ANSI_COLOR_BG_YELLOW     "\033[43m"
#define ANSI_COLOR_BG_BLUE       "\033[44m"
#define ANSI_COLOR_BG_MAGENTA    "\033[45m"
#define ANSI_COLOR_BG_CYAN       "\033[46m"

/* A special value meaning "no color selected" */
#define ANSI_COLOR_NIL "NIL"

/*
0        All attributes off
1        Bold on
4        Underscore (on monochrome display adapter only)
5        Blink on
7        Reverse video on
8        Concealed on

Foreground colors
30        Black
31        Red
32        Green
33        Yellow
34        Blue
35        Magenta
36        Cyan
37        White
 
Background colors
40        Black
41        Red
42        Green
43        Yellow
44        Blue
45        Magenta
46        Cyan
47        White
*/

/*
 * The first three are chosen to match common usage in the code, and what is
 * returned from git_config_colorbool. The "auto" value can be returned from
 * config_colorbool, and will be converted by want_color() into either 0 or 1.
 */
#define ANSI_COLOR_UNKNOWN -1
#define ANSI_COLOR_NEVER  0
#define ANSI_COLOR_ALWAYS 1
#define ANSI_COLOR_AUTO   2
