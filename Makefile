BUILD_DIR = ./out

INCLUDE_DIRS = -I./src -I./src/parser -I./src/tests -I./src/tests/parser -I./src/tests/lib
LIB_DIRS =
CC=gcc

CDEFS=
CFLAGS= -O3 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lpthread

PRODUCT=$(BUILD_DIR)/simplesitegenerator

SRC_DIR = src
HFILES = $(shell find $(SRC_DIR) -type f -name "*.h")
CFILES = $(shell find $(SRC_DIR) -type f -name "*.c")

OBJS = $(patsubst src/%, $(BUILD_DIR)/%, $(CFILES:.c=.o))

# Make sure that the build directory exists
$(shell mkdir -p $(BUILD_DIR))

all:	${PRODUCT}

clean:
	-rm -rf $(BUILD_DIR)

$(PRODUCT): $(OBJS)
			$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

depend:

# ======= TESTS =======
TEST_RESOURCES_SRC = src/tests/resources

test: $(OBJS)
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/simplesitegenerator $(TEST_OBJS) $(OBJS) $(LIBS)
	$(BUILD_DIR)/simplesitegenerator -Mrun_test_suite

$(BUILD_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -MD $(CFLAGS) -c $< -o $@
