import numpy as np
import matplotlib.pyplot as plt

# 1. Leer los datos del archivo generado por C++
# loadtxt automáticamente separa las columnas guiándose por los espacios/tabuladores
datos = np.loadtxt("magnetizacion_data.dat")

temperaturas = datos[:, 0]     # La primera columna es la Temperatura
magnetizaciones = datos[:, 1]  # La segunda columna es la Magnetización

# 2. Crear la gráfica
plt.figure(figsize=(8, 6))

# Dibujamos los puntos y la línea que los une
plt.plot(temperaturas, magnetizaciones, marker='o', color='blue', linestyle='-')

# Línea vertical roja para marcar visualmente dónde ocurre la transición de fase (T_c)
plt.axvline(x=2.27, color='red', linestyle='--', label='Temperatura Crítica (~2.27)')

# Decoración del gráfico
plt.title('Transición de Fase: Magnetización vs Temperatura', fontsize=14)
plt.xlabel('Temperatura (T)', fontsize=12)
plt.ylabel('Magnetización Media |m|', fontsize=12)
plt.grid(True, linestyle=':', alpha=0.7)
plt.legend()

# Mostrar la obra de arte
plt.show()