# CMistry - Chemistry Application in C
# Cross-platform Makefile

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I$(INCDIR)
LDFLAGS =

# Debug/Release modes
DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CFLAGS += -g -DDEBUG
else
    CFLAGS += -O2 -DNDEBUG
endif

# Directories
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Detect OS for platform-specific settings
ifeq ($(OS),Windows_NT)
    # Windows (MinGW)
    TARGET = $(BINDIR)/cmistry.exe
    RM = del /Q
    MKDIR = mkdir
    RMDIR = rmdir /S /Q
    SEP = \\
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        # macOS
        TARGET = $(BINDIR)/cmistry
    else
        # Linux and others
        TARGET = $(BINDIR)/cmistry
    endif
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
    SEP = /
endif

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
HEADERS = $(wildcard $(INCDIR)/*.h)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@$(MKDIR) $(OBJDIR) 2>/dev/null || true
	@$(MKDIR) $(BINDIR) 2>/dev/null || true

# Link the target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

# Compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	$(RM) $(OBJDIR)$(SEP)*.o 2>/dev/null || true
	$(RM) $(TARGET) 2>/dev/null || true

# Full clean (including directories)
distclean: clean
	$(RMDIR) $(OBJDIR) 2>/dev/null || true
	$(RMDIR) $(BINDIR) 2>/dev/null || true

# Install (Unix-like systems only)
install: $(TARGET)
ifeq ($(OS),Windows_NT)
	@echo "Install not supported on Windows. Copy $(TARGET) manually."
else
	install -d $(DESTDIR)/usr/local/bin
	install -m 755 $(TARGET) $(DESTDIR)/usr/local/bin/
endif

# Uninstall (Unix-like systems only)
uninstall:
ifeq ($(OS),Windows_NT)
	@echo "Uninstall not supported on Windows."
else
	$(RM) $(DESTDIR)/usr/local/bin/cmistry
endif

# Run the program
run: $(TARGET)
	./$(TARGET)

# Run with valgrind (memory check, Linux only)
memcheck: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Generate documentation placeholder
docs:
	@echo "Documentation generation not yet implemented"

# Show help
help:
	@echo "CMistry Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove object files and binary"
	@echo "  distclean  - Remove all generated files and directories"
	@echo "  install    - Install to /usr/local/bin (Unix only)"
	@echo "  uninstall  - Remove from /usr/local/bin (Unix only)"
	@echo "  run        - Build and run the program"
	@echo "  memcheck   - Run with valgrind (Linux only)"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  DEBUG=1    - Build with debug symbols (default)"
	@echo "  DEBUG=0    - Build optimized release version"
	@echo ""
	@echo "Examples:"
	@echo "  make              - Build debug version"
	@echo "  make DEBUG=0      - Build release version"
	@echo "  make clean all    - Rebuild from scratch"
	@echo "  make run          - Build and run"

.PHONY: all clean distclean install uninstall run memcheck docs help directories
