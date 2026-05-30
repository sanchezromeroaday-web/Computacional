import numpy as np
import matplotlib.pyplot as plt
import os

# Definimos las temperaturas usadas en C++ y sus colores
temperaturas = [1.0, 1.8, 2.3, 3.5]
colores = ['blue', 'orange', 'green', 'red']

# Crear la figura
plt.figure(figsize=(8, 6))

for T, color in zip(temperaturas, colores):
    # to_string(float) en C++ suele generar 6 decimales (ej: 1.000000)
    filename = f"perfil_densidad_T{T:.6f}.dat"
    
    # Alternativa por si el compilador cortó los ceros (ej: 1.0)
    if not os.path.exists(filename):
        filename = f"perfil_densidad_T{T}.dat"
        
    if os.path.exists(filename):
        # Leer los datos ignorando la cabecera
        y, densidad = np.loadtxt(filename, skiprows=1, unpack=True)
        
        # Dibujar la curva suave
        plt.plot(y, densidad, '-', color=color, linewidth=2, label=f'T = {T}')
    else:
        print(f"⚠️ Aviso: No se encontró el archivo para T={T} (Buscado: {filename})")

# Formato estético y académico de la gráfica
plt.title("Perfil de Densidad del Gas de Red frente a la Altura", fontsize=14, pad=15)
plt.xlabel("Posición vertical ($y$)", fontsize=12)
plt.ylabel(r"Densidad media de partículas ($\rho$)", fontsize=12)

# Añadimos una línea horizontal en densidad = 0.5 como referencia visual
plt.axhline(y=0.5, color='gray', linestyle='--', alpha=0.7, label='Densidad homogénea (0.5)')

# Ajustamos los límites de los ejes (N=128)
plt.xlim(0, 127) 
plt.ylim(-0.05, 1.05)

plt.legend(fontsize=11, loc='upper right')
plt.grid(True, linestyle=':', alpha=0.7)
plt.tight_layout()

# Guardar la imagen en alta calidad y mostrarla
plt.savefig("perfiles_densidad_T_comparacion.png", dpi=300)
plt.show()