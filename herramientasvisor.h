#ifndef HERRAMIENTASVISOR_H
#define HERRAMIENTASVISOR_H

#include <QObject>

class HerramientasVisor
{
private:
    HerramientasVisor();
    static HerramientasVisor* instancia;


public:
    ~HerramientasVisor();

    double factorEscalado = 1.0;
    int operacionEscalado = 1;  //1 aumentar, -1 disminuir
    int dx = 0;
    int dy = 0;
    int dzx = 0;
    int dzy = 0;


    static HerramientasVisor* getInstancia();
    void acumularDesplazamiento(int dx, int dy);
    void setDesplazamientoZoom(int dzx, int dzy) {
        this->dzx += dzx;
        this->dzy += dzy;
    }

    int getDesplazamientoX() { return dx + dzx; }
    int getDesplazamientoY() { return dy + dzy; }



};

#endif // HERRAMIENTASVISOR_H
