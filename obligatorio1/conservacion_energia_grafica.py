import pandas as pd
import matplotlib.pyplot as plt

# ========================================================
# ANÁLISIS DE CONSERVACIÓN: ENERGÍA Y MOMENTO ANGULAR
# ========================================================

print("Leyendo datos de energia_momento.txt...")

# 1. Leer los datos generados por C++
# El separador es el tabulador ("\t") tal como lo programaste en C++
energia = pd.read_csv("energia_momento.txt", sep="\t", names=["t", "E", "L"])

# 2. Crear la figura con dos subgráficas (una encima de la otra)
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

# --- Gráfica Superior: Momento Angular (L) ---
ax1.plot(energia['t'], energia['L'], color='purple')
ax1.set_title("Conservación del Momento Angular ($L$)")
ax1.set_ylabel("Momento Angular")
ax1.grid(True, linestyle='--', alpha=0.7)

# --- Gráfica Inferior: Energía Total (E) ---
ax2.plot(energia['t'], energia['E'], color='green')
ax2.set_title("Conservación de la Energía Mecánica Total ($E$)")
ax2.set_xlabel("Tiempo de simulación (t)")
ax2.set_ylabel("Energía Total")
ax2.grid(True, linestyle='--', alpha=0.7)

# 3. Ajustar espacios, guardar y mostrar
plt.tight_layout() # Ajusta automáticamente los márgenes para que no se pisen los textos

print("Guardando imágenes...")
plt.savefig("conservacion_E_L.png", dpi=300) # Imagen ideal para presentaciones
plt.savefig("conservacion_E_L.pdf")          # PDF ideal para el informe LaTeX

print("Abriendo ventana de gráficas...")
plt.show()