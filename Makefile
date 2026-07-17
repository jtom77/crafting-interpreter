CC = cc
CFLAGS = -Wall -Wextra
SRCS = $(wildcard *.c)
TARGET = clox

# make            # optimized build -> ./clox
# make debug      # -g -O0 build for gdb
# make gdb FILE=test.lox   # debug-build then launch gdb with the script as argv[1]
# make clean
# Default: optimized build
all: CFLAGS += -O2
all: $(TARGET)

# Debug build: symbols, no optimization
debug: CFLAGS += -g -O0
debug: $(TARGET)

# FORCE makes the link always run: all/debug share one output ($(TARGET))
# but use different flags, and make can't tell them apart by timestamp alone.
$(TARGET): $(SRCS) FORCE
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

FORCE:

# Build with debug symbols and launch gdb; pass a script with FILE=path/to.lox
gdb: debug
	gdb --args ./$(TARGET) $(FILE)

clean:
	rm -f $(TARGET)

.PHONY: all debug gdb clean FORCE
