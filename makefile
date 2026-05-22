CC := gcc
LL := ar

INCLUDEDIR := $(CURDIR)/include/

CFLAGS  := -Wall -Wextra -std=c99 -Iheaders -I$(INCLUDEDIR)

# --- Linux Config ---
SRC_BASE  := $(wildcard src/*.c)
SRC_LINUX := $(wildcard src/linux/*.c)
# Keep object arrays isolated to their actual file paths
OBJ_LINUX := $(patsubst src/%.c, build/linux/%.o, $(SRC_BASE)) \
             $(patsubst src/linux/%.c, build/linux/%.o, $(SRC_LINUX))

TARGET := build/lib/linux/libnet.a

# --- Windows (MinGW) Config ---
MINGW    := x86_64-w64-mingw32-gcc
MINGW_LL := x86_64-w64-mingw32-ar

MINGW_FLAGS := -Wall -Wextra -std=c99 -I$(INCLUDEDIR) -g \
               -I/usr/x86_64-w64-mingw32/include/

SRC_WIN   := $(wildcard src/win/*.c)
OBJ_WIN   := $(patsubst src/%.c, build/win/%.obj, $(SRC_BASE)) \
             $(patsubst src/win/%.c, build/win/%.obj, $(SRC_WIN))

MINGW_TARGET := build/lib/win/libnet.lib

# Tell Make these are commands, not actual file outputs
.PHONY: all linux windows clean

all: linux windows 

linux: $(TARGET)

windows: $(MINGW_TARGET)

# --- Linux Build Rules ---
$(TARGET): $(OBJ_LINUX)
	@mkdir -p $(dir $@)
	$(LL) rcs $@ $^

# Pattern rule for base files
build/linux/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for linux-specific subfolder files
build/linux/%.o: src/linux/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


# --- Windows Build Rules ---
$(MINGW_TARGET): $(OBJ_WIN)
	@mkdir -p $(dir $@)
	$(MINGW_LL) rcs $@ $^

# Pattern rule for base files compiled for windows
build/win/%.obj: src/%.c
	@mkdir -p $(dir $@)
	$(MINGW) $(MINGW_FLAGS) -c $< -o $@

# Pattern rule for windows-specific subfolder files
build/win/%.obj: src/win/%.c
	@mkdir -p $(dir $@)
	$(MINGW) $(MINGW_FLAGS) -c $< -o $@


clean: 
	rm -rf build/
