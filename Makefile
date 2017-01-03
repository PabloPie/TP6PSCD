#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   noviembre 2015
# Coms:   Compilar mediante "make"
#*****************************************************************

CC = g++
RM=/bin/rm

FLAGS=-std=c++11 -fmax-errors=1 -Werror -I. -O2 
LIBS=-pthread

# Create the executable
all: main

main: main.o LatLong-UTMconversion.o
	${CC} $(FLAGS) main.o LatLong-UTMconversion.o -o main ${LIBS}

main.o: main.cpp
	${CC} $(FLAGS) -c main.cpp  ${LIBS}

LatLong-UTMconversion.o: LatLong-UTMconversion.cpp LatLong-UTMconversion.h
	${CC} $(FLAGS) -c LatLong-UTMconversion.cpp  ${LIBS}
clean:
	$(RM) -f main *.o


