INSTALL_PATH?=/usr/local

all: build
	#some stuff

build:
	mkdir -p build
	cc journal.c mongoose.c cJSON.c -o build/journal

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
