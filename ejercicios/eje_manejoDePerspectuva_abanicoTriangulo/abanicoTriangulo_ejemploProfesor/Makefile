# Makefile para el ejemplo del profesor (Estructura de un solo archivo)
# Proyecto: abanicoTriangulo_ejemploProfesor

# 1. Configuración del Compilador
CC = g++
# -Isrc: permite buscar encabezados si el profesor los incluyó en la misma carpeta
CFLAGS = -Isrc -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
# Según tu ls -R, el archivo está en src/main.cpp
SRC = src/main.cpp
# El ejecutable se guardará en bin/
TARGET = bin/abanico_profe

# 4. Reglas de Construcción

# Regla principal
all: directory $(TARGET)

# Crear carpeta bin si no existe
directory:
	@mkdir -p bin

# Compilación directa: de .cpp a ejecutable
# En Linux, las librerías (LDFLAGS) deben ir al final del comando
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# 5. Comandos de Mantenimiento
.PHONY: clean run help

# Borra la carpeta bin
clean:
	@echo "Borrando ejecutable..."
	rm -rf bin

# Compila y lanza la aplicación
run: all
	@echo "Ejecutando ejemplo del profesor..."
	./$(TARGET)

help:
	@echo "Comandos:"
	@echo "  make     - Compila src/main.cpp"
	@echo "  make run  - Compila y ejecuta"
