import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

print("Leyendo datos para la animación...")
try:
    with open('datos_galaxia.dat', 'r') as f:
        bloques = f.read().split('\n\n')
except FileNotFoundError:
    print("Error: No se encuentra 'datos_galaxia.dat'.")
    exit()

frames = []
for bloque in bloques:
    if bloque.strip() == '':
        continue
    lineas = bloque.strip().split('\n')
    coords = np.array([list(map(float, linea.split())) for linea in lineas])
    frames.append(coords)

print(f"Se han cargado {len(frames)} frames. Generando animación...")

fig, ax = plt.subplots(figsize=(8, 8))
LIMITE = 8000 
ax.set_xlim(-LIMITE, LIMITE)
ax.set_ylim(-LIMITE, LIMITE)
ax.set_xlabel("x (pc)")
ax.set_ylabel("y (pc)")

# AÑADIDO: Etiqueta para el agujero negro
ax.plot(0, 0, 'ro', markersize=4, label="Sgr A* (Agujero Negro)")

# AÑADIDO: Etiqueta para los sistemas solares (usamos un punto invisible para crear la leyenda)
ax.scatter([], [], s=1, c='blue', alpha=0.5, label="Sistemas Solares")

# Mostramos la leyenda
ax.legend(loc="upper right", markerscale=5)

scatter = ax.scatter([], [], s=1, c='blue', alpha=0.5)

def update(frame_idx):
    data = frames[frame_idx]
    scatter.set_offsets(data)
    ax.set_title(f"Evolución de la Galaxia - Frame {frame_idx}")
    return scatter,

ani = animation.FuncAnimation(fig, update, frames=len(frames), interval=50, blit=True)

# AÑADIDO: Guardar la animación como GIF en la misma carpeta
nombre_archivo_animacion = "animacion_galaxia.gif"
print(f"Guardando la animación como '{nombre_archivo_animacion}' (Esto puede tardar un minuto)...")
ani.save(nombre_archivo_animacion, writer='pillow', fps=20)
print("¡Animación guardada con éxito!")

plt.show()