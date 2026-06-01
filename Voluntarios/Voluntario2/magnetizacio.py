import numpy as np
import matplotlib.pyplot as plt
import os

archivos = [
    ("magnetizacion_32.dat", "N = 32", "blue"),
    ("magnetizacion_64.dat", "N = 64", "orange"),
    ("magnetizacion_128.dat", "N = 128", "green")
]

plt.figure(figsize=(9, 6))

# 1. Curva Teórica (Onsager-Yang)
T_teo = np.linspace(0.5, 3.5, 1000)
T_c = 2.0 / np.log(1.0 + np.sqrt(2.0))
m_teo = np.zeros_like(T_teo)
mascara = T_teo < T_c
m_teo[mascara] = (1.0 - np.sinh(2.0 / T_teo[mascara])**(-4))**(1.0 / 8.0)
plt.plot(T_teo, m_teo, '--', color='black', linewidth=2, zorder=5, label='Solución exacta (Onsager)')

# 2. Datos Experimentales con Barras de Error
for archivo, etiqueta, color in archivos:
    if os.path.exists(archivo):
        # Desempaquetamos 3 columnas: T, media y el error calculado
        T, m_dom, error = np.loadtxt(archivo, skiprows=1, unpack=True)
        
        # Función errorbar dibuja el punto, la línea y la barra de incertidumbre
        plt.errorbar(T, m_dom, yerr=error, fmt='-o', markersize=4, color=color, 
                     capsize=3, elinewidth=1.5, label=etiqueta)
    else:
        print(f"Aviso: No se encontró el archivo '{archivo}'.")

# 3. Formato
plt.title("Transición de Fase con Barras de Error (Dinámica de Kawasaki)", fontsize=14, pad=15)
plt.xlabel("Temperatura ($T$)", fontsize=12)
plt.ylabel(r"Magnetización por dominios ($m_{dom}$)", fontsize=12)
plt.axvline(x=T_c, color='red', linestyle=':', alpha=0.8, label=rf'$T_c \approx {T_c:.3f}$')

plt.legend(fontsize=11)
plt.grid(True, linestyle=':', alpha=0.7)
plt.xlim(0.4, 3.6)
plt.ylim(-0.05, 1.05)
plt.tight_layout()

plt.savefig("curva_magnetizacion_errores.png", dpi=300)
plt.show()