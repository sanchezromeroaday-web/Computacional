
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace std;

int main() {
    int dim = 20;            
    int iter = 1000000;     //  10^6 pasos Monte Carlo
    unsigned int seed = 12345;
    std::srand(seed);

    ofstream data_mag("magnetizacion_data.dat"); //
    
    // Bucle de temperaturas (de 1.0 a 4.0)
    for (float T = 1.0; T <= 4.0; T += 0.1) {
        
        // * ESTADO INICIAL ORDENADO (+1) * //
        vector<vector<int>> s(dim, vector<int>(dim, 1));

        float M_acumulado = 0; // Para promediar al final
        int pasos_medicion = 0;

        // * EVOLUCIÓN: PASOS MONTE CARLO * //
        for (int k = 0; k < iter; k++) {
            for (int l = 0; l < dim * dim; l++) {
                int i = std::rand() % dim;
                int j = std::rand() % dim;

                int up = (i - 1 + dim) % dim;
                int down = (i + 1) % dim;
                int left = (j - 1 + dim) % dim;
                int right = (j + 1) % dim;

                int E = 2 * s[i][j] * (s[down][j] + s[up][j] + s[i][right] + s[i][left]);

                float p = std::exp(-E / T);
                if (p > 1.0f) p = 1.0f;
                
                float r = static_cast<float>(std::rand()) / RAND_MAX;

                if (r < p) {
                    s[i][j] *= -1;
                }
            }

            //  Empezamos a medir M solo en el último 10% de los pasos
            if (k >= iter - 1000) {
                float M_paso = 0;
                for (int i = 0; i < dim; i++) {
                    for (int j = 0; j < dim; j++) {
                        M_paso += s[i][j];
                    }
                }
                M_acumulado += std::abs(M_paso) / (dim * dim);
                pasos_medicion++;
            }
        }

        // Promedio final de la magnetización a esta temperatura T
        float M_final = M_acumulado / pasos_medicion;
        
        // Guardar T y M en el archivo
        data_mag << T << "\t" << M_final << endl;
        cout << "Calculada T = " << T << " | M = " << M_final << endl; // Para ver el progreso
    }

    data_mag.close();
    return 0;
}
