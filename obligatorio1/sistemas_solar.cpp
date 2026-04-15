#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// DEFINICIÓN DE CONSTANTES (PARÁMETROS)
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#define ARCHIVO_ENTRADA "condiciones_i.txt" 
#define ARCHIVO_SALIDA "planets_data.dat" 
#define ARCHIVO_ENERGIA "energia_momento.txt" 

#define MAX_CUERPOS 100
#define ITERACIONES 1000000
#define PASO_TIEMPO 0.001

#define MASA_SOLAR 1.989e30
#define UNIDAD_ASTRONOMICA 1.496e11
#define FACTOR_TIEMPO 5022004.955

// ==========================================
// ARRAYS PARA LAS PROPIEDADES FÍSICAS
// ==========================================
double masas[MAX_CUERPOS], pos_x[MAX_CUERPOS], pos_y[MAX_CUERPOS];
double vel_x[MAX_CUERPOS], vel_y[MAX_CUERPOS];
double acel_x[MAX_CUERPOS], acel_y[MAX_CUERPOS];
double w_x[MAX_CUERPOS], w_y[MAX_CUERPOS]; 

// Variables para periodos
double periodos[MAX_CUERPOS] = {0};        
double y_ant[MAX_CUERPOS] = {0};       
bool tiene_vuelta[MAX_CUERPOS] = {0}; 

int main() {
    int n = 0;
    float t = 0;
    long double E = 0, L = 0;

    // 1. LECTURA DE DATOS
    ifstream entrada(ARCHIVO_ENTRADA);
    if (!entrada) {
        cout << "Error al abrir " << ARCHIVO_ENTRADA << endl;
        return 1;
    }

    while(entrada >> pos_x[n] >> vel_y[n] >> masas[n]) {
        n++;
    }
    entrada.close();

    // 2. REESCALAMIENTO [cite: 148]
    for (int i = 0; i < n; i++) {
        pos_x[i] *= 1e6 * 1000 / UNIDAD_ASTRONOMICA;
        pos_y[i] = 0; 
        masas[i] *= 1e24 / MASA_SOLAR;
        vel_x[i] = 0; 
        vel_y[i] *= 1000 / UNIDAD_ASTRONOMICA * FACTOR_TIEMPO;
    }

    // 3. ARCHIVOS Y ACELERACIÓN INICIAL
    ofstream f_salida(ARCHIVO_SALIDA);
    ofstream f_energia(ARCHIVO_ENERGIA);

    // Escribir t=0
    for (int i = 0; i < n; i++) f_salida << pos_x[i] << "," << pos_y[i] << "\n";
    f_salida << "\n";

    // Cálculo inicial de aceleraciones según la ley de gravitación [cite: 105, 159]
    for (int i = 0; i < n; i++) {
        acel_x[i] = acel_y[i] = 0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                float r = sqrt(pow(pos_x[i]-pos_x[j], 2) + pow(pos_y[i]-pos_y[j], 2));
                acel_x[i] += -masas[j] * (pos_x[i] - pos_x[j]) / pow(r, 3);
            }
        }
    }

    // 4. ALGORITMO DE VERLET [cite: 117, 137]
    for (int k = 0; k < ITERACIONES; k++) {
        t += PASO_TIEMPO;
        E = 0; L = 0;

        // A) Posiciones
        for (int i = 0; i < n; i++) {
            y_ant[i] = pos_y[i];

            // Velocidad intermedia W 
            w_x[i] = vel_x[i] + (PASO_TIEMPO / 2.0) * acel_x[i];
            w_y[i] = vel_y[i] + (PASO_TIEMPO / 2.0) * acel_y[i];

            // Nueva posición r(t+h) 
            pos_x[i] += PASO_TIEMPO * vel_x[i] + (pow(PASO_TIEMPO, 2) / 2.0) * acel_x[i];
            pos_y[i] += PASO_TIEMPO * vel_y[i] + (pow(PASO_TIEMPO, 2) / 2.0) * acel_y[i];

            // NUEVO: ¡Solo guardamos 1 de cada 100 pasos!
            if (k % 100 == 0) {
                f_salida << pos_x[i] << "," << pos_y[i] << "\n";
            }
        }
        
        // NUEVO: El salto de línea también lo hacemos solo 1 de cada 100 veces
        if (k % 100 == 0) {
            f_salida << "\n";
        }

        // B) Aceleraciones nuevas a partir de r(t+h) [cite: 141]
        for (int i = 0; i < n; i++) {
            acel_x[i] = acel_y[i] = 0;
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    float r = sqrt(pow(pos_x[i]-pos_x[j], 2) + pow(pos_y[i]-pos_y[j], 2));
                    acel_x[i] += -masas[j] * (pos_x[i] - pos_x[j]) / pow(r, 3);
                    acel_y[i] += -masas[j] * (pos_y[i] - pos_y[j]) / pow(r, 3);
                }
            }
        }

        // C) Velocidades finales v(t+h) [cite: 127, 142]
        for (int i = 0; i < n; i++) {
            vel_x[i] = w_x[i] + (PASO_TIEMPO / 2.0) * acel_x[i];
            vel_y[i] = w_y[i] + (PASO_TIEMPO / 2.0) * acel_y[i];

            // Periodos [cite: 195]
            if (pos_x[i] > 0 && pos_y[i] * y_ant[i] < 0 && k > 100 && i != 0 && !tiene_vuelta[i]) {
                tiene_vuelta[i] = true;
                periodos[i] = k * PASO_TIEMPO * FACTOR_TIEMPO / 86400.0;
                cout << "Planeta " << i << ": Periodo de " << periodos[i] << " dias." << endl;
            }
        }

        // D) Energía y Momento [cite: 113, 130]
        for (int i = 0; i < n; i++) {
            L += masas[i] * (pos_x[i] * vel_y[i] - pos_y[i] * vel_x[i]);
            E += 0.5 * masas[i] * (pow(vel_x[i], 2) + pow(vel_y[i], 2));
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    float r = sqrt(pow(pos_x[i]-pos_x[j], 2) + pow(pos_y[i]-pos_y[j], 2));
                    E += -masas[i] * masas[j] / (2.0 * r);
                }
            }
        } 
        f_energia << fixed << setprecision(7) << t << "\t" << E << "\t" << L << "\n";
    }

    f_salida.close();
    f_energia.close();
    cout << "Simulacion terminada. Tiempo total: " << t * FACTOR_TIEMPO / 86400.0 << " dias terrestres." << endl;

    return 0;
}