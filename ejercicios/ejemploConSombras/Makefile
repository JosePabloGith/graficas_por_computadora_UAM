# Makefile para el proyecto de Sombras (Parche de compatibilidad Windows -> Linux)
# Proyecto: ejemploConSombras

# 1. Configuración del Compilador
CC = g++
# -Isrc: busca en la carpeta fuente
# -I/usr/include/GL: PARCHE CLAVE. Permite que #include "glut.h" funcione en Ubuntu 
# sin tener que modificar el .cpp a #include <GL/glut.h>
CFLAGS = -Isrc -I/usr/include/GL -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT para Linux)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
SRC = src/ejemploConSombras.cpp
TARGET = bin/ejemplo_sombras

# 4. Reglas de Construcción
all: directory $(TARGET)

directory:
	@mkdir -p bin

# Compilación
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# 5. Comandos de Mantenimiento
.PHONY: clean run help

clean:
	@echo "Limpiando binarios..."
	rm -rf bin

run: all
	@echo "Iniciando ejemplo de sombras..."
	./$(TARGET)

help:
	@echo "Instrucciones de uso:"
	@echo "  make      - Compila el archivo ejemploConSombras.cpp"
	@echo "  make run  - Compila y ejecuta la aplicación de sombras"
	@echo "  make clean - Elimina los archivos generados"
