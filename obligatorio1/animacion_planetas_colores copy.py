# ================================================================================
# ANIMACION SISTEMA SOLAR (VERSIÓN MEJORADA VISUALMENTE)
# ================================================================================

from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Circle
import numpy as np

# ========================================
# Parámetros
# ========================================
file_in = "planets_data.dat" 
file_out = "planetas_colores" 

# Límites de los ejes X e Y (Ajusta si los planetas se salen de la pantalla)
x_min = -20
x_max = 20
y_min = -20 
y_max = 20

# Configuración de Animación
interval = 20        # Tiempo entre fotogramas en milisegundos
frame_skip = 1       # Lo dejamos en 1 porque el programa C++ ya recortó los datos
save_to_file = True  # True: guarda en mp4. False: muestra en ventana
dpi = 150            # Calidad del vídeo 

# Configuración Visual
show_trail = True 
trail_width = 1.5 
# Lista de colores: [Sol, Mercurio, Venus, Tierra, Marte, Júpiter, Saturno, Urano]
planet_colors = ['#FFD700', 'gray', 'orange', 'deepskyblue', 'tomato', 'sandybrown', 'khaki', 'paleturquoise'] 

# Tamaños: [Sol, Mercurio, Venus, Tierra, Marte, Júpiter, Saturno, Urano]
planet_radius = [0.75, 0.4, 0.45, 0.50, 0.35, 0.65, 0.65, 0.55]


# ========================================
# Lectura del fichero de datos
# ========================================
with open(file_in, "r") as f:
    data_str = f.read()

frames_data = list()

for frame_data_str in data_str.split("\n\n"):
    frame_data = list()
    for planet_pos_str in frame_data_str.split("\n"):
        planet_pos = np.fromstring(planet_pos_str, sep=",")
        if planet_pos.size > 0:
            frame_data.append(np.fromstring(planet_pos_str, sep=","))
    if frame_data: # Evitar añadir bloques vacíos
        frames_data.append(frame_data)

# Aplicamos el salto de fotogramas (en este caso es 1, así que coge todos)
if len(frames_data) > frame_skip:
    frames_data = frames_data[::frame_skip]

nplanets = len(frames_data[0])


# ========================================
# Creación de la animación/gráfico
# ========================================
# Crea los objetos figure y axis con fondo negro
fig, ax = plt.subplots()
fig.patch.set_facecolor('black') # Fondo exterior de la ventana
ax.set_facecolor('black')        # Fondo interior del gráfico

# Define el rango de los ejes y quita los bordes
ax.axis("equal")  
ax.set_xlim(x_min, x_max)
ax.set_ylim(y_min, y_max)
ax.axis('off') # ¡Esto oculta los números y ejes para que parezca el espacio!

if not hasattr(planet_radius, "__iter__"):
    planet_radius = planet_radius*np.ones(nplanets)
else:
    # Si pusiste menos radios que planetas, rellena con 1 para que no de error
    while len(planet_radius) < nplanets:
        planet_radius.append(1.0)

planet_points = list()
planet_trails = list()

for j, (planet_pos, radius) in enumerate(zip(frames_data[0], planet_radius)):
    x, y = planet_pos
    
    # Si hay más planetas que colores, vuelve a empezar la lista
    color = planet_colors[j % len(planet_colors)]

    planet_point = Circle((x, y), radius, color=color, zorder=3)
    ax.add_artist(planet_point)
    planet_points.append(planet_point)

    if show_trail:
        # Transparencia (alpha) para que la estela no tape al planeta
        planet_trail, = ax.plot(
                x, y, "-", linewidth=trail_width,
                color=color, alpha=0.5, zorder=2)
        planet_trails.append(planet_trail)
 
def update(j_frame, frames_data, planet_points, planet_trails, show_trail):
    for j_planet, planet_pos in enumerate(frames_data[j_frame]):
        x, y = planet_pos
        planet_points[j_planet].center = (x, y)

        if show_trail:
            xs_old, ys_old = planet_trails[j_planet].get_data()
            xs_new = np.append(xs_old, x)
            ys_new = np.append(ys_old, y)
            planet_trails[j_planet].set_data(xs_new, ys_new)

    return planet_points + planet_trails

def init_anim():
    if show_trail:
        for j_planet in range(nplanets):
            planet_trails[j_planet].set_data(list(), list())
    return planet_points + planet_trails

nframes = len(frames_data)

if nframes > 1:
    animation = FuncAnimation(
            fig, update, init_func=init_anim,
            fargs=(frames_data, planet_points, planet_trails, show_trail),
            frames=len(frames_data), blit=True, interval=interval)

    if save_to_file:
        # Si te sigue dando problemas el mp4, cambia la línea de abajo por:
        # animation.save(f"{file_out}.gif", dpi=dpi, writer='pillow')
        animation.save(f"{file_out}.mp4", dpi=dpi, writer='ffmpeg')
    else:
        plt.show()
else:
    if save_to_file:
        fig.savefig(f"{file_out}.pdf", facecolor=fig.get_facecolor())
    else:
        plt.show()


