import numpy as np
import matplotlib.pyplot as plt

# ==========================================
# 1. GRÁFICA: FLUJO DE MASA (Masa absorbida vs Tiempo)
# ==========================================
print("Generando gráfica de flujo de masa...")
datos_flujo = np.loadtxt('flujo_masa.dat')
tiempo = datos_flujo[:, 0]
absorbidos = datos_flujo[:, 1]

# Hacemos un ajuste lineal (y = mx + n) de la segunda mitad de la simulación
# (para asegurarnos de que el sistema ya ha entrado en estado estacionario)
mitad = len(tiempo) // 2
coeficientes = np.polyfit(tiempo[mitad:], absorbidos[mitad:], 1)
flujo_medio = coeficientes[0] # La pendiente (m)
ajuste_lineal = coeficientes[0] * tiempo + coeficientes[1]

fig1, ax1 = plt.subplots(figsize=(8, 5))
ax1.plot(tiempo, absorbidos, label='Datos simulados', color='teal', linewidth=2)
ax1.plot(tiempo, ajuste_lineal, color='red', linestyle='--', linewidth=2, 
         label=f'Ajuste estacionario (Flujo = {flujo_medio:.2f} masa/ud_tiempo)')

ax1.set_title("Evolución de la masa absorbida por el agujero negro")
ax1.set_xlabel("Tiempo (unidades reescaladas)")
ax1.set_ylabel("Sistemas solares absorbidos")
ax1.grid(True, linestyle='--', alpha=0.6)
ax1.legend(loc='upper left')

plt.tight_layout()
fig1.savefig("flujo_masa_estacionario.png", dpi=300, bbox_inches='tight')

# ==========================================
# 2. GRÁFICA: MAPA VISUAL 2D CON VELOCIDADES
# ==========================================
print("Generando mapa visual térmico de la galaxia...")
datos_mapa = np.loadtxt('mapa_final.dat')
x = datos_mapa[:, 0]
y = datos_mapa[:, 1]
v = datos_mapa[:, 2]

fig2, ax2 = plt.subplots(figsize=(8, 8))
# cmap='plasma' hace que lo lento sea azul oscuro y lo rápido amarillo/rojo
scatter = ax2.scatter(x, y, c=v, cmap='plasma', s=12, alpha=0.8, edgecolors='none')

# Añadir agujero negro en el centro
ax2.plot(0, 0, marker='o', color='black', markersize=10, label='Sgr A*')

# Añadir barra de color
cbar = plt.colorbar(scatter, ax=ax2, fraction=0.046, pad=0.04)
cbar.set_label('Velocidad orbital ($v$)', rotation=270, labelpad=15)

ax2.set_title("Distribución espacial y velocidades en el estado estacionario")
ax2.set_xlabel("Coordenada x (pc)")
ax2.set_ylabel("Coordenada y (pc)")
ax2.set_aspect('equal', 'box') # Para que el círculo no se vea ovalado
ax2.grid(True, linestyle='--', alpha=0.3)
ax2.legend(loc='upper right')

plt.tight_layout()
fig2.savefig("mapa_galaxia_velocidades.png", dpi=300, bbox_inches='tight')

print("¡Ambas gráficas guardadas con éxito!")
plt.show()