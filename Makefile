INSTALL_PATH?=/usr/local

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
		mongoose.c

CFLAGS = -std=c99

all: build
#	some stuff

build: setup compile

setup:
	mkdir -p build

compile:
	gcc $(SOURCE_FILES) -o build/journal $(CFLAGS)

clean:
	rm -rf build/

install:
	#mv journal $INSTALL_PATH/bin
	#mv libraries $INSTALL_PATH/lib or $INSTALL_PATH/lib/yourappname
	#mv journal.1 $INSTALL_PATH/share/man/man1
	#other docs go in $INSTALL_PATH/share/yourappname	
	#mv journal.conf $INSTALL_PATH/etc/yourappname
	#mv headers $INSTALL_PATH/include/yourappname

uninstall:
	#rm $INSTALL_PATH/bin/journal
	#rm $INSTALL_PATH/share/man/man1/journal.1
	#etc
