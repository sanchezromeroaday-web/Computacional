#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace std;

int main() {
    int dim = 20;       
    int iter = 500;     
    float T = 1.5;     
    unsigned int seed = 1649176165;

    std::srand(seed);

    ofstream data_anim("ising_data.dat");

    // * CONFIGURACIÓN INICIAL (Fotograma 0) * //
    vector<vector<int>> s(dim, vector<int>(dim));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            s[i][j] = ((std::rand() % 2) == 0) ? -1 : 1; 
            data_anim << s[i][j];
            if (j < dim - 1) data_anim << ",";
        }
        data_anim << "\n";
    }
    data_anim << "\n"; 

    // * ALGORITMO MONTE CARLO * //
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

            if (r < p) s[i][j] *= -1; 
        }

        // GUARDAMOS EL FOTOGRAMA
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                data_anim << s[i][j];
                if (j < dim - 1) data_anim << ",";
            }
            data_anim << "\n"; 
        }
        // Salto de línea separador (excepto en el último fotograma)
        if (k < iter - 1) data_anim << "\n";
    }

    data_anim.close();
    return 0;
}