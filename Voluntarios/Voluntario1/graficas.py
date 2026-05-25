import numpy as np
import matplotlib.pyplot as plt

# ==========================================
# 1. CARGAR DATOS
# ==========================================
print("Cargando datos para análisis...")
try:
    with open('datos_galaxia.dat', 'r') as f:
        bloques = f.read().split('\n\n')
except FileNotFoundError:
    print("Error: No se encuentra el archivo 'datos_galaxia.dat'.")
    exit()

frames = []
for bloque in bloques:
    if bloque.strip() == '': 
        continue
    lineas = bloque.strip().split('\n')
    coords = np.array([list(map(float, linea.split())) for linea in lineas])
    frames.append(coords)

N_frames = len(frames)
if N_frames == 0:
    print("Error: El archivo de datos está vacío.")
    exit()

print(f"Se han cargado {N_frames} frames. Calculando física...")

# ==========================================
# 2. CALCULAR MOMENTO DE INERCIA EN EL TIEMPO
# ==========================================
inercia = np.zeros(N_frames)
for i, data in enumerate(frames):
    r_cuadrado = data[:, 0]**2 + data[:, 1]**2
    inercia[i] = np.sum(r_cuadrado)

# ==========================================
# 3. DENSIDAD RADIAL MEDIA Y ERRORES
# ==========================================
frames_estacionarios = frames[int(N_frames * 0.75):]
radios_todos = []

for data in frames_estacionarios:
    r = np.sqrt(data[:, 0]**2 + data[:, 1]**2)
    radios_todos.extend(r)

radios_todos = np.array(radios_todos)

R_MAX = 8000
num_bins = 50
cuentas, bordes_bins = np.histogram(radios_todos, bins=num_bins, range=(0, R_MAX))

areas = np.pi * (bordes_bins[1:]**2 - bordes_bins[:-1]**2)
centros_bins = (bordes_bins[1:] + bordes_bins[:-1]) / 2

densidad_radial = cuentas / (areas * len(frames_estacionarios))
error_densidad = np.sqrt(cuentas) / (areas * len(frames_estacionarios))

# ==========================================
# 4. PLOTEAR, GUARDAR Y MOSTRAR RESULTADOS
# ==========================================
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))

# Gráfica del Momento de Inercia
# AÑADIDO: label para la leyenda
ax1.plot(inercia, color='purple', label='Inercia simulada')
ax1.set_title("Evolución temporal del Momento de Inercia")
# Calculamos la media para centrar la gráfica
media_inercia = np.mean(inercia)

# Le damos un margen del +/- 5% por arriba y por abajo
ax1.set_ylim(media_inercia * 0.95, media_inercia * 1.05)
ax1.set_xlabel("Frames (Tiempo)")
ax1.set_ylabel("Momento de Inercia (I)")
ax1.grid(True, linestyle='--', alpha=0.6)
ax1.legend(loc="lower right") # AÑADIDO: Muestra la leyenda

# Gráfica de la Densidad Radial
# AÑADIDO: label para la leyenda
ax2.errorbar(centros_bins, densidad_radial, yerr=error_densidad, 
             marker='o', markersize=4, color='teal', linestyle='-', 
             capsize=3, elinewidth=1.5, alpha=0.8, label=r'Densidad $\rho(r)$')
ax2.set_title("Distribución radial media de la densidad de masa")
ax2.set_xlabel("Distancia al Agujero Negro (r)")
ax2.set_ylabel(r"Densidad $\rho(r)$")
ax2.grid(True, linestyle='--', alpha=0.6)
ax2.legend(loc="upper right") # AÑADIDO: Muestra la leyenda

plt.tight_layout()

# AÑADIDO: Guardar la imagen en la misma carpeta antes de mostrarla
nombre_archivo_imagen = "graficas_estado_estacionario.png"
plt.savefig(nombre_archivo_imagen, dpi=300, bbox_inches='tight')
print(f"Imagen guardada automáticamente como: {nombre_archivo_imagen}")
# Calculamos la inercia media solo del último cuarto (estado estacionario)
inercia_media_estacionaria = np.mean(inercia[int(N_frames * 0.75):])
print(f"Momento de inercia medio (estado estacionario): {inercia_media_estacionaria:.2e}")

plt.show()