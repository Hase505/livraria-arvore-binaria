CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -Iinclude
SRC = $(wildcard src/*.c)
MAIN = main.c

BUILD_DIR = build
BIN = $(BUILD_DIR)/programa

TEST_DIR = tests
TEST_MAIN = $(TEST_DIR)/test_run.c
TEST_MODULES = $(wildcard $(TEST_DIR)/test_*.c)
TEST_OBJS = $(filter-out $(TEST_MAIN), $(TEST_MODULES))
TEST_BIN = $(BUILD_DIR)/tests
TEST_LIBS = -lcmocka

.PHONY: all clean run test

all: $(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN): $(SRC) $(MAIN) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(TEST_BIN): $(SRC) $(TEST_MAIN) $(TEST_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@ $(TEST_LIBS)

test: $(TEST_BIN)
	./$(TEST_BIN)

run: all
	./$(BIN)

clean:
	rm -rf $(BUILD_DIR)
