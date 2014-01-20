/* html.h - HTML renderer */

#ifndef HOEDOWN_HTML5_H
#define HOEDOWN_HTML5_H

#include "markdown.h"
#include "buffer.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	HOEDOWN_HTML5_SKIP_HTML = (1 << 0),
	HOEDOWN_HTML5_SKIP_STYLE = (1 << 1),
	HOEDOWN_HTML5_SKIP_IMAGES = (1 << 2),
	HOEDOWN_HTML5_SKIP_LINKS = (1 << 3),
	HOEDOWN_HTML5_EXPAND_TABS = (1 << 4),
	HOEDOWN_HTML5_SAFELINK = (1 << 5),
	HOEDOWN_HTML5_TOC = (1 << 6),
	HOEDOWN_HTML5_HARD_WRAP = (1 << 7),
	HOEDOWN_HTML5_USE_XHTML = (1 << 8),
	HOEDOWN_HTML5_ESCAPE = (1 << 9),
	HOEDOWN_HTML5_PRETTIFY = (1 << 10)
} hoedown_html5_render_mode;

typedef enum {
	HOEDOWN_HTML5_TAG_NONE = 0,
	HOEDOWN_HTML5_TAG_OPEN,
	HOEDOWN_HTML5_TAG_CLOSE
} hoedown_html5_tag;

struct hoedown_html5_renderer_state {
	struct {
		int header_count;
		int current_level;
		int level_offset;
		int nesting_level;
	} toc_data;

	unsigned int flags;

	/* extra callbacks */
	void (*link_attributes)(hoedown_buffer *ob, const hoedown_buffer *url, void *self);
};

typedef struct hoedown_html5_renderer_state hoedown_html5_renderer_state;

int
hoedown_html5_is_tag(const uint8_t *tag_data, size_t tag_size, const char *tagname);

extern hoedown_renderer *
hoedown_html5_renderer_new(unsigned int render_flags, int nesting_level);

extern hoedown_renderer *
hoedown_html5_toc_renderer_new(int nesting_level);

extern void
hoedown_html5_renderer_free(hoedown_renderer *renderer);

extern void
hoedown_html5_smartypants(hoedown_buffer *ob, const uint8_t *text, size_t size);

#ifdef __cplusplus
}
#endif

#endif /** HOEDOWN_HTML5_H **/
