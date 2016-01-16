TARGET = wrecker
SRC_DIR = src
OBJ_DIR = obj

LDFLAGS = -lBearLibTerminal
CFLAGS = -std=c99 -g -Wall

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
_OBJS = $(SOURCES:$(SRC_DIR)/%.c=%.o)
OBJECTS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJ_DIR) $(OBJECTS)
	gcc $(CFLAGS) -o $@ $(OBJECTS) -L. $(LDFLAGS) -lm

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET)
	rm -rf $(OBJ_DIR)

