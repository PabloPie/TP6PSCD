#*****************************************************************
# File:   Makefile
# Author: Pablo Piedrafita / Hilton Lipschitz
# Date:   Enero 2017
# Coms:   Compilar mediante "make"
#*****************************************************************

SRCDIR := src
BUILDDIR := build
TARGET := bin/Servidor

CC := g++

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS :=-std=c++11 -fmax-errors=1  -I. -O2 #-Werror
LIB :=-pthread

all: $(TARGET)
# Create the executable
$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)";$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<";$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)";rm -r $(BUILDDIR) $(TARGET)

.PHONY: clean
