import numpy as np
import matplotlib.pyplot as plt
import io

# ========================================
# Parámetros
# ========================================
file_in = "ising_kawasaki.dat" # El archivo que genera tu C++

# Indica aquí qué números de iteración quieres guardar.
# IMPORTANTE: Como C++ empieza a contar en 0, si tu iter=500, el último es el 499.
fotogramas_a_guardar = [0, 3333,6666 , 9999] 

# ========================================
# Lectura del fichero de datos
# ========================================
with open(file_in, "r") as f:
    data_str = f.read()

frames_data = list()
for frame_data_str in data_str.split("\n\n"):
    if frame_data_str.strip(): # Comprobación para evitar fallos con líneas vacías
        frame_data = np.loadtxt(io.StringIO(frame_data_str), delimiter=",")
        frames_data.append(frame_data)

# ========================================
# Generación de imágenes PDF
# ========================================
for idx in fotogramas_a_guardar:
    # Nos aseguramos de que el fotograma que pides realmente existe
    if idx < len(frames_data):
        fig, ax = plt.subplots()
        ax.axis("off")  # Ocultar los ejes
        
        # Representar con el mapa de color morado
        ax.imshow(frames_data[idx], cmap="Purples", vmin=-1, vmax=+1)
        
        # Guardar la imagen en PDF recortando los márgenes blancos extra
        nombre_archivo = f"fotograma_{idx}.png"
        fig.savefig(nombre_archivo, bbox_inches='tight', dpi=150)
        plt.close(fig) # Cerrar la figura para liberar memoria
        
        print(f"Éxito: Se ha guardado {nombre_archivo}")
    else:
        print(f"Aviso: El fotograma {idx} no existe. El archivo solo tiene {len(frames_data)} fotogramas.")