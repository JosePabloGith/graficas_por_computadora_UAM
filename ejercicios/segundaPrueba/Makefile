# Makefile para el proyecto Segunda Prueba
# Diseñado para compilar src/main.cpp y alojar el binario en bin/

# 1. Configuración del Compilador
CC = g++
# -Isrc: permite incluir encabezados locales si se agregan después
# -I/usr/include/GL: parche de compatibilidad para OpenGL/GLUT en Ubuntu
CFLAGS = -Isrc -I/usr/include/GL -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
SRC = src/main.cpp
TARGET = bin/segunda_prueba

# 4. Reglas de Construcción
all: directory $(TARGET)

# Crear carpeta bin para alojar el ejecutable
directory:
	@mkdir -p bin

# Compilación (LDFLAGS al final es requerido en entornos Linux/Ubuntu)
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# 5. Comandos de Mantenimiento
.PHONY: clean run help

clean:
	@echo "Limpiando binarios..."
	rm -rf bin

run: all
	@echo "Ejecutando la segunda prueba..."
	./$(TARGET)

help:
	@echo "Comandos disponibles:"
	@echo "  make       - Compila el proyecto"
	@echo "  make run   - Compila y ejecuta la aplicación"
	@echo "  make clean - Elimina la carpeta bin/ generada"
