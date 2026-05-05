#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace std;

int main() {
    int dim = 50;            
    int iter = 1000;      
    unsigned int seed = 12345;
    std::srand(seed);

    // Solo creamos el archivo para la magnetización
    ofstream data_mag("magnetizacion.txt");
    
    // Bucle de temperaturas (de 1.0 a 4.0)
    for (float T = 1.0; T <= 4.0; T += 0.1) {
        
        // * ESTADO INICIAL ORDENADO (+1) * //
        vector<vector<int>> s(dim, vector<int>(dim, 1));

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
        }

        // * CALCULAR MAGNETIZACIÓN ESPERADA AL FINALIZAR * //
        float M = 0;
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                M += s[i][j];
            }
        }
        M = std::abs(M) / (dim * dim);
        
        // Guardar T y M en el archivo
        data_mag << T << "\t" << M << endl;
    }

    data_mag.close();
   
    return 0;
}