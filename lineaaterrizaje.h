#ifndef LINEAATERRIZAJE_H
#define LINEAATERRIZAJE_H

#include <QPainter>
#include <QPointF>
//#include "waypoint.h"
#include "herramientaconversorcoordenadas.h"

class LineaAterrizaje
{
public:
    //LineaAterrizaje(WayPoint* wp);
    LineaAterrizaje();

    //WayPoint* wp = Q_NULLPTR;
    bool aterrizajeEnU = true;
    bool aterrizajeADerecha = true;
    bool mostrada = false;
    double base = 100; //en metros
    double recorrido = 300; //en metros
    //double angulo = 10;

    void pintar(QPainter* painter, QPointF punto, double angulo,bool aterrizajeADerecha);

private:
    void pintarAterrizajeEnUDerecha(QPainter* painter,QPointF punto, double angulo);
    void pintarAterrizajeEnUIzquierda(QPainter* painter, QPointF punto, double angulo);
    void pintarFlechaDireccion(QPainter* painter,QLineF linea);

};

#endif // LINEAATERRIZAJE_H
