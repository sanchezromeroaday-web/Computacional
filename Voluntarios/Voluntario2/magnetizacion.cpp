#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <random>

using namespace std;

int main() {
    int dim = 32;                  // Cambiar a 64 y 128
    int mcs_termalizacion = 40000; // Mantenemos tu termalización larga
    int mcs_medida = 10000;        
    unsigned int seed = 1649176165;
    
    std::mt19937 g(seed);
    
    ofstream data_out("magnetizacion_32.dat");
    data_out << "T\tm_dom\terror\n"; // ¡Nueva cabecera de 3 columnas!

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

        // 3. MEDIDA DEL OBSERVABLE Y SU CUADRADO
        float suma_m_dom = 0;
        float suma_m_dom_cuadrado = 0; // NUEVO: para la varianza

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

            float m_sup = 0, m_inf = 0;
            for (int i = 0; i < dim / 2; i++) {
                for (int j = 0; j < dim; j++) m_sup += s[i][j];
            }
            for (int i = dim / 2; i < dim; i++) {
                for (int j = 0; j < dim; j++) m_inf += s[i][j];
            }

            int particulas = (dim / 2) * dim;
            m_sup /= particulas;
            m_inf /= particulas;

            float m_dom_actual = (std::abs(m_sup) + std::abs(m_inf)) / 2.0f;
            
            // Acumulamos el valor y su cuadrado
            suma_m_dom += m_dom_actual;
            suma_m_dom_cuadrado += m_dom_actual * m_dom_actual;
        }

        // 4. CÁLCULO DE MEDIA Y ERROR ESTADÍSTICO
        float m_media = suma_m_dom / mcs_medida;
        float m_cuadrado_media = suma_m_dom_cuadrado / mcs_medida;
        
        // Varianza = <m^2> - <m>^2 (usamos abs por si hay errores de precisión de coma flotante)
        float varianza = std::abs(m_cuadrado_media - (m_media * m_media));
        
        // Error de la media (Desviación típica / sqrt(N))
        float error = std::sqrt(varianza / mcs_medida);

        data_out << T << "\t" << m_media << "\t" << error << "\n";
        std::cout << "Calculado T = " << T << " | m_dom = " << m_media << " +- " << error << std::endl;
    }

    data_out.close();
    return 0;
}