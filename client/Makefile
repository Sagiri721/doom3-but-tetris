ifeq ($(OS),Windows_NT)
    LDFLAGS = -lgdi32 -lole32 -ld3d11 -ldxgi
else ifeq ($(shell uname -s),Darwin)
    LDFLAGS = -framework Cocoa -framework QuartzCore -framework Metal -framework MetalKit
else
    LDFLAGS = -lGL -lX11 -lXi -lXcursor -lpthread -lm -ldl
endif

FLAGS = -Wall -Wextra
TARGET = app

SOURCES = \
    src/main.c \
	src/game.c \
    src/core/tetris.c \
	src/core/input.c \
	src/core/providers/input_keyboard.c \
	src/core/providers/input_cpu.c \
    src/gfx/render.c \
	src/queue/queue.c \
	src/gfx/image.c \
	src/gfx/bitmap_text.c

main:
	zig cc $(SOURCES) -o build/$(TARGET).exe $(LDFLAGS) $(FLAGS)

run: main
	./build/app.exe