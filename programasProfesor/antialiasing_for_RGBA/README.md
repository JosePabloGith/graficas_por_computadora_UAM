# 🖥️ Ejercicio: Antialiasing en RGBA (Suavizado de Bordes)

**Curso:** Gráficas por Computadora - UAM (Trimestre 26-I)
**Entorno:** Ubuntu 24 (Wayland/Hyprland) | C++ & OpenGL Funciones Fijas

![Demostración de Antialiasing](assets/captura_antialiasing.png)
*(Nota: Visualización de primitivas renderizadas usando técnicas de suavizado para eliminar el efecto de bordes dentados).*

## 📖 Descripción del Código
Este programa demuestra las técnicas de mejora visual (Antialiasing) en un entorno de color RGBA. Suaviza los bordes dentados (efecto escalera) de las primitivas geométricas (líneas) mediante la mezcla de colores con el fondo usando el canal Alfa (Transparencia).

## 🛠️ Técnicas de Graficación Empleadas
* **Blending (Mezcla):** Uso de `glEnable(GL_BLEND)` y `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)`. (Abonando al **Objetivo 9: Reflexión/Blending** del proyecto).
* **Point/Line Smooth:** Uso de `GL_LINE_SMOOTH` para indicar a la máquina de estados que deseamos primitivas con bordes difuminados.
* **Máquina de Estados de UI:** Intercambio interactivo mediante el teclado (Tecla `R`) para rotar la matriz, lo que permite apreciar el antialiasing en diferentes ángulos. (Abonando al **Objetivo 10: Controles**).

## ⚙️ Compilación y Ejecución
Para compilar en Linux, abre la terminal en el directorio raíz y ejecuta:

```bash
make clean
make run
