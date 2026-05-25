import numpy as np
import matplotlib.pyplot as plt

print("Cargando datos termodinámicos...")
try:
    datos = np.loadtxt('energias_galaxia.dat')
except FileNotFoundError:
    print("Error: No se encuentra 'energias_galaxia.dat'.")
    exit()

# Extraer columnas
tiempo = datos[:, 0]
K = datos[:, 1]
U = datos[:, 2]
E_total = datos[:, 3]
Lz = datos[:, 4]
virial = datos[:, 5]

# Crear un panel con 3 subgráficas verticales
fig, axs = plt.subplots(3, 1, figsize=(10, 12), sharex=True)

# 1. Gráfica de Energías
axs[0].plot(tiempo, K, label='Cinética (K)', color='blue')
axs[0].plot(tiempo, U, label='Potencial (U)', color='red')
axs[0].plot(tiempo, E_total, label='Energía Total (E = K + U)', color='black', linewidth=2)
axs[0].set_title('Evolución de las Energías')
axs[0].set_ylabel('Energía')
axs[0].legend(loc='center right')
axs[0].grid(True, linestyle='--', alpha=0.6)

# 2. Gráfica del Momento Angular
# Centramos el eje Y usando la media y un margen del 5% igual que hicimos con la inercia
media_Lz = np.mean(Lz)
axs[1].plot(tiempo, Lz, label='Momento Angular ($L_z$)', color='green')
axs[1].set_ylim(media_Lz * 0.95, media_Lz * 1.05)
axs[1].set_title('Evolución del Momento Angular Total')
axs[1].set_ylabel('$L_z$')
axs[1].legend(loc='upper right')
axs[1].grid(True, linestyle='--', alpha=0.6)

# 3. Gráfica del Teorema del Virial
axs[2].plot(tiempo, virial, label='$2K + U$', color='purple')
axs[2].axhline(0, color='black', linestyle='-', linewidth=2, label='Equilibrio (Cero)')
axs[2].set_title('Demostración del Teorema del Virial')
axs[2].set_xlabel('Tiempo (unidades reescaladas)')
axs[2].set_ylabel('Valor del Virial')
axs[2].legend(loc='upper right')
axs[2].grid(True, linestyle='--', alpha=0.6)

plt.tight_layout()

nombre_archivo = "observables_termodinamicos.png"
plt.savefig(nombre_archivo, dpi=300, bbox_inches='tight')
print(f"Imagen guardada automáticamente como: {nombre_archivo}")

plt.show()