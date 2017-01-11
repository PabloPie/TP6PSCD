#*****************************************************************
# File:   Makefile
# Author: Pablo Piedrafita / Hilton Lipschitz
# Date:   Enero 2017
# Coms:   Compilar mediante "make"
#*****************************************************************

SRCDIR := src
BUILDDIR := build
DATADIR := datos
TARGETS := bin/Servidor
TARGETC := bin/Cliente
CC := g++

SRCEXT := cpp
SOURCES := $(filter-out src/Cliente.cpp, $(shell find $(SRCDIR) -type f -name *.$(SRCEXT)))
SOURCESC := src/Cliente.cpp src/Socket.cpp
OBJECTS := $(filter-out src/Cliente.o, $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o)))
OBJECTSC := build/Cliente.o build/Socket.o

CFLAGS :=-std=c++11 -fmax-errors=1  -I. -O2 -I/usr/local/include -lsockets #-Werror
LIB :=-pthread -lcurl
HLIB := -L/usr/local/lib -lm -pthread -lcurl -lsocket -lnsl

all: server client

server: $(TARGETS)
client: $(TARGETC)

$(TARGETC): $(OBJECTSC)
	@echo " Linking client..."
	@echo " $(CC) $^ -o $(TARGETC) $(LIB)";$(CC) $^ -o $(TARGETC) $(LIB) #$(HLIB) descomentar para Hendrix


# Create the executable
$(TARGETS): $(OBJECTS)
	@echo " Linking server..."
	@echo " $(CC) $^ -o $(TARGETS) $(LIB)";$(CC) $^ -o $(TARGETS) $(LIB) #$(HLIB) descomentar para Hendrix

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(DATADIR)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) -c -o $@ $<";$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGETS) $(TARGETC)";rm -r $(BUILDDIR) $(TARGETS) $(TARGETC)

.PHONY: clean
