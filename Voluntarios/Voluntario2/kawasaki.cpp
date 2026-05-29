#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <algorithm> // Necesario para std::shuffle
#include <random>    // Necesario para el generador aleatorio

using namespace std;

int main() {
    int dim = 128;       // Aumentamos a 32x32 como sugiere el PDF
    int iter = 10000;     // Pasos Monte Carlo
    float T = 4.0;      // Temperatura
    unsigned int seed = 1649176165;

    std::srand(seed);
    std::mt19937 g(seed); // Generador para mezclar bien los espines iniciales

    ofstream data_anim("ising_kawasaki.dat");

    // * 1. CONFIGURACIÓN INICIAL (Magnetización Nula y Bordes fijos) * //
    vector<vector<int>> s(dim, vector<int>(dim));
    
    // Para asegurar magnetización nula exacta en el centro, creamos un vector
    // con la mitad de espines a +1 y la otra mitad a -1, y lo barajamos.
    vector<int> espines_activos;
    int num_activos = (dim - 2) * dim;
    for (int k = 0; k < num_activos; k++) {
        if (k < num_activos / 2) espines_activos.push_back(1);
        else espines_activos.push_back(-1);
    }
    std::shuffle(espines_activos.begin(), espines_activos.end(), g);

    // Asignamos a la matriz
    int c = 0;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (i == 0) {
                s[i][j] = -1; // Borde superior fijo (Figura 1)
            } else if (i == dim - 1) {
                s[i][j] = 1;  // Borde inferior fijo (Figura 1)
            } else {
                s[i][j] = espines_activos[c++]; // Región interior
            }
            
            data_anim << s[i][j];
            if (j < dim - 1) data_anim << ",";
        }
        data_anim << "\n";
    }
    data_anim << "\n"; 

    // * 2. ALGORITMO MONTE CARLO (Dinámica de Kawasaki) * //
    for (int k = 0; k < iter; k++) {
        for (int l = 0; l < dim * dim; l++) {
            // Seleccionamos espín aleatorio SOLO en la región activa (filas 1 a dim-2)
            int i = std::rand() % (dim - 2) + 1;
            int j = std::rand() % dim;

            // Elegimos un vecino al azar (0: arriba, 1: abajo, 2: izq, 3: der)
            int dir = std::rand() % 4;
            int ni = i, nj = j;

            if (dir == 0) ni = i - 1;
            else if (dir == 1) ni = i + 1;
            else if (dir == 2) nj = (j - 1 + dim) % dim; // Condición periódica en X
            else if (dir == 3) nj = (j + 1) % dim;       // Condición periódica en X

            // Si el vecino que hemos elegido es uno de los bordes fijos, no podemos intercambiar
            if (ni == 0 || ni == dim - 1) continue;

            // Si los espines son iguales, intercambiarlos no hace nada, pasamos al siguiente
            if (s[i][j] == s[ni][nj]) continue;

            // Calculamos hA y hB (la suma de los 4 vecinos de cada posición)
            int hA = s[i-1][j] + s[i+1][j] + s[i][(j-1+dim)%dim] + s[i][(j+1)%dim];
            int hB = s[ni-1][nj] + s[ni+1][nj] + s[ni][(nj-1+dim)%dim] + s[ni][(nj+1)%dim];

            // * 3. APLICAMOS LA FÓRMULA DE ENERGÍA DEDUCIDA * //
            int sA = s[i][j];
            float dE = 2 * sA * (hA - hB) + 4.0f;

            // * 4. CONDICIÓN DE METROPOLIS PARA EL INTERCAMBIO * //
            if (dE <= 0) {
                // Aceptamos el intercambio directamente
                s[i][j] = s[ni][nj];
                s[ni][nj] = sA;
            } else {
                float p = std::exp(-dE / T);
                float r = static_cast<float>(std::rand()) / RAND_MAX;
                if (r < p) {
                    // Aceptamos el intercambio por probabilidad
                    s[i][j] = s[ni][nj];
                    s[ni][nj] = sA;
                }
            }
        }

        // GUARDAMOS EL FOTOGRAMA
        for (int i = 0; i < dim; i++) {
            for (int j = 0; j < dim; j++) {
                data_anim << s[i][j];
                if (j < dim - 1) data_anim << ",";
            }
            data_anim << "\n"; 
        }
        if (k < iter - 1) data_anim << "\n";
    }

    data_anim.close();
    return 0;
}