# Makefile generado por Gema 1 & Pablo para Ubuntu 24.04 (Basado en Mecklenburg)
# Proyecto: Sombras, Reflejo y Stencil en OpenGL/GLUT

# Compilador
CC = g++

# Banderas de compilación
# -I. para buscar encabezados en la carpeta actual
CFLAGS = -I. -Wall -O2

# Librerías de enlace (Vital para Linux)
# -lGL: OpenGL
# -lGLU: Utilidades OpenGL
# -lglut: Interfaz GLUT
# -lm: Librería matemática para sin, cos, sqrt
LDFLAGS = -lGL -lGLU -lglut -lm

# Archivos
SRC = src/main.cpp
EXEC = bin/proyecto

# Regla principal
all: directory $(EXEC)

# Crear estructura de carpetas si no existe
directory:
	@mkdir -p bin

# Compilar el ejecutable
$(EXEC): $(SRC)
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) $(LDFLAGS)

# Limpiar archivos generados
clean:
	rm -rf bin

# Ejecutar el programa
run: all
	./$(EXEC)

# Ayuda
help:
	@echo "Opciones del Makefile:"
	@echo "  make       - Compila el proyecto"
	@echo "  make run   - Compila y ejecuta inmediatamente"
	@echo "  make clean - Borra el ejecutable generado"
