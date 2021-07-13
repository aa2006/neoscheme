CC ?= gcc
CFLAGS = 					\
	-std=gnu2x				\
	-Werror					\
	-Wall					\
	-Wextra					\
	-ggdb 					\
	-MD						\
	-fsanitize=address 		\
    -fsanitize=undefined	\
	-Isrc/					\

LDFLAGS += 					\
    -fsanitize=address 		\
    -fsanitize=undefined 	\
	-lreadline				\

DIRECTORY_GUARD=@mkdir -p $(@D)

TARGET = neoscheme 
SRCS   = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS   = $(patsubst %.c, $(BUILD_DIRECTORY)/%.c.o, $(SRCS))

BUILD_DIRECTORY ?= build

$(BUILD_DIRECTORY)/%.c.o: %.c
	$(DIRECTORY_GUARD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(DIRECTORY_GUARD)
	$(CC) $^ $(LDFLAGS) -o $@

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -r build
	-rm $(TARGET)