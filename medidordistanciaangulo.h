#ifndef MEDIDORDISTANCIAANGULO_H
#define MEDIDORDISTANCIAANGULO_H

#include <QPainter>
#include <QPointF>
#include "herramientaconversorcoordenadas.h"


class MedidorDistanciaAngulo
{
public:
    MedidorDistanciaAngulo();


    double latitud1, latitud2;
    double longitud1, longitud2;

    QPointF puntoAnclaje;
    QPointF puntoFinal;
    bool finalizado = false;
    bool ver = false;
    bool seleccionado = false;

    void marcar(QRectF caja);
    void marcar(QPointF punto);
    void pintar(QPainter* painter);
};

#endif // MEDIDORDISTANCIAANGULO_H
