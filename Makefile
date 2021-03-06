# Makefile for Scumm Translations
 
# *****************************************************
# Variables to control Makefile operation
INCLUDES=-Ithird-party/cxxopts/include
CC_LINUX = g++
CFLAGS_LINUX = -Wall -g -std=c++1z

CC = g++
CFLAGS = -Wall -g $(INCLUDES)

CC_WIN = i686-w64-mingw32-g++
CFLAGS_WIN = -Wall -g -static $(INCLUDES)

BINARY_LINUX = ./bin/linux/scummtr2po
BINARY_WIN = ./bin/windows/scummtr2po.exe

OBJS=main.cpp scummtr2po.cpp stringId.cpp objects.cpp

# ****************************************************
# Targets needed to bring the executable up to date

all: release

release: target_windows target_linux

target_windows: $(OBJS)
	$(CC_WIN) $(CFLAGS_WIN) -o $(BINARY_WIN) $(OBJS)

target_linux: $(OBJS)
	$(CC_LINUX) $(CFLAGS_LINUX) -o $(BINARY_LINUX) $(OBJS)
	

.PHONY: clean
clean:
	rm $(BINARY_LINUX) $(BINARY_WIN)
