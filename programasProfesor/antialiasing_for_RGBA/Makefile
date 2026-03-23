# Makefile Universal para Proyectos UAM 
# Proyecto: Antialiasing for RGBA

CC = g++
CFLAGS = -Iinclude -Isrc -I/usr/include/GL -w -O2
LDFLAGS = -lGL -lGLU -lglut -lGLEW -lm

SOURCES = $(wildcard src/*.cpp)
TARGET = bin/ejecutable_antialiasing

all: directories $(TARGET)

directories:
	@mkdir -p bin include src assets

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

.PHONY: clean run

clean:
	@echo "Limpiando binarios..."
	rm -rf bin/*

run: all
	@echo "Ejecutando proyecto..."
	./$(TARGET)
