# Makefile Estándar para Proyecto de Animación POO
# Compilación incremental para optimizar tiempos de construcción

# 1. Configuración del Compilador
CC = g++
# -Iinclude: Busca tus propios encabezados (Cuadrado.h, Motor.h, etc.)
# -I/usr/include/GL: Parche de compatibilidad para #include "glut.h" nativo en Ubuntu
CFLAGS = -Iinclude -I/usr/include/GL -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT nativas de Linux)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
# Encuentra automáticamente todos los archivos .cpp en src/
SOURCES = $(wildcard src/*.cpp)
# Convierte la lista de .cpp a .o para la carpeta temporal obj/
OBJECTS = $(patsubst src/%.cpp, obj/%.o, $(SOURCES))
# Nombre del ejecutable final
TARGET = bin/animacion_poo

# 4. Reglas de Construcción
all: build_dirs $(TARGET)

# Crear carpetas necesarias si no existen (incluyendo obj/ para los temporales)
build_dirs:
	@mkdir -p bin obj

# Enlace: Une todos los .o generados en el ejecutable final
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compilación Incremental: Convierte cada .cpp en un .o independiente
obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# 5. Comandos de Mantenimiento
.PHONY: clean run help

clean:
	@echo "Limpiando archivos objeto y binarios..."
	rm -rf obj bin

run: all
	@echo "Ejecutando la animación POO..."
	./$(TARGET)

help:
	@echo "Comandos disponibles:"
	@echo "  make       - Compila los archivos modificados y genera el ejecutable"
	@echo "  make run   - Compila y lanza la ventana gráfica"
	@echo "  make clean - Elimina las carpetas obj/ y bin/"
