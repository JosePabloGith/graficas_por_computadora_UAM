# Makefile para el proyecto de Reflejo
# Diseñado para compilar el archivo fuente en src/ y generar el ejecutable en bin/

# 1. Configuración del Compilador
CC = g++
# -Isrc: busca encabezados locales si existen
# -I/usr/include/GL: parche de compatibilidad para #include "glut.h" en Ubuntu
CFLAGS = -Isrc -I/usr/include/GL -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
SRC = src/ejemploConSombras.cpp
TARGET = bin/ejemplo_reflejo

# 4. Reglas de Construcción
all: directory $(TARGET)

# Crear carpeta bin para alojar el ejecutable (ignora README.md)
directory:
	@mkdir -p bin

# Compilación (LDFLAGS va al final para asegurar el enlace en Linux)
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# 5. Comandos de Mantenimiento
.PHONY: clean run help

clean:
	@echo "Limpiando binarios..."
	rm -rf bin

run: all
	@echo "Ejecutando la aplicación de reflejo..."
	./$(TARGET)

help:
	@echo "Comandos disponibles:"
	@echo "  make       - Compila el proyecto"
	@echo "  make run   - Compila y ejecuta automáticamente"
	@echo "  make clean - Elimina la carpeta bin/"
