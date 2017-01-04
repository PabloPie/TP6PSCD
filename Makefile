#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   noviembre 2015
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
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:   
	$(RM) -r $(BUILDDIR) $(TARGET); $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
