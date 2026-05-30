import numpy as np
import matplotlib.pyplot as plt
import os

# ========================================
# Parámetros de la gráfica
# ========================================
# Lista de tuplas con (nombre_del_archivo, etiqueta_para_la_leyenda, color)
archivos = [
    ("magnetizacion_32.dat", "N = 32", "blue"),
    ("magnetizacion_64.dat", "N = 64", "orange"),
    ("magnetizacion_128.dat", "N = 128", "green")
]

# Crear la figura
plt.figure(figsize=(8, 6))

# ========================================
# Lectura de datos y representación
# ========================================
for archivo, etiqueta, color in archivos:
    # Comprobamos si el archivo existe para evitar errores
    if os.path.exists(archivo):
        # np.loadtxt ignora automáticamente la primera línea si es texto (la cabecera)
        # unpack=True separa las columnas en variables distintas
        T, m_dom = np.loadtxt(archivo, skiprows=1, unpack=True)
        
        # Pintamos la línea con marcadores
        plt.plot(T, m_dom, marker='o', markersize=4, linestyle='-', color=color, label=etiqueta)
    else:
        print(f"Aviso: No se encontró el archivo '{archivo}'. Omítelo o genéralo en C++.")

# ========================================
# Formato de la gráfica
# ========================================
plt.title("Magnetización por dominios frente a la Temperatura", fontsize=14)
plt.xlabel(r"Temperatura ($T$)", fontsize=12)
plt.ylabel(r"Magnetización por dominios ($m_{dom}$)", fontsize=12)

# Añadimos una línea vertical punteada aproximada en la temperatura crítica analítica
plt.axvline(x=2.269, color='red', linestyle='--', alpha=0.6, label=r'$T_c \approx 2.269$')

plt.legend(fontsize=11)
plt.grid(True, linestyle=':', alpha=0.7)
plt.tight_layout()

# Guardamos la gráfica en alta calidad para el informe y la mostramos
plt.savefig("curva_magnetizacion.png", dpi=300)
plt.show()