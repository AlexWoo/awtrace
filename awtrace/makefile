#
# makefile for awtrace
#
# Author: AlexWoo <wj19840501@gmail.com>
#

CPPCOMPILER = g++
CCOMPILER = gcc

OBJEXE = awtrace.o  funcrec.o  funcstack.o
OBJLIB = instrument.o

EXE = awtrace
LIB = libawtrace.so

release: $(LIB) $(EXE)

$(EXE): $(OBJEXE)
	$(CPPCOMPILER) -o $@ $(OBJEXE)

$(LIB): $(OBJLIB)
	$(CCOMPILER) -shared -lpthread -o $@ $(OBJLIB)

.c.o:
	$(CCOMPILER) $(CFLAGS) -Wall -c -fPIC -lpthread $<

.C.o:
	$(CPPCOMPILER) $(CFLAGS) -Wall -c $<

all:
	make clean
	make release

install:
	make clean
	make release
	cp $(LIB) /usr/lib
	cp $(EXE) /usr/bin

clean:
	rm -f $(EXE) $(LIB) *.o
