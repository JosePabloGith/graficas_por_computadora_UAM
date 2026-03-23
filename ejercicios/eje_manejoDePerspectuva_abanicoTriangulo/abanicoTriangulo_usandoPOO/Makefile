# Makefile Estándar para Proyecto de Gráficos (POO)
# Configurado para: Camara, Cono, MotorGrafico y Main

# 1. Parámetros del Compilador
CC = g++
# -Iinclude: Indispensable para que el compilador encuentre Camara.h, Cono.h, etc.
CFLAGS = -Iinclude -Wall -O2

# 2. Librerías del Sistema (OpenGL / GLUT)
LDFLAGS = -lGL -lGLU -lglut -lm

# 3. Estructura de Archivos
# Detecta automáticamente: Camara.cpp, Cono.cpp, Main.cpp, MotorGrafico.cpp
SOURCES = $(wildcard src/*.cpp)
# Genera los objetos correspondientes en la carpeta obj/
OBJECTS = $(patsubst src/%.cpp, obj/%.o, $(SOURCES))
# Nombre del ejecutable final
TARGET = bin/abanico_poo

# 4. Reglas de Construcción

# Regla principal
all: build_dirs $(TARGET)

# Crear carpetas de salida
build_dirs:
	@mkdir -p bin obj

# Enlace: Une todos los objetos (POO) en el ejecutable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compilación: Transforma cada .cpp de src/ en un .o en obj/
obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

# 5. Mantenimiento
.PHONY: clean run help

# Limpia objetos y el binario
clean:
	@echo "Limpiando archivos del proyecto..."
	rm -rf obj bin

# Compila y ejecuta la aplicación
run: all
	@echo "Iniciando: $(TARGET)"
	./$(TARGET)

help:
	@echo "Opciones para este proyecto POO:"
	@echo "  make       - Compila todos los módulos y el Main"
	@echo "  make run   - Compila y ejecuta la perspectiva del abanico"
	@echo "  make clean - Elimina la carpeta bin/ y obj/"
