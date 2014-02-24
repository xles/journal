/**
 * Journal, the UNIX user's personal log and diary.
 *
 * Copyright (c) 2014 xles, 
 *    see LICENSE file for details.
 * 
 * mustache.c
 *    Main source file for the journal project.
 */
#include "sds.h"

struct tags {
	struct tags *next;
	const char *key;
	const char *value;
};


/**
 * Initializer for the mustache template engine.
 *
 * @param char *td  The template directory.
 * @param char *rd  The right delimiter.
 * @param char *ld  The left delimiter.
 *
 * Typical usage:
 * @code
 *    mustache_init(NULL, NULL, NULL); // Init with defaults
 * @endcode
 */
extern void mustache_init(char *td, char *te, char *rd, char *ld);

/**
 * Renders a template file.
 *
 * @param char *tpl  The template to open.
 *
 * @retval sds buff  The template buffer.
 */
extern sds render_template(sds tpl);

/**
 * Opens a file and reads it to a buffer.
 *
 * @param char *file  The filename to open.
 *
 * @retval char *buff  The file buffer.
 * @retval NULL  on failure
 */
extern char *read_file(char *file);
