TARGET = wrecker
SRC_DIR = src
OBJ_DIR = obj

LDFLAGS = -lBearLibTerminal
CFLAGS = -std=c99 -g -Wall

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
_OBJS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJECTS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJ_DIR) $(OBJECTS)
	gcc $(CFLAGS) -o $@ $(OBJECTS) -L. $(LDFLAGS) -lm

.PHONY: clean
clean:
	rm -f $(TARGET)
	rm -rf $(OBJ_DIR)

