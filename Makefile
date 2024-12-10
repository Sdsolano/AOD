CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS =

# Directorios
SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = tests

# Archivos fuente y objetos
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/distanceEdition

# Crear directorios necesarios
$(shell mkdir -p $(OBJDIR) $(BINDIR))

.PHONY: all clean test valgrind

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	cd $(TESTDIR) && $(MAKE) -f Makefile-test all

valgrind: $(TARGET)
	cd $(TESTDIR) && $(MAKE) -f Makefile-test all-valgrind

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf $(OBJDIR) $(BINDIR)
	cd $(TESTDIR) && $(MAKE) -f Makefile-test clean