# Makefile para el proyecto Tercera Técnica (Estructura POO Multi-archivo)
# Compilación incremental para optimizar tiempos de construcción

# 1. Configuración del Compilador
CC = g++
# -Iinclude: Busca encabezados en tu carpeta local (Cubo.h, Motor.h, etc.)
# -I/usr/include/GL: Busca el glut.h nativo de Linux tras haber borrado el local
# -Wall: Muestra advertencias para mantener el código limpio
CFLAGS = -Iinclude -I/usr/include/GL -Wall -O2

# 2. Librerías de Enlace (OpenGL/GLUT para Linux)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Archivos y Rutas
# Encuentra automáticamente todos los archivos .cpp en src/
SOURCES = $(wildcard src/*.cpp)
# Convierte la lista de .cpp a .o para la carpeta obj/
OBJECTS = $(patsubst src/%.cpp, obj/%.o, $(SOURCES))
# Nombre del ejecutable final
TARGET = bin/tercera_tecnica

# 4. Reglas de Construcción
all: build_dirs $(TARGET)

# Crear carpetas temporales y de salida
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
	@echo "Ejecutando la aplicación..."
	./$(TARGET)

help:
	@echo "Comandos disponibles:"
	@echo "  make       - Compila los archivos modificados y genera el ejecutable"
	@echo "  make run   - Compila y lanza la ventana gráfica"
	@echo "  make clean - Elimina las carpetas obj/ y bin/"
