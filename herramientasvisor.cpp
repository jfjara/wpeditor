#include "herramientasvisor.h"
#include <QDebug>

HerramientasVisor* HerramientasVisor::instancia = Q_NULLPTR;

HerramientasVisor::HerramientasVisor()
{
}

HerramientasVisor::~HerramientasVisor()
{
}

HerramientasVisor* HerramientasVisor::getInstancia()
{
    if (instancia == Q_NULLPTR) {
      instancia = new HerramientasVisor();
    }
    return instancia;
}

void HerramientasVisor::acumularDesplazamiento(int dx, int dy)
{
    this->dx += dx;
    this->dy += dy;    
}
