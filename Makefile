TARGET = test
BUILD_DIR = build
CC = gcc
CFLAGS = $(INCLUDES)
OFLAGS = -lm

SOURCES = \
test.c \
$(wildcard ./src/*.c)

INCLUDES =  \
-Iinclude/ \
-Isrc/

FILE_PATH = \
./ \
./src/

TEST_OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:.c=.o)))

vpath %.c $(FILE_PATH)

all: $(TARGET)

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) $< -o $@
	
$(TARGET) : $(TEST_OBJECTS)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) $(OFLAGS) -o $@ 

$(BUILD_DIR):
	mkdir $@

clean:
	rm -rf $(BUILD_DIR)
	rm $(TARGET)