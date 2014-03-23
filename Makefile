all: main.cpp drawGL

SOURCES    = main.cpp
OBJECTS    = $(SOURCES:.cpp=.o)

.cpp.o:
	g++ -c -Wall $< -o $@

drawGL: main.o
	g++ $(OBJECTS) -lGL -lGLU -lglut $(LDFLAGS) -o $@

clean:
	rm -f *.o

