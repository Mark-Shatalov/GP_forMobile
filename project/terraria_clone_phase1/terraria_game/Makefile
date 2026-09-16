# Makefile for the Terraria-style sandbox game
#
# This assumes raylib is installed on your system (see README.md for
# install instructions on Linux/macOS/Windows). It does NOT bundle
# raylib itself, so the project stays small and portable.

CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Isrc
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/game

# Automatically pick up every .c file in src/ so you don't have to edit
# the Makefile every time a new module (world.c, player.c, ...) is added.
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
