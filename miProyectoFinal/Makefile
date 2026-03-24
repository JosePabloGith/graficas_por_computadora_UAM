# Makefile Jose Pablo Perez De Anda
CC = g++
CFLAGS = -I. -I./include -Wall -g
LDFLAGS = -lGL -lGLU -lglut -lassimp -lm

# El Makefile ahora busca automáticamente todos los .cpp dentro de src/
SRC = $(wildcard src/*.cpp)

# Convierte los nombres de src/archivo.cpp a obj/archivo.o
OBJ = $(patsubst src/%.cpp,obj/%.o,$(SRC))
EXEC = bin/practica9

all: directories $(EXEC)

directories:
	@mkdir -p obj bin

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Regla para compilar los .cpp de src/ hacia la carpeta obj/
obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf obj bin

run: all
	./$(EXEC)
