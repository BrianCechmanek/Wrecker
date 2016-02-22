TARGET = wrecker
SRC_DIR = src
OBJ_DIR = obj

LDFLAGS = -lBearLibTerminal
CFLAGS = -std=gnu99 -g -Wall -Iinclude

SOURCES = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
_OBJS = $(SOURCES:$(SRC_DIR)/%.c=%.o)
OBJECTS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))

# test targets
TEST_KRNG = test_krng
TEST_DAMAGE = test_damage

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
	rm -f $(TEST_KRNG) $(TEST_DAMAGE)

.PHONY: $(TEST_KRNG)
$(TEST_KRNG): $(OBJ_DIR)
	gcc $(CFLAGS) -DTEST_KRNG -c src/krng.c -o obj/krng.o
	gcc $(CFLAGS) -o $@ obj/krng.o

.PHONY: $(TEST_DAMAGE)
$(TEST_DAMAGE): $(OBJ_DIR)
	gcc $(CFLAGS) -DTEST_DAMAGE -c src/test/test_damage.c -o obj/test_damage.o
	gcc $(CFLAGS) -c src/diana.c -o obj/diana.o
	gcc $(CFLAGS) -c src/health.c -o obj/health.o
	gcc $(CFLAGS) -c src/resistance.c -o obj/resistance.o
	gcc $(CFLAGS) -o $@ obj/test_damage.o obj/diana.o obj/health.o obj/resistance.o
