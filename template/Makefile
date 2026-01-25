# Compile without any optimizations, but with maximum verbosity of debug
# symbols

CC = gcc
CXX = g++
RM = rm -rf

CFLAGS = -std=gnu17 -Wall -Wextra -Og -g3 -lm -MMD
CXXFLAGS = -std=gnu++17 -Wall -Wextra -Og -g3 -lm -lstdc++ -MMD -MF $(patsubst %.oxx,%.dxx,$@)

SRC_DIR = src
TEST_DIR = tests
DIST_DIR = target

MAIN = main

OBJ_DIR = $(DIST_DIR)/obj
TEST_OBJ_DIR = $(DIST_DIR)/obj/test


# no need to change rules below this line
C_SRCS = $(wildcard $(SRC_DIR)/*.c)
C_TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
CXX_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
CXX_TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)

C_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))
C_TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(TEST_OBJ_DIR)/%.o,$(C_TEST_SRCS))
CXX_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.oxx,$(CXX_SRCS))
CXX_TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.oxx,$(CXX_TEST_SRCS))

C_LIB_OBJS = $(filter-out $(OBJ_DIR)/$(MAIN).o,$(C_OBJS))
CXX_LIB_OBJS = $(filter-out $(OBJ_DIR)/$(MAIN).oxx,$(CXX_OBJS))

TEST_TARGETS = $(patsubst $(TEST_DIR)/%.c,$(DIST_DIR)/%.test,$(C_TEST_SRCS)) \
	       $(patsubst $(TEST_DIR)/%.cpp,$(DIST_DIR)/%.test.xx,$(CXX_TEST_SRCS))

default: $(DIST_DIR)/main

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.oxx: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(TEST_OBJ_DIR)/%.oxx: $(TEST_DIR)/%.cpp | $(TEST_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIST_DIR)/%.test: $(TEST_OBJ_DIR)/%.o $(C_LIB_OBJS) | $(DIST_DIR)
	$(CC) $(CFLAGS) $^ -o $@
$(DIST_DIR)/%.test.xx: $(TEST_OBJ_DIR)/%.oxx $(C_LIB_OBJS) $(CXX_LIB_OBJS) | $(DIST_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(DIST_DIR)/main: $(C_OBJS) $(CXX_OBJS) | $(DIST_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(DIST_DIR) $(OBJ_DIR) $(TEST_OBJ_DIR):
	mkdir -p $@

tests: $(TEST_TARGETS)

all: $(DIST_DIR)/main $(TEST_TARGETS)

clean:
	$(RM) $(DIST_DIR) docs

docs:
	doxygen

help:
	@echo "Available targets:"
	@echo "  make        - Build main executable"
	@echo "  make tests  - Build test suite"
	@echo "  make all    - Build main + tests"
	@echo "  make clean  - Remove build artifacts"
	@echo "  make docs   - Generate documentation"


.SECONDARY: $(C_OBJS) $(C_TEST_OBJS) $(CXX_OBJS) $(CXX_TEST_OBJS)
-include $(C_OBJS:.o=.d)
-include $(C_TEST_OBJS:.o=.d)
-include $(CXX_OBJS:.oxx=.dxx)
-include $(CXX_TEST_OBJS:.oxx=.dxx)

.PHONY: clean docs default all tests help
