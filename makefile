PROGRAM_NAME=client
SERVER_NAME=server
SOURCES=src/main.cpp src/engine.cpp src/gameClass.cpp src/vectors.cpp src/mouse.cpp src/usefull.cpp src/serverClient.cpp
SOURCES2=src/server.cpp src/serverClient.cpp

####################
OUTPUT_DIR=bin
LINUX_GCC=g++
LINUX_CFLAGS=-pthread -std=c++17 -Wall -Wextra
ALLEGRO5_LINUX=`pkg-config --cflags --libs allegro-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5 allegro_color-5`
export PKG_CONFIG_PATH=/usr/i686-w64-mingw32/lib/pkgconfig/
####################

.PHONY: all run clean help linux

all: linux

help:
	@echo "##### HELP #####"
	@echo "make all - build for linux and windows"
	@echo "make clean - clean up"
	@echo "make linux - build for linux only"
	@echo "make windows - build for windows only"
	@echo "make help - print this help"
	@echo "################"

linux:
	mkdir -p $(OUTPUT_DIR)
	cp -R fonts $(OUTPUT_DIR)
	cp -R img $(OUTPUT_DIR)
	cp -R maps $(OUTPUT_DIR)
	cp -R config $(OUTPUT_DIR)
	$(LINUX_GCC) $(LINUX_CFLAGS) -o $(OUTPUT_DIR)/$(PROGRAM_NAME) $(SOURCES) $(ALLEGRO5_LINUX)
	$(LINUX_GCC) $(LINUX_CFLAGS) -o $(OUTPUT_DIR)/$(SERVER_NAME) $(SOURCES2) $(ALLEGRO5_LINUX)


clean:
	rm -rf $(OUTPUT_DIR)
