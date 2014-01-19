INSTALL_PATH?=/usr/local
CC?=gcc
CFLAGS?=

#CFLAGS += -std=c99 -Wall

PROGRAM = journal
SOURCE_FILES = 	journal.c \
		cmd_*.c \
		validate.c \
		slre.c \
		frozen.c \
		sha1c.c \
		sha1hl.c \
		hoedown/*.c \
		build/embed_data.c

EMBED_SOURCE =  test/syntax.md

# gmake specific syntax, please fix me.
ifeq ($(OS),Windows_NT)
	CFLAGS += -U__STRICT_ANSI__ -D_NO_OLDNAMES -I/d/lib/sfu/SDK/usr/inlcude -I/d/lib/sfu/SDK/opt/gcc.3.3/lib/gcc-lib/i586-pc-interix3/3.3/include/
else
	CFLAGS += -std=c99 -Wall -pthread
	SOURCE_FILES += mongoose.c
endif


all: compile
	@echo "Done."

build: setup compile

setup:
	@echo "Setting up directories and generating dependecies."
	@mkdir -p build
	@$(CC) embed.c -o build/embed
	@build/embed $(EMBED_SOURCE) > build/embed_data.c
	@echo "Done setting up."

compile:
	@echo "Compiling \"$(PROGRAM)\" program."
	$(CC) $(SOURCE_FILES) -o build/$(PROGRAM) $(CFLAGS)
	@echo "$(PROGRAM) built successfully."

clean:
	@echo "Cleaning up..."
	@rm -rf build/

install: build
	@echo "Installing \"$(PROGRAM)\" program ..."
	@echo "Copying executables to $(INSTALL_PATH)/bin ..."
#	@cp build/$(PROGRAM) $(INSTALL_PATH)/bin
	@echo "Copying libraries to $(INSTALL_PATH)/lib..."
#	@cp libraries $(INSTALL_PATH)/lib or $(INSTALL_PATH)/lib/$(PROGRAM)
	@echo "Copying manual pages from $(INSTALL_PATH)/share/man/ ..."
#	@cp $(PROGRAM).1 $(INSTALL_PATH)/share/man/man1
	@echo "Copying documentation to $(INSTALL_PATH)/share/$(PROGRAM) ..."
#	@cp $(PROGRAM).md $(PROGRAM).pdf $(INSTALL_PATH)/share/$(PROGRAM)	
	@echo "Copying configuration files to $(INSTALL_PATH)/etc/$(PROGRAM) ..."
#	@cp $(PROGRAM).conf $(INSTALL_PATH)/etc/$(PROGRAM)
	@echo "Copying source header files to $(INSTALL_PATH)/include/$(PROGRAM)..."
#	@cp headers $(INSTALL_PATH)/include/$(PROGRAM)
	@echo "$(PROGRAM) installed successfully."

uninstall:
	@echo "Installing \"$(PROGRAM)\" program ..."
	@echo "Removing executables from $(INSTALL_PATH)/bin ..."
#	rm -rf $(INSTALL_PATH)/bin/$(PROGRAM)
	@echo "Removing libraries from $(INSTALL_PATH)/lib..."
	@echo "Removing manual pages from $(INSTALL_PATH)/share/man/ ..."
	@echo "Removing documentation from $(INSTALL_PATH)/share/$(PROGRAM) ..."
#	rm -rf $(INSTALL_PATH)/share/man/man1/$(PROGRAM).1
	@echo "Removing configuration files from $(INSTALL_PATH)/etc/$(PROGRAM) ..."
#	rm -rf $(INSTALL_PATH)/etc/$(PROGRAM)
	@echo "Removing source header files from $(INSTALL_PATH)/include/$(PROGRAM)..."
	@echo "$(PROGRAM) uninstalled successfully."
