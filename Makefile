C++        = g++
CPPFLAGS = -O3

OBJECTS =     Generator.o \
              FileIO.o \
              Person.o \
              Graph.o \
	      Main.o 

.PHONY : clean delete

all:    $(OBJECTS)
	$(C++) -o schedule $(CPPFLAGS) $(OBJECTS)

$(OBJECTS) :
	$(C++) -c -o$(@) $(CPPFLAGS) $(<)


Generator.o       : Generator.C Generator.H
FileIO.o       	  : FileIO.C FileIO.H
Person.o          : Person.C Person.H
Graph.o           : Graph.C Graph.H
Main.o            : Main.C 

clean :
	rm -f *.o

