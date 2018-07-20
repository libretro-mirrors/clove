OS=$(shell uname -s)
DIR=$(shell pwd)

# AR_FLOAT; all numbers are treated as floats else as double
CFLAGS=-std=c89 -DAR_FLOAT -g -DAR_STANDALONE -Wall -Wextra -I. -I/utils -O3
LIBS=-lm -ldl
# local (header only) depedencies located in utils/
DEPS=utils/vector.h  utils/hash.h
DEPS_OBJ=$(patsubst %.c,%.o,$(DEPS))

# On Windows you have to specify where gcc is located, thanks to MasterGeek_ on ##netcode, Freenode.
# On Linux gcc is default, macOS has an alis for it but it's clang actually, afaik
CC=
ifeq ($(OS), Windows_NT)
	CC +=/usr/bin/x86_64-w64-mingw32-gcc
else
	CC +=gcc
endif

SRC=aria.c ar_math.c ar_vector.c ar_list.c ar_io.c ar_string.c ar_map.c utils/hash.c utils/vector.c
OBJ=$(patsubst %.c,%.o,$(SRC))

# Place where the binary will be placed
BIN_DIR=bin

all: make_directories aria

# BINARIES
aria: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/$@ $(LIBS)

#TODO Make this work on Windows as well...
install: make_directories aria
ifneq ($(OS), Windows_NT)
	cp $(BIN_DIR)/aria /usr/local/bin
endif

uninstall:
ifneq ($(OS), Windows_NT)
	rm -rf /usr/local/bin/aria
endif

web: $(DEPS_OBJ) $(OBJ)
		emcc -o $(BIN_DIR)/index.html -s ALLOW_MEMORY_GROWTH=1
#END OF BINARIES

# MISC
#my_lib = FFI test
ifeq ($(OS), Linux)
my_lib:
	$(CC) $(CFLAGS) -fPIC -shared $(SCR) script/my_lib.c -o my_lib.so $(LIBS) -ldl; mv my_lib.so script/
endif
ifeq ($(OS), Darwin)
my_lib:
	$(CC) $(CFLAGS) -dynamiclib $(SRC) script/my_lib.c -o my_lib.dylib $(LIBS); mv my_lib.dylib script/
endif
ifeq ($(OS), Windows_NT)
my_lib:
	$(CC) $(CFLAGS) -fPIC -shared $(SRC) script/my_lib.c -o my_lib.dll $(LIBS) -ldl; mv my_lib.dll script/
endif


make_directories:
	mkdir -p $(BIN_DIR)


# END OF MISC


.PHONY: clean my_lib web
clean:
	rm -rf $(BIN_DIR) *.o *.gch *.a

