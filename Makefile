#*****************************************************************
# File:   Makefile
# Author: Pablo Piedrafita / Hilton Lipschitz
# Date:   Enero 2017
# Coms:   Compilar mediante "make"
#*****************************************************************

SRCDIR := src
BUILDDIR := build
TARGET := bin/main

CC := g++
RM :=/bin/rm

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS :=-std=c++11 -fmax-errors=1 -Werror -I. -O2 
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
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)";$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
