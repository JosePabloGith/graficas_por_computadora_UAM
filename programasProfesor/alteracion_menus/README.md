### 🖱️ Ejercicio: Alteración de Menús Contextuales
**Archivo:** `src/main.cpp` (o `src/ejercicio_menus.cpp`)

Este programa demuestra el uso avanzado de la interacción humano-computadora mediante las bibliotecas de GLUT, cumpliendo con las bases del **Objetivo 10 (Controles)**. 

**Características Técnicas:**
* Máquina de estados para intercambiar interfaces gráficas.
* Creación y asignación dinámica de menús pop-up mediante `glutCreateMenu` y `glutAttachMenu`.
* Intercepción de eventos de hardware mediante `glutMouseFunc` (uso del botón central y derecho).
* Manejo de redimensionamiento dinámico y proyecciones ortogonales mediante `gluOrtho2D`.

![Demostración de Menús](assets/captura_menus.png)
*(Nota: Ejecuta el programa, abre el menú con click derecho y tómale una captura de pantalla para guardarla en la carpeta `assets/` como `captura_menus.png`)*

### 💡 Notas del Director Gema 1 para Ubuntu 24:
1. En tu archivo hay una referencia a `#include <GL/glew.h>`. En Ubuntu, si el compilador (Makefile) te da un error diciendo que no encuentra `glew.h`, puedes instalarlo en la terminal con:
   `sudo apt-get install libglew-dev`
2. Si prefieres no usar `glew` (ya que este código no lo necesita estrictamente), puedes simplemente borrar esa línea. Tu `Makefile` actual compilará esto a la perfección con `make`.


