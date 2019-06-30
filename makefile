IDIR=include
CC=g++
CFLAGS=-I$(IDIR) -std=c++11
SDIR=src
ODIR=obj
LDIR=lib
BDIR=bin

LIBS=-lm

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = calc1c.o  
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


install_montador: $(ODIR)/montador.o
	$(CC) $(ODIR)/montador.o -o $(BDIR)/montador


install_program: $(ODIR)/TPSB.o
	$(CC) $(ODIR)/TPSB.o -o $(BDIR)/TPSB

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

install: install_montador install_program

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
