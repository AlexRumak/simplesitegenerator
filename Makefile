BUILD_DIR = ./out

INCLUDE_DIRS = -I./src -I./src/parser -I./src/tests
LIB_DIRS =
CC=gcc

CDEFS=
CFLAGS= -O3 -g $(INCLUDE_DIRS) $(CDEFS)
LIBS= -lpthread

PRODUCT=$(BUILD_DIR)/simplesitegenerator

HFILES= parser.h
CFILES= src/simplesitegenerator.c src/parser/parser.c
TESTFILES= src/tests/test_parser.c

TEST_RESOURCES_SRC = src/tests/resources

OBJS = $(patsubst src/%, $(BUILD_DIR)/%, $(CFILES:.c=.o))
TEST_OBJS = $(patsubst src/%, $(BUILD_DIR)/%, $(TESTFILES:.c=.o))

# Make sure that the build directory exists
$(shell mkdir -p $(BUILD_DIR))

all:	${PRODUCT}

clean:
	-rm -rf $(BUILD_DIR)

$(PRODUCT): $(OBJS)
			$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

depend:

# ======= TESTS =======
test: $(TEST_OBJS) $(BUILD_DIR)/parser/parser.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/test_parser $(TEST_OBJS) $(BUILD_DIR)/parser/parser.o $(LIBS)
	$(BUILD_DIR)/test_parser $(TEST_RESOURCES_SRC)

$(BUILD_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -MD $(CFLAGS) -c $< -o $@
