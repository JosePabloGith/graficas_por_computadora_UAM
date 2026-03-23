# Makefile para el proyecto Semana 1 Apunte 3
# Diseñado para compilar src/main.cpp e ignorar recursos estáticos (imágenes) en la raíz

# 1. Configuración del Compilador
CC = g++
# -Isrc: permite incluir encabezados locales
# -I/usr/include/GL: parche de compatibilidad para OpenGL/GLUT en Ubuntu
CFLAGS = -Isrc -I/usr/include/GL -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
SRC = src/main.cpp
TARGET = bin/apunte3

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
	@echo "Ejecutando el apunte 3..."
	./$(TARGET)

help:
	@echo "Comandos disponibles:"
	@echo "  make       - Compila el proyecto"
	@echo "  make run   - Compila y ejecuta la aplicación"
	@echo "  make clean - Elimina la carpeta bin/ generada"
