import numpy as np
import matplotlib.pyplot as plt
import os

archivos = [
    ("energia_32.dat", "N = 32", "blue"),
    ("energia_64.dat", "N = 64", "orange"),
    ("energia_128.dat", "N = 128", "green")
]

plt.figure(figsize=(9, 6))

# Lectura de datos y dibujo con barras de error
for archivo, etiqueta, color in archivos:
    if os.path.exists(archivo):
        # Desempaquetamos T, Energía Media y Error
        T, E_media, error = np.loadtxt(archivo, skiprows=1, unpack=True)
        
        # plt.errorbar en lugar de plt.plot
        plt.errorbar(T, E_media, yerr=error, fmt='-o', markersize=4, color=color, 
                     capsize=3, elinewidth=1.5, label=etiqueta)
    else:
        print(f"⚠️ Aviso: No se encontró el archivo '{archivo}'.")

# Formato estético y físico
plt.title("Energía Media por Partícula con Barras de Error", fontsize=14, pad=15)
plt.xlabel("Temperatura ($T$)", fontsize=12)
plt.ylabel(r"Energía media por partícula ($\langle E \rangle / N^2$)", fontsize=12)

# Línea crítica
T_c = 2.0 / np.log(1.0 + np.sqrt(2.0))
plt.axvline(x=T_c, color='red', linestyle='--', alpha=0.7, label=rf'$T_c \approx {T_c:.3f}$')

plt.legend(fontsize=11)
plt.grid(True, linestyle=':', alpha=0.7)
plt.xlim(0.4, 3.6)
plt.tight_layout()

plt.savefig("curva_energia_errores.png", dpi=300)
plt.show()