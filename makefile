CC=g++
CFLAGS=-c -g -O0 -std=c++11 -Wall
LDFLAGS= -pthread
SOURCES=jacobi-pthread.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=jacobi-pthread

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f core $(EXECUTABLE) $(OBJECTS)