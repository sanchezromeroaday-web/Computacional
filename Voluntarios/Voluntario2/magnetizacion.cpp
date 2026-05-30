#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <random>

using namespace std;

int main() {
   // ==========================================
    // PARÁMETROS DE LA SIMULACIÓN
    // ==========================================
    int dim = 32;                // Empieza probando el más grande
    int mcs_termalizacion = 40000; // ¡Aumentado drásticamente!
    int mcs_medida = 10000;        // Para que la media sea más suave
    unsigned int seed = 1649176165;
    
    std::mt19937 g(seed);
    
    // IMPORTANTE: Cambia el nombre del archivo cuando cambies 'dim'
    ofstream data_out("magnetizacion_32.dat");
    data_out << "T\tm_dom\n"; // Cabecera del archivo

    // Bucle de Temperaturas (desde 0.5 hasta 3.5 en saltos de 0.1)
    for (float T = 0.5f; T <= 3.5f; T += 0.1f) {
        
        // ==========================================
        // 1. INICIALIZACIÓN ORDENADA (Evita estancamiento cinético)
        // ==========================================
        vector<vector<int>> s(dim, vector<int>(dim));
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                if (i < dim / 2) {
                    s[i][j] = -1; // Mitad superior negativa
                } else {
                    s[i][j] = 1;  // Mitad inferior positiva
                }
            }
        }

        // ==========================================
        // 2. TERMALIZACIÓN (Equilibrio sin medir)
        // ==========================================
        for (int k = 0; k < mcs_termalizacion; k++) {
            for (int l = 0; l < dim * dim; l++) {
                // Elegir un espín al azar excluyendo los bordes fijos (i=0 e i=dim-1)
                int i = std::rand() % (dim - 2) + 1;
                int j = std::rand() % dim;
                int dir = std::rand() % 4;
                int ni = i, nj = j;

                // Determinar el vecino
                if (dir == 0) ni = i - 1;
                else if (dir == 1) ni = i + 1;
                else if (dir == 2) nj = (j - 1 + dim) % dim;
                else if (dir == 3) nj = (j + 1) % dim;

                // No intercambiamos si el vecino es un borde fijo o si son iguales
                if (ni == 0 || ni == dim - 1) continue;
                if (s[i][j] == s[ni][nj]) continue;

                // Calcular energías locales (condiciones periódicas en x)
                int hA = s[i-1][j] + s[i+1][j] + s[i][(j-1+dim)%dim] + s[i][(j+1)%dim];
                int hB = s[ni-1][nj] + s[ni+1][nj] + s[ni][(nj-1+dim)%dim] + s[ni][(nj+1)%dim];

                int sA = s[i][j];
                float dE = 2 * sA * (hA - hB) + 4.0f; // Variación de energía al intercambiar

                // Algoritmo de Metrópolis
                if (dE <= 0) {
                    s[i][j] = s[ni][nj];
                    s[ni][nj] = sA;
                } else {
                    float p = std::exp(-dE / T);
                    float r = static_cast<float>(std::rand()) / RAND_MAX;
                    if (r < p) {
                        s[i][j] = s[ni][nj];
                        s[ni][nj] = sA;
                    }
                }
            }
        }

        // ==========================================
        // 3. MEDIDA DEL OBSERVABLE (Magnetización por dominios)
        // ==========================================
        float suma_m_dom_total = 0;

        for (int k = 0; k < mcs_medida; k++) {
            // Un paso Monte Carlo completo de dinámica de Kawasaki
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

            // Tras barrer la red, calculamos la magnetización de las mitades
            float m_sup_actual = 0;
            float m_inf_actual = 0;
            
            for (int i = 0; i < dim / 2; i++) {
                for (int j = 0; j < dim; j++) m_sup_actual += s[i][j];
            }
            for (int i = dim / 2; i < dim; i++) {
                for (int j = 0; j < dim; j++) m_inf_actual += s[i][j];
            }

            int particulas_mitad = (dim / 2) * dim;
            m_sup_actual /= particulas_mitad;
            m_inf_actual /= particulas_mitad;

            // m_dom es la media de los valores absolutos
            float m_dom_actual = (std::abs(m_sup_actual) + std::abs(m_inf_actual)) / 2.0f;
            suma_m_dom_total += m_dom_actual;
        }

        // ==========================================
        // 4. GUARDAR MEDIA ESTADÍSTICA
        // ==========================================
        float m_dom_promedio = suma_m_dom_total / mcs_medida;
        data_out << T << "\t" << m_dom_promedio << "\n";
        
        std::cout << "Calculado T = " << T << " | m_dom = " << m_dom_promedio << std::endl;
    }

    data_out.close();
    std::cout << "\nSimulación completada para N = " << dim << std::endl;
    return 0;
}