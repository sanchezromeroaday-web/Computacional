#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>

// ==========================================
// 1. CONSTANTES REESCALADAS Y PARAMETROS
// ==========================================
const double G = 1.0;
const double M_BH = 4100000.0;       // Masa del agujero negro central (Sagitario A*)
const double M_SISTEMA = 1.0;        // Masa de cada sistema solar
const double RADIO_BH = 50.0;        // Radio de absorción del agujero negro
const double RADIO_GALAXIA = 8000.0; // Radio máximo de la distribución inicial
const double CUTOFF_GRAV = 200.0;    // Distancia máxima para calcular gravedad entre sistemas
const double RADIO_COLISION = 2.0;   // Radio ficticio de interacción para choques elásticos

// Estructuras de datos
struct Vector2D {
    double x, y;
};

struct SistemaSolar {
    Vector2D pos;
    Vector2D vel;
    Vector2D a;
    bool activo;
};

// ==========================================
// 2. FUNCIONES MATEMÁTICAS AUXILIARES
// ==========================================

// Genera un número aleatorio decimal entre min y max
double numAleatorio(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

// Inicializa un sistema solar en una órbita circular cerrada a una distancia r
void generarOrbitaCerrada(SistemaSolar& sis, double radio) {
    double theta = numAleatorio(0, 2 * M_PI);
    
    // Posición en coordenadas cartesianas
    sis.pos.x = radio * std::cos(theta);
    sis.pos.y = radio * std::sin(theta);
    
    // Velocidad: factor aleatorio para dar excentricidad y permitir flujo de masa
    double v_mag = std::sqrt(G * M_BH / radio) * numAleatorio(0.2, 1.1);
    
    sis.vel.x = -v_mag * std::sin(theta);
    sis.vel.y = v_mag * std::cos(theta);
    
    sis.a = {0.0, 0.0};
    sis.activo = true;
}

// ==========================================
// 3. MOTOR FÍSICO Y CONDICIONES DE CONTORNO
// ==========================================

void calcularAceleraciones(std::vector<SistemaSolar>& galaxia) {
    int N = galaxia.size();
    
    for (int i = 0; i < N; i++) {
        galaxia[i].a.x = 0.0;
        galaxia[i].a.y = 0.0;
    }

    for (int i = 0; i < N; i++) {
        if (!galaxia[i].activo) continue;

        // A. Fuerza del Agujero Negro
        double dx_bh = -galaxia[i].pos.x;
        double dy_bh = -galaxia[i].pos.y;
        double dist2_bh = dx_bh*dx_bh + dy_bh*dy_bh;
        double dist_bh = std::sqrt(dist2_bh);
        
        double factor_bh = (G * M_BH) / (dist_bh * dist2_bh); 
        galaxia[i].a.x += factor_bh * dx_bh;
        galaxia[i].a.y += factor_bh * dy_bh;

        // B. Fuerza del resto de Sistemas (Optimizada con CUTOFF)
        for (int j = i + 1; j < N; j++) {
            if (!galaxia[j].activo) continue;

            double dx = galaxia[j].pos.x - galaxia[i].pos.x;
            double dy = galaxia[j].pos.y - galaxia[i].pos.y;
            double dist2 = dx*dx + dy*dy;
            double dist = std::sqrt(dist2);

            if (dist < CUTOFF_GRAV && dist > 0.01) {
                double factor_sis = (G * M_SISTEMA) / (dist * dist2);
                galaxia[i].a.x += factor_sis * dx;
                galaxia[i].a.y += factor_sis * dy;
                galaxia[j].a.x -= factor_sis * dx;
                galaxia[j].a.y -= factor_sis * dy;
            }
        }
    }
}

void resolverColisionesElasticas(std::vector<SistemaSolar>& galaxia) {
    int N = galaxia.size();
    for (int i = 0; i < N; i++) {
        if (!galaxia[i].activo) continue;
        for (int j = i + 1; j < N; j++) {
            if (!galaxia[j].activo) continue;

            double dx = galaxia[j].pos.x - galaxia[i].pos.x;
            double dy = galaxia[j].pos.y - galaxia[i].pos.y;
            double dist2 = dx*dx + dy*dy;

            if (dist2 < (2 * RADIO_COLISION) * (2 * RADIO_COLISION)) {
                double dist = std::sqrt(dist2);
                
                double nx = dx / dist;
                double ny = dy / dist;

                double dvx = galaxia[i].vel.x - galaxia[j].vel.x;
                double dvy = galaxia[i].vel.y - galaxia[j].vel.y;

                double v_normal = dvx * nx + dvy * ny;

                if (v_normal > 0) continue;

                galaxia[i].vel.x -= v_normal * nx;
                galaxia[i].vel.y -= v_normal * ny;
                galaxia[j].vel.x += v_normal * nx;
                galaxia[j].vel.y += v_normal * ny;
            }
        }
    }
}

void absorberYRegenerar(std::vector<SistemaSolar>& galaxia, int& absorbidos_totales) {
    for (auto& sis : galaxia) {
        if (!sis.activo) continue;

        double dist2 = sis.pos.x*sis.pos.x + sis.pos.y*sis.pos.y;
        
        // 1. Lo que traga el agujero negro
        if (dist2 < RADIO_BH * RADIO_BH) {
            sis.activo = false;
            absorbidos_totales++;
            // NOTA: Para el análisis extra de "inanición", comenta la línea de abajo
            generarOrbitaCerrada(sis, RADIO_GALAXIA); 
        }
        // 2. El muro exterior para evitar fugas
        else if (dist2 > (RADIO_GALAXIA * 1.5) * (RADIO_GALAXIA * 1.5)) { 
            sis.activo = false;
            // NOTA: Para el análisis extra de "inanición", comenta la línea de abajo
            generarOrbitaCerrada(sis, RADIO_GALAXIA);
        }
    }
}

// Función para calcular los observables macroscópicos específicos (medias)
void calcularObservables(const std::vector<SistemaSolar>& galaxia, double& K, double& U, double& Lz) {
    K = 0.0;
    U = 0.0;
    Lz = 0.0;
    int n_activos = 0;
    
    for (const auto& sis : galaxia) {
        if (!sis.activo) continue;
        
        n_activos++;
        double r = std::sqrt(sis.pos.x * sis.pos.x + sis.pos.y * sis.pos.y);
        double v2 = sis.vel.x * sis.vel.x + sis.vel.y * sis.vel.y;
        
        K += 0.5 * M_SISTEMA * v2;
        U -= (G * M_BH * M_SISTEMA) / r;
        Lz += M_SISTEMA * (sis.pos.x * sis.vel.y - sis.pos.y * sis.vel.x);
    }

    if (n_activos > 0) {
        K /= n_activos;
        U /= n_activos;
        Lz /= n_activos;
    }
}

// ==========================================
// 4. INTEGRACIÓN TEMPORAL (VERLET)
// ==========================================

void pasoVerlet(std::vector<SistemaSolar>& galaxia, double h) {
    int N = galaxia.size();
    std::vector<Vector2D> w(N);

    for (int i = 0; i < N; i++) {
        if (!galaxia[i].activo) continue;
        galaxia[i].pos.x += h * galaxia[i].vel.x + 0.5 * h * h * galaxia[i].a.x;
        galaxia[i].pos.y += h * galaxia[i].vel.y + 0.5 * h * h * galaxia[i].a.y;
        w[i].x = galaxia[i].vel.x + 0.5 * h * galaxia[i].a.x;
        w[i].y = galaxia[i].vel.y + 0.5 * h * galaxia[i].a.y;
    }

    calcularAceleraciones(galaxia);

    for (int i = 0; i < N; i++) {
        if (!galaxia[i].activo) continue;
        galaxia[i].vel.x = w[i].x + 0.5 * h * galaxia[i].a.x;
        galaxia[i].vel.y = w[i].y + 0.5 * h * galaxia[i].a.y;
    }
}

void guardarDatos(const std::vector<SistemaSolar>& galaxia, std::ofstream& archivo) {
    for (const auto& sis : galaxia) {
        if (sis.activo) {
            archivo << sis.pos.x << " " << sis.pos.y << "\n";
        }
    }
    archivo << "\n\n"; 
}

// ==========================================
// 5. BLOQUE PRINCIPAL
// ==========================================

int main() {
    std::srand(std::time(nullptr));

    // Parámetros de la simulación
    int num_sistemas = 1000;
    double h = 0.005; 
    int num_pasos = 25000; 
    int pasos_por_frame = 50; 
    int absorbidos_totales = 0;

    std::vector<SistemaSolar> galaxia(num_sistemas);
    
    // Apertura de archivos en bucle
    std::ofstream archivo_salida("datos_galaxia.dat");
    std::ofstream archivo_energias("energias_galaxia.dat"); 
    std::ofstream archivo_flujo("flujo_masa.dat"); 

    std::cout << "Inicializando galaxia..." << std::endl;
    for (int i = 0; i < num_sistemas; i++) {
        double radio_inicial = numAleatorio(RADIO_BH * 1.1, RADIO_GALAXIA / 1.5);
        generarOrbitaCerrada(galaxia[i], radio_inicial);
    }

    calcularAceleraciones(galaxia);

    std::cout << "Comenzando simulacion..." << std::endl;
    for (int t = 0; t < num_pasos; t++) {
        
        pasoVerlet(galaxia, h);
        resolverColisionesElasticas(galaxia);
        absorberYRegenerar(galaxia, absorbidos_totales);

        if (t % pasos_por_frame == 0) {
            guardarDatos(galaxia, archivo_salida);
            
            // Termodinámica
            double K, U, Lz;
            calcularObservables(galaxia, K, U, Lz);
            double E_total = K + U;
            double virial = 2.0 * K + U;
            archivo_energias << (t * h) << " " << K << " " << U << " " << E_total << " " << Lz << " " << virial << "\n";
            
            // Flujo de masa (La línea que te faltaba antes)
            archivo_flujo << (t * h) << " " << absorbidos_totales << "\n";

            if (t % (num_pasos/10) == 0) std::cout << "Completado: " << (t*100)/num_pasos << "%" << std::endl;
        }
    }

    // Cierre de archivos en bucle
    archivo_salida.close();
    archivo_energias.close(); 
    archivo_flujo.close(); 

    double tiempo_total = num_pasos * h;
    double flujo_medio = absorbidos_totales / tiempo_total;

    std::cout << "Simulacion terminada." << std::endl;
    std::cout << "Sistemas totales absorbidos: " << absorbidos_totales << std::endl;
    std::cout << "FLUJO MEDIO DE MASA ABSORBIDO: " << flujo_medio << " masas solares / unidad de tiempo" << std::endl;

    // ==========================================
    // EXTRA: FOTOS FINALES (ROTACIÓN Y MAPA)
    // ==========================================
    std::ofstream archivo_vel("curva_rotacion.dat");
    std::ofstream archivo_mapa("mapa_final.dat");
    
    for (const auto& sis : galaxia) {
        if (sis.activo) {
            double r = std::sqrt(sis.pos.x * sis.pos.x + sis.pos.y * sis.pos.y);
            double v = std::sqrt(sis.vel.x * sis.vel.x + sis.vel.y * sis.vel.y);
            
            archivo_vel << r << " " << v << "\n";
            archivo_mapa << sis.pos.x << " " << sis.pos.y << " " << v << "\n";
        }
    }
    
    archivo_vel.close();
    archivo_mapa.close();
    
    std::cout << "Datos finales guardados para curvas y mapas." << std::endl;
    
    return 0;
}