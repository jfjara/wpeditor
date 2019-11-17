#ifndef WAYPOINTALTURA_H
#define WAYPOINTALTURA_H

#include "waypoint.h"
#include "constantes.h"
#include <QRectF>

class WayPointAltura
{
public:
    WayPointAltura();
    int posY = 0;
    WayPoint* wayPoint = Q_NULLPTR;
    QRectF cajaEnvolvente;

    int radioMarcaWP = RADIO_MARCA_WAYPOINT;
    int diametroMarcaWP = DIAMETRO_MARCA_WAYPOINT;
    bool verDistancia = false;
    int distanciaAcumulada = 0;

    void setWayPoint(WayPoint* wp, int height,int maximaAltura);
    void desplazarEjeY(int dy,int height, int maximaAltura);
    int getAltura(int dy,int height, int maximaAltura);
    void recalcularPosicionEnVisor(int height, int maximaAltura);
    void pintar(QPainter* painter, int posX, bool verAltura);

};

#endif // WAYPOINTALTURA_H
