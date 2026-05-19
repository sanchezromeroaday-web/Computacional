import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Cargar los datos
data = np.loadtxt("data.dat")

moon_x = data[:, 0]
moon_y = data[:, 1]
rocket_x = data[:, 2]
rocket_y = data[:, 3]

IT = len(data) # Total de iteraciones

fig, ax = plt.subplots(figsize=(7, 7))

# Arrays vacíos para la estela del cohete
xldata, yldata = [], []

# Inicialización de los elementos gráficos (nota la coma después de las variables)
tierra, = plt.plot(0, 0, linestyle='None', color="Blue", marker='o', markersize=15, label="Tierra")
luna, = plt.plot([], [], linestyle='None', color="Grey", marker='o', markersize=8, label="Luna")
cohete, = plt.plot([], [], linestyle='None', color="Red", marker='.', markersize=6, label="Nave")
lcohete, = plt.plot([], [], linestyle='-', color="Red", alpha=0.5, linewidth=1.5)

def init():
    ax.set_title("Viaje a la Luna")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_xlim(-1.2, 1.2)
    ax.set_ylim(-1.2, 1.2)
    ax.legend(loc="upper right")
    ax.grid(True, linestyle=':', alpha=0.6)
    return luna, cohete, lcohete

def update(i):
    # Controlamos el tamaño de la estela (trail) del cohete (ej. últimos 150 puntos)
    if len(xldata) > 150:
        xldata.pop(0)
        yldata.pop(0)

    xldata.append(rocket_x[i])
    yldata.append(rocket_y[i])
    
    lcohete.set_data(xldata, yldata)
    
    # En versiones recientes de matplotlib, set_data espera listas o arrays
    luna.set_data([moon_x[i]], [moon_y[i]])
    cohete.set_data([rocket_x[i]], [rocket_y[i]])

    return luna, cohete, lcohete

# Animamos tomando 1 frame cada 1000 iteraciones para no sobrecargar el GIF
paso_frames = 1000
frames_totales = range(0, IT, paso_frames)

anim = animation.FuncAnimation(fig, update, frames=frames_totales, 
                               init_func=init, blit=True)

# Guardar como GIF
print("Generando GIF, esto puede tardar un momento...")
anim.save("cohete.gif", writer=animation.PillowWriter(fps=30))
print("GIF guardado con éxito.")

plt.close()