INSTALL_PATH? =/usr/local
          CC? =cc
      CFLAGS? =

CFLAGS += -std=c99

SOURCE_FILES = 	journal.c \
		validate.c \
		slre.c \
		frozen.c \
		sundown/markdown.c \
		sundown/autolink.c \
		sundown/buffer.c \
		sundown/stack.c \
		sundown/html.c \
		sundown/houdini_html_e.c \
		sundown/houdini_href_e.c \
		build/embed_data.c

EMBED_SOURCE =  test/syntax.md

ifeq ($(OS),Windows_NT)
    CFLAGS += -U__STRICT_ANSI__ -D_NO_OLDNAMES
else
    CFLAGS += -pthread
    SOURCE_FILES += mongoose.c
endif



all: compile
#	some stuff

build: setup compile

setup:
	mkdir -p build
	$(CC) embed.c -o build/embed
	build/embed $(EMBED_SOURCE) > build/embed_data.c

compile:
	$(CC) $(SOURCE_FILES) -o build/journal $(CFLAGS)

clean:
	rm -rf build/

install:
#	cp build/journal $INSTALL_PATH/bin
#	cp libraries $INSTALL_PATH/lib or $INSTALL_PATH/lib/journal
#	cp journal.1 $INSTALL_PATH/share/man/man1
#	cp journal.md journal.pdf $INSTALL_PATH/share/journal	
#	cp journal.conf $INSTALL_PATH/etc/journal
#	cp headers $INSTALL_PATH/include/journal

uninstall:
#	rm -rf $(INSTALL_PATH)/bin/journal
#	rm -rf $(INSTALL_PATH)/share/man/man1/journal.1
#	rm -rf $(INSTALL_PATH)/etc/journal
