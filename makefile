CC := gcc
LL      := ar

INCLUDEDIR := $(pwd)/include

CFLAGS  := -Wall -Wextra -std=c99 -Iheaders

SRC_     := $(notdir $(wildcard src/*.c))
SRC      := $(wildcard src/*.c)
OBJ     := $(addprefix build/, $(SRC_:.c=.o))

TARGET := build/lib/linux/libnet.a

LDFLAGS := 
LDLIBS := 


MINGW := x86_64-w64-mingw32-gcc
MINGW_LL := x86_64-w64-mingw32-ar

MINGW_FLAGS := -Wall -Wextra -std=c99 -I$(INCLUDEDIR) -g \
	       -I/usr/x86_64-w64-mingw32/include/ \
	       -Wl,-subsystem,windows -lmingw32 -lws2_32 
MINGW_LDLIBS := -L/usr/x86_64-w64-mingw32/lib/ 

MINGW_OBJ := $(addprefix build/, $(SRC_:.c=.obj))

MINGW_TARGET := build/lib/win/libnet.lib

all: linux windows 

linux: $(TARGET)

windows: $(MINGW_TARGET)

$(TARGET): $(OBJ)
	$(LL) rcs $@ $^

$(MINGW_TARGET): $(MINGW_OBJ) 
	$(MINGW_LL) rcs $(@) $^

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(MINGW_OBJ): $(SRC)
	$(MINGW) $(MINGW_FLAGS) -c $< -o $@
clean: 
	rm $(OBJ) $(MINGW_OBJ) $(TARGET) $(MINGW_TARGET) 
