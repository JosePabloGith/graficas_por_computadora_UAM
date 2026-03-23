# Makefile para el experimento: Triángulo Simple
# Diseñado para compilar src/ejercicio_trianguloBase.cpp y alojar el binario en bin/

# 1. Configuración del Compilador
CC = g++
# -Isrc: permite incluir encabezados locales si se agregan en el futuro
# -I/usr/include/GL: parche de compatibilidad para OpenGL/GLUT en Ubuntu
CFLAGS = -Isrc -I/usr/include/GL -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
SRC = src/ejercicio_trianguloBase.cpp
TARGET = bin/triangulo_simple

# 4. Reglas de Construcción
all: directory $(TARGET)

# Crear carpeta bin para mantener la raíz limpia
directory:
	@mkdir -p bin

# Compilación (LDFLAGS al final es requerido en entornos Linux/Ubuntu)
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# 5. Comandos de Mantenimiento
.PHONY: clean run help

clean:
	@echo "Limpiando binarios del experimento..."
	rm -rf bin

run: all
	@echo "Ejecutando el experimento del triángulo..."
	./$(TARGET)

help:
	@echo "Comandos disponibles:"
	@echo "  make       - Compila el proyecto"
	@echo "  make run   - Compila y ejecuta la aplicación"
	@echo "  make clean - Elimina la carpeta bin/ generada"
