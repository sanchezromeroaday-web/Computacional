import numpy as np
import matplotlib.pyplot as plt

# ==========================================
# PARÁMETRO FÍSICO IMPORTANTÍSIMO
# ==========================================
# Sustituye esto por el valor de (G * M_BH) que estés usando en tu C++
GM_BH = 1.0  # <--- ¡CAMBIAR AQUÍ!

print("Cargando datos de rotación...")
try:
    datos = np.loadtxt('curva_rotacion.dat')
except FileNotFoundError:
    print("Error: No se encuentra 'curva_rotacion.dat'. Ejecuta tu C++ primero.")
    exit()

radios = datos[:, 0]
velocidades = datos[:, 1]

# ==========================================
# AGRUPAR EN BINS (VELOCIDAD MEDIA POR RADIO)
# ==========================================
num_bins = 40
R_MAX = 8000  # Tu RADIO_GALAXIA
bordes_bins = np.linspace(100, R_MAX, num_bins + 1)
centros_bins = (bordes_bins[1:] + bordes_bins[:-1]) / 2

vel_media = np.zeros(num_bins)
vel_error = np.zeros(num_bins)

for i in range(num_bins):
    # Seleccionamos las estrellas que caen en este anillo
    mascara = (radios >= bordes_bins[i]) & (radios < bordes_bins[i+1])
    vel_en_bin = velocidades[mascara]
    
    if len(vel_en_bin) > 0:
        vel_media[i] = np.mean(vel_en_bin)
        # El error es la desviación estándar (dispersión de velocidades)
        vel_error[i] = np.std(vel_en_bin) 

# Filtrar bins vacíos para que no den error al plotear
bins_validos = vel_media > 0
centros_validos = centros_bins[bins_validos]
vel_media = vel_media[bins_validos]
vel_error = vel_error[bins_validos]

# ==========================================
# CURVA TEÓRICA (KEPLER)
# ==========================================
r_teorico = np.linspace(min(centros_validos), R_MAX, 500)
# Fórmula: v = sqrt(G * M_BH / r)
v_teorica = np.sqrt(GM_BH / r_teorico)

# ==========================================
# PLOTEAR RESULTADOS
# ==========================================
plt.figure(figsize=(9, 6))

# Plotear curva teórica
plt.plot(r_teorico, v_teorica, color='red', linewidth=2, linestyle='--', zorder=3, 
         label=r'Teoría Kepleriana ($v \propto 1/\sqrt{r}$)')

# Plotear datos de la simulación
plt.errorbar(centros_validos, vel_media, yerr=vel_error, fmt='o', color='royalblue', 
             capsize=3, elinewidth=1.5, markersize=5, zorder=2, alpha=0.9,
             label='Simulación (Medias y dispersión)')

# Para darle contexto, dibujamos las estrellas de fondo muy suavizadas
plt.scatter(radios, velocidades, s=2, color='gray', alpha=0.15, zorder=1, label='Sistemas solares individuales')

plt.title("Curva de Rotación Galáctica: Simulación vs Teoría")
plt.xlabel("Distancia al Centro Galáctico $r$")
plt.ylabel("Velocidad Orbital $v(r)$")
plt.grid(True, linestyle='--', alpha=0.6)
plt.legend(loc="upper right")

plt.tight_layout()
nombre_archivo = "curva_rotacion_comparativa.png"
plt.savefig(nombre_archivo, dpi=300, bbox_inches='tight')
print(f"¡Gráfica guardada como {nombre_archivo}!")

plt.show()