FUZIX_ROOT ?= $(HOME)/src/fuzix
TARGET ?= coco3
include $(FUZIX_ROOT)/Kernel/platform/platform-$(TARGET)/target.mk
ifeq ($(USERCPU),)
	USERCPU = $(CPU)
endif
include $(FUZIX_ROOT)/Target/rules.$(USERCPU)
export FUZIX_ROOT

# Compiler and flags
#CC := gcc
#CFLAGS := -Wall -Wextra
CFLAGS += -std=c99 -Os -Wall -Wextra

# Debug flags with sanitizers
#DEBUG_FLAGS := -fsanitize=address,undefined

# Source and object files
SRC_DIR := ./src
INCLUDE_DIR := ./include
BIN_DIR := ./bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SRC_FILES))

# Installation directory
INSTALL_DIR := /usr/local/bin

# Output target
TARGET := 2048

# Phony targets
.PHONY: all debug release run install uninstall clean

# Default target
all: debug

# Build rule
$(BIN_DIR)/$(TARGET): $(OBJ_FILES)
	#$(CC) -I $(INCLUDE_DIR) $(CFLAGS) $^ -o $@
	$(LINKER) $(LINKER_OPT) -o $@ $(CRT0) $(OBJ_FILES) $(LINKER_TAIL)

# Compile source files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) -I $(INCLUDE_DIR) $(CFLAGS) -c $< -o $@

# Create bin directory
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(BIN_DIR)/$(TARGET)

# Release build
release: clean $(BIN_DIR)/$(TARGET)

# Compile and run
run: debug
	$(BIN_DIR)/$(TARGET)

# Install the executable
install: release
	cp $(BIN_DIR)/$(TARGET) $(INSTALL_DIR)

# Uninstall the executable
uninstall:
	rm -f $(INSTALL_DIR)/$(TARGET)

# Clean up
clean:
	rm -f $(BIN_DIR)/*.o
	rm -f $(BIN_DIR)/$(TARGET)
