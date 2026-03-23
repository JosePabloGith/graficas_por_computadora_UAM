# Makefile actualizado para estructura con carpeta src/
# Proyecto: eje_bitmaps (Archivo main.cpp dentro de src/)

# 1. Configuración del Compilador
CC = g++
# -Isrc e -Iinclude para encontrar encabezados en esas carpetas
CFLAGS = -Isrc -Iinclude -I. -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT para Linux)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
# Según tu 'ls -R', el archivo está en src/main.cpp
SRC = src/main.cpp
# El ejecutable final se guardará en bin/
TARGET = bin/bitmaps_app

# 4. Reglas de Construcción

# Regla principal: Crea la carpeta bin y compila
all: directory $(TARGET)

# Crear la carpeta bin si no existe
directory:
	@mkdir -p bin

# Compilación directa de .cpp a ejecutable
# Es fundamental que LDFLAGS vaya al final en sistemas basados en Ubuntu
$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# 5. Comandos de Mantenimiento
.PHONY: clean run help

# Borra la carpeta del ejecutable
clean:
	@echo "Borrando ejecutable y carpeta bin..."
	rm -rf bin

# Compila y ejecuta
run: all
	@echo "Lanzando aplicación..."
	./$(TARGET)

help:
	@echo "Uso rápido:"
	@echo "  make     -> Compila el archivo src/main.cpp en bin/"
	@echo "  make run -> Compila y abre la ventana de OpenGL"
	@echo "  make clean -> Elimina el contenido de la carpeta bin/"
