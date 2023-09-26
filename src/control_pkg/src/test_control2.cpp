#include <iostream>
#include <cmath>
using namespace std;

#include <memory>
#include <cinttypes>
#include <chrono>
#include <functional>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

int main()
{
    float tf = 30; // tiempo de simulacion
    float ts = 0.1; // tiempo de muestreo
    int N = (int)((tf + ts) / ts); // cantidad de muestras

    float* hx = new float[N+1]; // asignar memoria
    float* hy = new float[N+1];
    float* phi = new float[N+1];

    hx[0] = 0;  // Posicion inicial en el eje x en metros [m]
    hy[0] = 0;  // Posicion inicial en el eje y en metros [m]
    phi[0] = 0*(M_PI/180); // Orientacion inicial en radianes [rad]

    // POSICION DESEADA
    float hxd = 1;
    float hyd = 1;
    float phid = M_PI/2;

    // VELOCIDADES DE REFERENCIA
    float* uRef = new float[N];  // Velocidad lineal en metros/segundos [m/s]
    float* wRef = new float[N]; // Velocidad angular en radianes/segundos [rad/s]

    // ERRORES
    float* l = new float[N];
    float* rho = new float[N];
    float* thetha = new float[N];

    // BUCLE
    for (int k = 0; k < N; k++)
    {
        // CONTROL

        // Errores
        l[k] = sqrt(pow(hxd-hx[k], 2) + pow(hyd-hy[k], 2));
        rho[k] = atan2(hyd-hy[k], hxd-hx[k])-phi[k];
        thetha[k] = atan2(hyd-hy[k], hxd-hx[k])-phid;

        // Parametros de control
        float K1 = 0.2;
        float K2 = 0.2;

        // Ley de control
        uRef[k] = K1*cos(rho[k])*l[k];
        wRef[k] = K2*rho[k]+(K1/rho[k])*cos(rho[k])*sin(rho[k])*(rho[k]+thetha[k]);

        // APLICAR ACCIONES DE CONTROL

        // Integral numerica
        phi[k+1] = phi[k] + ts*wRef[k];

        // Modelo cinemático
        float hxp = uRef[k]*cos(phi[k+1]);
        float hyp = uRef[k]*sin(phi[k+1]);

        // Integral numerica
        hx[k+1] = hx[k] + ts*hxp;
        hy[k+1] = hy[k] + ts*hyp;


        std::cout << "Error L = " << l[k] << "Error rho = " << rho[k] << "Error thetha = " <<thetha[k]  << "X = " << hx[k+1] << " Y =" << hy[k+1] << std::endl;
    }

    // Libera la memoria
    delete[] hx;
    delete[] hy;
    delete[] phi;
    delete[] uRef;
    delete[] wRef;
    delete[] l;
    delete[] rho;
    delete[] thetha;

    return 0;
}