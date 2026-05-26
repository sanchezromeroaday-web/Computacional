# ================================================================================
# ANIMACION SCHRÖDINGER
# ================================================================================

from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import io

# Parámetros
# ========================================
file_in = "fonda.dat"     # Nombre del fichero de datos
file_out = "schrodinger"  # Nombre del fichero de salida (sin extensión)
interval = 50             # Tiempo entre fotogramas en milisegundos (reducido para mayor fluidez)
save_to_file = True       # False: pantalla, True: fichero
dpi = 150                 # Calidad del vídeo de salida
step = 10                 # NUEVO: Saltará de 10 en 10 fotogramas para acortar la animación

# Lectura del fichero de datos
# ========================================
with open(file_in, "r") as f:
    data_str = f.read()

frames_data = list()

for frame_data_str in data_str.split("\n\n"):
    if len(frame_data_str) > 0:
        frame_data = np.loadtxt(io.StringIO(frame_data_str), delimiter=",").T
        frames_data.append(frame_data)

frames_data = np.array(frames_data)

# Creación de la animación/gráfico
# ========================================
fig, ax = plt.subplots(figsize=[8, 5]) # Ampliado un poco para que quepa bien la leyenda

# Define el rango de los ejes
xmin = np.amin(frames_data[0][0])
xmax = np.amax(frames_data[0][0])
ymin = np.amin(frames_data[:,1:])
ymax = np.amax(frames_data[:,1:])
ax.set_xlim(xmin, xmax)
ax.set_ylim(ymin, ymax * 1.1) # Dejo un 10% de margen arriba para que respire la gráfica

# NUEVO: Etiquetas de los ejes
ax.set_xlabel("Posición (Nodos)")
ax.set_ylabel("Amplitud de Probabilidad")
# NUEVO: Representación gráfica del Potencial con altura definida
# Define aquí una altura visual que quede bien en tu gráfica actual.
# En tu imagen, el pico está en 0.09. Vamos a poner la barrera un poco más alta.
altura_visual_barrera = 0.12 # Puedes cambiar este valor según te guste visualmente

# Usamos fill_between para controlar la parte superior (y2)
ax.fill_between([400, 600], y1=0, y2=altura_visual_barrera, color='lightgray', alpha=0.5, label='Barrera de Potencial')

# NOTA OPCIONAL: Ajusta un poco más el límite superior del eje Y para
# que el rectángulo y la leyenda no se pisen.
ymax = np.amax(frames_data[:,1:])
ax.set_ylim(ymin, max(ymax, altura_visual_barrera) * 1.15) # Asegura espacio arriba
# Representa el primer fotograma
xs = frames_data[0][0]
lines = list()
for ys in frames_data[0][1:]:
    # NUEVO: Se añade el atributo "label" para la leyenda
    line, = ax.plot(xs, ys, "-", color="blue", linewidth=1.5, label="Función de Onda")
    lines.append(line)

# NUEVO: Activar la leyenda
ax.legend(loc="upper right")
 
# Función que actualiza las curvas en la animación 
def update(j_frame, frames_data, lines):
    xs = frames_data[j_frame][0]
    for j_curve, ys in enumerate(frames_data[j_frame][1:]):
        lines[j_curve].set_data(xs, ys)
    return lines

nframes = len(frames_data)

# Si hay más de un instante de tiempo, genera la animación
if nframes > 1:
    # NUEVO: Uso de "range(0, nframes, step)" para omitir fotogramas y hacer el vídeo más corto/rápido
    animation = FuncAnimation(
            fig, update,
            fargs=(frames_data, lines), 
            frames=range(0, nframes, step), 
            blit=True, 
            interval=interval)

    if save_to_file:
        animation.save("{}.mp4".format(file_out), dpi=dpi)
    else:
        plt.show()
else:
    if save_to_file:
        fig.savefig("{}.pdf".format(file_out))
    else:
        plt.show()