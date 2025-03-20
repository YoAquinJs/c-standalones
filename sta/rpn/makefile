CC = gcc
CFLAGS = -Wall -Wextra
TEST_CFLAGS = -lcriterion

# targets
TARGET := $(notdir $(CURDIR))
TEST_TARGET = $(TARGET)-test

# parameters
ARGS ?=
RELEASE ?= 0

ifeq ($(RELEASE), 1)
	CFLAGS += -O2
else
	CFLAGS += -g -DDEBUG
endif

# directories
SRC_DIR = src
TESTS_DIR = tests
BUILD_DIR = build
UTILS_DIR = '../utils'

# absolute imports
CFLAGS += -I $(SRC_DIR)
CFLAGS += -I $(UTILS_DIR)

# files and obj files
UTILS_FILES = $(shell find $(UTILS_DIR) -name '*.c')
UTILS_OBJS = $(UTILS_FILES:.c=.o)

TEST_FILES = $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJS = $(TEST_FILES :.c=.o)

FILES = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(FILES:.c=.o)

# execution rules
all: $(TARGET)

run: $(TARGET)
	@echo "./$(BUILD_DIR)/$(TARGET) $(ARGS)"
	@./$(BUILD_DIR)/$(TARGET) $(ARGS) || echo "exit with status code $$?"

test: $(TEST_TARGET)
	@echo "testing $(TARGET)"
	@echo "./$(BUILD_DIR)/$(TEST_TARGET) $(ARGS)"
	@./$(BUILD_DIR)/$(TARGET) $(ARGS) || echo "exit with status code $$?"

clean:
	@rm -f $(OBJS) $(TESTS_OBJS)

clean-all: clean
	@rm -rf $(BUILD_DIR) $(UTILS_OBJS)

.PHONY: all test build run clean clean-all

# compilation rules
$(TARGET): $(BUILD_DIR)/$(TARGET)
$(TEST_TARGET): $(BUILD_DIR)/$(TEST_TARGET)
.PHONY: $(TARGET) $(TEST_TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS) $(UTILS_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(UTILS_OBJS) -o $@

$(BUILD_DIR)/$(TEST_TARGET): $(OBJS) $(UTILS_OBJS) $(TESTS_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_CFLAGS) $(OBJS) $(UTILS_OBJS) $(TESTS_OBJS) -o $@

# background rules

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
