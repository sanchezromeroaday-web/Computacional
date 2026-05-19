import numpy as np
import matplotlib.pyplot as plt

# Cargar los datos
# Para esta gráfica solo necesitamos la columna 4 (Tiempo) y la 5 (H_prima)
data = np.loadtxt("data.dat")

t = data[:, 4]
H_prima = data[:, 5]

# ==========================================
# GRÁFICA: CONSERVACIÓN DE H'
# ==========================================
fig, ax = plt.subplots(figsize=(10, 4))

ax.plot(t, H_prima, color='purple', linewidth=1.5, label=r"$H'$ (Integral de Jacobi)")

ax.set_title("Evolución de la constante del movimiento $H'$", fontsize=14)
ax.set_xlabel("Tiempo (adim)", fontsize=12)
ax.set_ylabel(r"$H'$", fontsize=12)
ax.grid(True, linestyle=':', alpha=0.7)
ax.legend()

# Usar notación científica en el eje Y si las variaciones son muy pequeñas
ax.ticklabel_format(useOffset=False, style='plain')

fig.tight_layout()
fig.savefig("constante_H.png", dpi=300)

plt.show()