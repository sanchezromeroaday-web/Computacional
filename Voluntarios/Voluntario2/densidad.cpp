#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
#include <random>

using namespace std;

int main() {
    int dim = 128;                 // Usar 128 para que la curva sea suave
    int mcs_termalizacion = 40000; // Muchos pasos para equilibrar bien
    int mcs_medida = 10000;        
    unsigned int seed = 123456789;
    
    std::mt19937 g(seed);

    // Seleccionamos las 4 temperaturas que cuentan la historia térmica
    vector<float> temperaturas = {1.0f, 1.8f, 2.3f, 3.5f};

    for (float T : temperaturas) {
        cout << "Calculando perfil para T = " << T << "..." << endl;
        
        // 1. INICIALIZACIÓN ORDENADA POR DOMINIOS
        vector<vector<int>> s(dim, vector<int>(dim));
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                if (i < dim / 2) s[i][j] = -1; 
                else s[i][j] = 1;  
            }
        }

        // 2. TERMALIZACIÓN
        for (int k = 0; k < mcs_termalizacion; k++) {
            for (int l = 0; l < dim * dim; l++) {
                int i = std::rand() % (dim - 2) + 1;
                int j = std::rand() % dim;
                int dir = std::rand() % 4;
                int ni = i, nj = j;

                if (dir == 0) ni = i - 1;
                else if (dir == 1) ni = i + 1;
                else if (dir == 2) nj = (j - 1 + dim) % dim;
                else if (dir == 3) nj = (j + 1) % dim;

                if (ni == 0 || ni == dim - 1) continue;
                if (s[i][j] == s[ni][nj]) continue;

                int hA = s[i-1][j] + s[i+1][j] + s[i][(j-1+dim)%dim] + s[i][(j+1)%dim];
                int hB = s[ni-1][nj] + s[ni+1][nj] + s[ni][(nj-1+dim)%dim] + s[ni][(nj+1)%dim];
                int sA = s[i][j];
                float dE = 2 * sA * (hA - hB) + 4.0f;

                if (dE <= 0 || (static_cast<float>(std::rand()) / RAND_MAX) < std::exp(-dE / T)) {
                    s[i][j] = s[ni][nj];
                    s[ni][nj] = sA;
                }
            }
        }

        // 3. MEDIDA DEL PERFIL DE DENSIDAD
        // Vector para acumular la densidad en cada fila
        vector<float> densidad_fila(dim, 0.0f);

        for (int k = 0; k < mcs_medida; k++) {
            // Un paso MC de Kawasaki
            for (int l = 0; l < dim * dim; l++) {
                int i = std::rand() % (dim - 2) + 1;
                int j = std::rand() % dim;
                int dir = std::rand() % 4;
                int ni = i, nj = j;

                if (dir == 0) ni = i - 1;
                else if (dir == 1) ni = i + 1;
                else if (dir == 2) nj = (j - 1 + dim) % dim;
                else if (dir == 3) nj = (j + 1) % dim;

                if (ni == 0 || ni == dim - 1) continue;
                if (s[i][j] == s[ni][nj]) continue;

                int hA = s[i-1][j] + s[i+1][j] + s[i][(j-1+dim)%dim] + s[i][(j+1)%dim];
                int hB = s[ni-1][nj] + s[ni+1][nj] + s[ni][(nj-1+dim)%dim] + s[ni][(nj+1)%dim];
                int sA = s[i][j];
                float dE = 2 * sA * (hA - hB) + 4.0f;

                if (dE <= 0 || (static_cast<float>(std::rand()) / RAND_MAX) < std::exp(-dE / T)) {
                    s[i][j] = s[ni][nj];
                    s[ni][nj] = sA;
                }
            }

            // Tras barrer la red, sumamos la densidad actual de cada fila
            for (int i = 0; i < dim; i++) {
                float densidad_actual = 0;
                for (int j = 0; j < dim; j++) {
                    // Convertimos el espín a densidad de partículas (0 o 1)
                    densidad_actual += (s[i][j] + 1.0f) / 2.0f; 
                }
                // Promediamos por el ancho de la red y acumulamos
                densidad_fila[i] += densidad_actual / dim;
            }
        }

        // 4. GUARDAR DATOS
        string filename = "perfil_densidad_T" + to_string(T) + ".dat";
        ofstream data_out(filename);
        data_out << "y\tDensidad\n";
        
        for (int i = 0; i < dim; i++) {
            // Dividimos entre las medidas para obtener la media estadística
            data_out << i << "\t" << (densidad_fila[i] / mcs_medida) << "\n";
        }
        data_out.close();
    }

    cout << "Todos los perfiles calculados con exito." << endl;
    return 0;
}