CC = zig cc
CFLAGS = -O2 -fPIC

# Detect OS
ifeq ($(OS),Windows_NT)
    RM = rmdir /s /q
    MKDIR = mkdir
    CP = xcopy /e /i /y
    SEP = \\
    LIB_EXT = dll
else
    RM = rm -rf
    MKDIR = mkdir -p
    CP = cp -r
    SEP = /
    LIB_EXT = so
endif

# Source files
SRCS = core/tetris.c core/input.c core/rng.c core/queue/queue.c

# Build
BUILD_DIR = build
OUT_DIR = $(BUILD_DIR)/$(LIB_EXT)

LIB_NAME = libgame_core.$(LIB_EXT)

all: $(LIB_NAME)

$(LIB_NAME): $(SRCS)
	$(MKDIR) $(OUT_DIR)
	$(CC) $(CFLAGS) -shared -o $(OUT_DIR)/$(LIB_NAME) $(SRCS)
	$(CP) $(OUT_DIR) ..$(SEP)client$(SEP)game_core$(SEP)$(LIB_EXT)

clean:
	$(RM) $(BUILD_DIR)

.PHONY: all clean