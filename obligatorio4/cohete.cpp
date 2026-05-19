#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <array>
#include <iomanip>

using namespace std;

//? CONSTANTES UNIVERSALES Y FÍSICAS 
constexpr double PI = 3.14159265358979323846;
constexpr double G = 6.674e-11;

constexpr double MT = 5.9736e24;   // Masa Tierra (Kg)
constexpr double ML = 0.07349e24;  // Masa Luna (Kg)
constexpr double RT = 6.378160e6;  // Radio Tierra (m)
constexpr double RL = 1.7374e6;    // Radio Luna (m)

constexpr double DTL = 3.844e8;    // Radio órbita Tierra-Luna (m)
constexpr double OMEGA = 2.6617e-6; // Periodo órbita Luna (s)

constexpr double DELTA = 7.01474e-12; // Parámetro Delta
constexpr double MU = 0.0123025;      // Parámetro Mu

//? PARÁMETROS DE LA SIMULACIÓN
constexpr int ITER = 500000; // Iteraciones a realizar
constexpr double H_STEP = 1.0; // Paso de integración h

//? ECUACIONES DIFERENCIALES
// n = 0 (pr), n = 1 (pphi/r^2), n = 2 (dot_pr), n = 3 (dot_pphi)
double f(int n, const array<double, 4>& y, double t) {
    double r = y[0];
    double phi = y[1];
    double pr = y[2];
    double pphi = y[3];
    
    double rprima = sqrt(1.0 + r*r - 2.0*r*cos(phi - OMEGA*t));

    if (n == 0) return pr;
    if (n == 1) return pphi / (r*r);
    if (n == 2) return (pphi*pphi) / (r*r*r) - DELTA * (1.0 / (r*r) + (MU / (rprima*rprima*rprima)) * (r - cos(phi - OMEGA*t)));
    if (n == 3) return - (DELTA * MU * r / (rprima*rprima*rprima)) * sin(phi - OMEGA*t);

    return 0.0;
}

//? FUNCIÓN PARA CALCULAR LA CONSTANTE DEL MOVIMIENTO H'
double calcular_H_prima(const array<double, 4>& y, double t) {
    double r = y[0];
    double phi = y[1];
    double pr = y[2];
    double pphi = y[3];
    
    double rprima = sqrt(1.0 + r*r - 2.0*r*cos(phi - OMEGA*t));
    
    // Hamiltoniano H
    double H = 0.5 * (pr*pr + (pphi*pphi)/(r*r)) - (DELTA / r) - (DELTA * MU / rprima);
    
    // H' = H - omega * pphi
    return H - OMEGA * pphi;
}

int main() {
    //? VALORES INICIALES
 double theta = PI / 3.4; 
double v = 11000.0 / DTL;
    // Vector y(t) = {Radio(0), ángulo(1), momento radial(2), momento angular(3)}
    array<double, 4> y = {
        RT / DTL,
        PI / 2.0,
        v * cos(theta - PI / 2.0),
        (RT / DTL) * v * sin(theta - PI / 2.0)
    };

    // Arrays para los pasos de Runge-Kutta
    array<double, 4> k1, k2, k3, k4;
    array<double, 4> y_temp;

    //? ABRIMOS FICHERO DE SALIDA USANDO fstream
    ofstream out("data.dat");
    if (!out.is_open()) {
        cerr << "Error al abrir el archivo data.dat" << endl;
        return 1;
    }

    double t = 0.0;
    bool paso = false;

    // Guardamos el estado inicial (X_luna, Y_luna, X_cohete, Y_cohete, tiempo, H_prima)
    out << fixed << setprecision(8);
    out << 1.0 << "\t" << 0.0 << "\t" 
        << y[0]*cos(y[1]) << "\t" << y[0]*sin(y[1]) << "\t" 
        << t << "\t" << calcular_H_prima(y, t) << "\n";

    //! BUCLE PRINCIPAL (RK4)
    for (int k = 0; k < ITER; k++) {
        
        // Paso 1: k1
        for (int n = 0; n < 4; n++) {
            k1[n] = H_STEP * f(n, y, t);
        }

        // Paso 2: k2
        for (int n = 0; n < 4; n++) y_temp[n] = y[n] + k1[n] / 2.0;
        for (int n = 0; n < 4; n++) {
            k2[n] = H_STEP * f(n, y_temp, t + H_STEP / 2.0);
        }

        // Paso 3: k3
        for (int n = 0; n < 4; n++) y_temp[n] = y[n] + k2[n] / 2.0;
        for (int n = 0; n < 4; n++) {
            k3[n] = H_STEP * f(n, y_temp, t + H_STEP / 2.0);
        }

        // Paso 4: k4
        for (int n = 0; n < 4; n++) y_temp[n] = y[n] + k3[n];
        for (int n = 0; n < 4; n++) {
            k4[n] = H_STEP * f(n, y_temp, t + H_STEP);
        }

        // Actualizamos y[n] para t + h
        for (int n = 0; n < 4; n++) {
            y[n] += (1.0 / 6.0) * (k1[n] + 2.0 * k2[n] + 2.0 * k3[n] + k4[n]);
        }
        
        // Avanzamos el tiempo
        t += H_STEP;

        // Escribimos en el archivo: pos_luna_x, pos_luna_y, pos_cohete_x, pos_cohete_y, tiempo, H_prima
        out << cos(OMEGA*t) << "\t" << sin(OMEGA*t) << "\t"
            << y[0]*cos(y[1]) << "\t" << y[0]*sin(y[1]) << "\t"
            << t << "\t" << calcular_H_prima(y, t) << "\n";

        // Comprobamos la distancia mínima cohete-luna
        if (y[0]*cos(y[1]) >= cos(OMEGA*t) && !paso) {
            cout << "Angulo Lunar: " << atan2(sin(OMEGA*t), cos(OMEGA*t)) << endl;
            cout << "Angulo Cohete: " << y[1] << endl;
            cout << "Distancia minima cohete-luna: " 
                 << sqrt(1 + y[0]*y[0] - 2*y[0]*cos(y[1] - OMEGA*t)) * DTL / 1000.0 << " km" << endl;
            paso = true;
        }
    }
    
    out.close();
    cout << "Simulacion terminada. Datos guardados en data.dat" << endl;
    
    return 0;
}