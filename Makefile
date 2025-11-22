CC      = gcc
CXX     = g++
CFLAGS  = -Wall -std=c99 -DSDL_MAIN_HANDLED -DCIMGUI_USE_SDL2 -DCIMGUI_USE_OPENGL3

SRC_DIR = ./Main/Src
SRCS    = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(SRCS:.c=.o)

OUT     = renderer

# Path to your cimgui repo & built libcimgui.a
CIMGUI_DIR = C:/cimgui

INCLUDE_DIRS = \
	-I./include \
	-IC:/msys64/mingw64/include \
	-IC:/msys64/mingw64/include/SDL2 \
	-I$(CIMGUI_DIR) \
	-I$(CIMGUI_DIR)/imgui \
	-I$(CIMGUI_DIR)/generator/output

LIB_DIRS = \
	-LC:/msys64/mingw64/lib \
	-L$(CIMGUI_DIR)

LIBS = \
	-lopengl32 \
	-lglew32 \
	-lcglm \
	-lassimp \
	-lm \
	-lcimgui \
	-lSDL2


all: build

build: $(OUT)

# Compile C files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

# Link with g++
$(OUT): $(OBJS)
	$(CXX) -o $@ $^ $(LIB_DIRS) $(LIBS)

debug-build: CFLAGS += -g
debug-build: clean $(OUT)

debug:
	gdb $(OUT).exe

run:
	./$(OUT)

clean:
	- del "$(OUT).exe"
	- del /q "Main\Src\*.o"

.PHONY: all build debug-build debug run clean
