TARGET = main
EXENAME = terrain
DEPS = main.o App.o Window.o Shader.o View.o Tile.o FPScounter.o Camera.o MapData.o GlobData.o Frame.o

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
OBJS = $(TARGET).o
LIBS = -lGLEW -lGL -lglfw

all: $(TARGET)

install: $(TARGET) clean

$(TARGET): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $(EXENAME) $^ $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o
distclean: clean
	rm -f $(EXENAME)

.PHONY: clean

