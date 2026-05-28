# Física Computacional - UGR 

Repositorio de proyectos y simulaciones numéricas desarrollados para la asignatura de Física Computacional del Grado en Física de la Universidad de Granada (UGR). 

Este repositorio se centra en la resolución de sistemas físicos complejos y el análisis de sus dinámicas mediante métodos numéricos, separando el motor de cálculo pesado de la visualización de datos.

## Stack Tecnológico y Herramientas

* **C++:** Motor principal de cálculo. Empleado para ejecutar las integraciones numéricas (como métodos de Runge-Kutta) y algoritmos de simulación para garantizar el máximo rendimiento y eficiencia computacional.
* **Python:** Análisis de datos y visualización gráfica. Utilizado para procesar los *outputs* de C++, generar gráficas y crear animaciones de los sistemas físicos.
* **Google Colab:** Entorno principal para la ejecución y presentación de los cuadernos de Jupyter (Jupyter Notebooks), combinando código, texto explicativo y visualizaciones interactivas.
* **Asistencia IA:**
    * **Google Gemini:** Modelo de lenguaje multimodal (LLM) utilizado como apoyo para estructurar las justificaciones matemáticas, analizar resultados teóricos y redactar la documentación de los cuadernos.
    * **GitHub Copilot:** Asistente de programación basado en IA utilizado para agilizar la escritura de código y el autocompletado de sintaxis.

##  Estructura del Repositorio

El código está organizado en dos directorios principales que agrupan los ejercicios del curso:

### 1. Problemas Obligatorios (`/Obligatorios`)
Contiene la implementación y resolución de los 4 problemas fundamentales exigidos en la asignatura.
* *Problema 1: Algoritmo de Verlet - Sistema Solar*
* *Problema 2: Algoritmo de Metropolis Modelo de Ising*
* *Problema 3: Algoritmo de Crank-Nicholson - eq de Schroedinger*
* *Problema 4: Algoritmo de Runge Kutta Problema de los tres cuerpos*

### 2. Problemas Voluntarios (`/Voluntarios`)
Contiene 3 proyectos de ampliación y simulación avanzada (ej. misiones espaciales, interacciones gravitatorias complejas, etc.).
* *Voluntario 1: Formación de las galaxias a partir de sistemas solares*
* *Voluntario 2: ??*

##  Flujo de Trabajo (Cómo usar este repositorio)

1.  **Cálculo:** Los archivos fuente `.cpp` contienen la lógica numérica. Al compilarse y ejecutarse, generan archivos de texto plano (`.txt` ,`.csv`, `.dat`) con los datos temporales del sistema.
2.  **Visualización:** Los cuadernos o scripts de Python leen estos archivos de datos para renderizar las trayectorias y calcular constantes de movimiento.
3.  **Informes**Algunos problemas , principalmente los voluntarios contienen un notebook con toda la información y analisis de las simulaciones

---
**Autor:** Aday | Grado en Física
