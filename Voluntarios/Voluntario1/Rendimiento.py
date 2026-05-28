import pandas as pd
import matplotlib.pyplot as plt

# Cargar los datos
df_portatil = pd.read_csv('rendimiento_portatil.csv')
df_cluster = pd.read_csv('rendimiento_cluster.csv')
df_portatil_03 = pd.read_csv('rendimiento_portatil_03.csv')

plt.figure(figsize=(10, 6))

# Quitamos los 'marker' y jugamos solo con el tipo de línea (linestyle) y grosor (linewidth)
plt.plot(df_portatil['Paso'], df_portatil['Tiempo_ms'], 
         label='Portátil (Normal)', linestyle='--', linewidth=2)

plt.plot(df_cluster['Paso'], df_cluster['Tiempo_ms'], 
         label='Clúster Facultad', linestyle='-.', linewidth=2)

plt.plot(df_portatil_03['Paso'], df_portatil_03['Tiempo_ms'], 
         label='Portátil Optimizado (-O3)', color='green', linestyle='-', linewidth=2.5)

plt.title('Comparativa de Rendimiento: Portátil vs Clúster')
plt.xlabel('Paso de Simulación')
plt.ylabel('Tiempo de ejecución (ms)')
plt.legend()
plt.grid(True, alpha=0.6)

plt.show()