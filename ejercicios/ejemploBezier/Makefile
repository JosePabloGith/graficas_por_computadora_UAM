# Makefile para contruir el ejemplo usando la tecnica bezier
# objetivo que cumplira este ejemplo
# renderizar una superficiee curva (sabana bezier) iluminada
# compilador
CC = g++

# banderas de compilacion
# -I para buscar encabezados en la carpeta actual donde esta este makefile
CFLAGS = -I. -Wall -O2

# Librerias de enlace virtual para linux
# - IGL:OpenGl
# - IGLU: utilidades OpenGl
# - lglut: interfaz GLUT
# - lm: libreria matematica para seno, cosenos etc

LDFLAGS = -lGL -lGLU -lglut -lm

# Archivos
SRC = src/main.cpp
EXEC = bin/bezmesh_ejemplo

#regla principal
all: directory $(EXEC)

# creamos la estructura de carpetas en caso de que no exista
directory:
	@mkdir -p bin
# compilar el ejecutable
$(EXEC): $(SRC)
	$(CC) $(SRC) -o $(EXEC) $(CFLAGS) $(LDFLAGS)

# limpiar archivos generados
clean:
	rm -rf bin

# ejecutar el programa
run: all
	./$(EXEC)

# ayuda
help:
	@echo "Opciones del makefile :D"
	@echo "make - compilara tu proyecto bro"
	@echo "make run -compilara y de golpe arancara tu proyecto"
	@echo "make clean - borrara el directorio creado /bin"
