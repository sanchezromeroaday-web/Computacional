#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <random>

using namespace std;

int main() {
    int dim = 64;                  // Cambiar a 64 y 128
    int mcs_termalizacion = 40000; 
    int mcs_medida = 10000;        
    unsigned int seed = 987654321;
    
    std::mt19937 g(seed);
    
    // Cambiar nombre según dim
    ofstream data_out("energia_64.dat");
    data_out << "T\tE_media\terror\n"; // 3 columnas

    for (float T = 0.5f; T <= 3.5f; T += 0.1f) {
        
        // 1. INICIALIZACIÓN ORDENADA
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

        // 3. MEDIDA DE LA ENERGÍA Y SU CUADRADO
        double suma_energia_total = 0;
        double suma_energia_cuadrado = 0;

        for (int k = 0; k < mcs_medida; k++) {
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

            double energia_paso = 0;
            for (int i = 0; i < dim; i++) {
                for (int j = 0; j < dim; j++) {
                    energia_paso += -s[i][j] * s[i][(j + 1) % dim];
                    if (i < dim - 1) {
                        energia_paso += -s[i][j] * s[i + 1][j];
                    }
                }
            }
            
            double energia_particula = energia_paso / (dim * dim);
            suma_energia_total += energia_particula;
            suma_energia_cuadrado += (energia_particula * energia_particula);
        }

        // 4. CÁLCULO ESTADÍSTICO (Media, Varianza y Error)
        double e_media = suma_energia_total / mcs_medida;
        double e_cuadrado_media = suma_energia_cuadrado / mcs_medida;
        
        // Varianza = <E^2> - <E>^2
        double varianza = std::abs(e_cuadrado_media - (e_media * e_media));
        
        // Error de la media
        double error = std::sqrt(varianza / mcs_medida);

        data_out << T << "\t" << e_media << "\t" << error << "\n";
        
        std::cout << "T = " << T << " | <E>/N = " << e_media << " +- " << error << std::endl;
    }

    data_out.close();
    std::cout << "\nTerminado para N = " << dim << std::endl;
    return 0;
}