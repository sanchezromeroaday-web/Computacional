#include <iostream>
#include <fstream>
#include <iomanip>
#include <complex>
#include <vector>
#include <cmath>

using namespace std;

// Definición de constantes según las diapositivas 
const double PI = 3.14159265358979323846;

// Alias para manejar números complejos de forma sencilla
using Complex = complex<double>;

int main() {
    // 1. Configuración de los archivos de salida
    // Usaremos "fonda.dat" para la animación (formato: x,y)
    ofstream out("fonda.dat");
    ofstream pot("pot.dat");
    ofstream norma("norm.dat");

    // Formato de alta precisión (15 decimales)
    out << fixed << setprecision(15);
    pot << fixed << setprecision(15);
    norma << fixed << setprecision(15);

    // 2. Parámetros iniciales del problema 
    int N = 1000;              // Número de puntos espacial (discretización)
    int n_ciclos = N / 10;     // Número de ciclos (k0) 
    int n_temp = 2000;         // Pasos de tiempo para la simulación
    double lam = 0.3;          // Intensidad del potencial (lambda) 

    // Cálculo de parámetros reescalados 
    double k0 = 2.0 * PI * double(n_ciclos) / double(N); // Momento inicial
    double s = 1.0 / (4.0 * k0 * k0);                    // Paso temporal (s tilde)

    // Vectores para el algoritmo
    vector<double> V(N, 0.0);
    vector<Complex> fonda(N, Complex(0.0, 0.0));
    vector<Complex> A(N, Complex(0.0, 0.0));
    vector<Complex> alpha(N, Complex(0.0, 0.0));
    vector<Complex> B(N, Complex(0.0, 0.0));
    vector<Complex> beta(N, Complex(0.0, 0.0));
    vector<Complex> chi(N, Complex(0.0, 0.0));

    // 3. Definición del Potencial V_j 
    // Barrera cuadrada en el intervalo [2N/5, 3N/5]
    for (int j = 0; j < N; j++) {
        if (j >= 2 * N / 5 && j <= 3 * N / 5) {
            V[j] = lam * k0 * k0;
        } else {
            V[j] = 0.0;
        }
        pot << j << "," << V[j] << "\n";
    }
    pot.close();

    // 4. Función de onda inicial: Paquete Gaussiano 
    double sum_norm = 0.0;
    for (int j = 0; j < N; j++) {
        // Condiciones de contorno: psi=0 en los extremos 
        if (j == 0 || j == N - 1) {
            fonda[j] = Complex(0.0, 0.0);
        } else {
            // Exponente de la gaussiana y fase de onda plana
            double exponente = -8.0 * pow(4.0 * j - N, 2) / double(N * N);
            fonda[j] = polar(exp(exponente), k0 * j);
            sum_norm += norm(fonda[j]); // norm() devuelve |psi|^2
        }
    }

    // Normalización inicial y volcado al archivo compatible con Python
    double factor_norm = sqrt(sum_norm);
    for (int j = 0; j < N; j++) {
        fonda[j] /= factor_norm;
        // FORMATO: x, y (requerido por el script de animación)
        out << j << "," << abs(fonda[j]) << "\n";
    }
    out << "\n"; // Doble salto de línea para separar frames en el script

    // 5. Coeficientes estáticos del Algoritmo de Thomas 
    for (int j = 0; j < N; j++) {
        A[j] = Complex(-2.0 - V[j], 2.0 / s);
    }

    // Cálculo de los coeficientes alpha (no dependen del tiempo) 
    alpha[N - 1] = Complex(0.0, 0.0);
    for (int j = N - 2; j > 0; j--) {
        alpha[j - 1] = -1.0 / (A[j] + alpha[j]);
    }

    // 6. Bucle de Evolución Temporal (Crank-Nicolson) 
    for (int t = 0; t < n_temp; t++) {
        double current_norm = 0.0;

        // Calcular vectores B y beta (pasada hacia atrás) 
        for (int j = 0; j < N; j++) {
            B[j] = (Complex(0.0, 4.0) * fonda[j]) / s;
        }

        beta[N - 1] = Complex(0.0, 0.0);
        for (int j = N - 2; j > 0; j--) {
            beta[j - 1] = (B[j] - beta[j]) / (A[j] + alpha[j]);
        }

        // Calcular vector chi (pasada hacia adelante) 
        chi[0] = Complex(0.0, 0.0);
        for (int j = 0; j < N - 1; j++) {
            chi[j + 1] = alpha[j] * chi[j] + beta[j];
        }

        // Actualizar la función de onda: Phi(n+1) = chi - Phi(n) 
        for (int j = 0; j < N; j++) {
            fonda[j] = chi[j] - fonda[j];
            current_norm += norm(fonda[j]);
        }

        // Escribir la norma (debería ser 1.0 siempre)
        norma << t << "\t" << sqrt(current_norm) << "\n";

        // Escribir los datos para la animación (cada frame separado por \n\n)
        for (int j = 0; j < N; j++) {
            out << j << "," << abs(fonda[j]) << "\n";
        }
        out << "\n"; 
    }

    out.close();
    norma.close();
    cout << "Simulacion completada. Datos guardados en fonda.dat" << endl;

    return 0;
}