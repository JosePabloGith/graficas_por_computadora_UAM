# Makefile Universal para Pruebas Rápidas (Código Legado / Windows a Linux)
# Diseñado para compilar cualquier código espagueti depositado en src/main.cpp

# 1. Configuración del Compilador
CC = g++
# -Isrc e -Iinclude: Preparado por si el código trae archivos extra
# -I/usr/include/GL: EL PARCHE MAESTRO. Permite que el #include "glut.h" de Windows funcione en Ubuntu.
# -w: Apaga los warnings (advertencias).
CFLAGS = -Iinclude -Isrc -I/usr/include/GL -w -O2

# 2. Librerías de Enlace (Nativas de Linux)
# ---> CIRUGÍA GEMA 1: Se agregó -lGLEW para soportar #include <GL/glew.h>
LDFLAGS = -lGL -lGLU -lglut -lGLEW -lm

# 3. Archivos y Rutas
SOURCES = $(wildcard src/*.cpp)
TARGET = bin/prueba_rapida

# 4. Reglas de Construcción
all: directories $(TARGET)

directories:
	@mkdir -p bin include src

# Compilación directa
$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# 5. Comandos de Mantenimiento
.PHONY: clean run help

clean:
	@echo "Limpiando el molde..."
	rm -rf bin/*

run: all
	@echo "Ejecutando prueba rápida..."
	./$(TARGET)

help:
	@echo "Uso del Molde:"
	@echo "  1. Pega el código de Windows en src/main.cpp"
	@echo "  2. Ejecuta 'make run'"
	@echo "  3. Ejecuta 'make clean' para la siguiente prueba"
