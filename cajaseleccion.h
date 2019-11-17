#ifndef CAJASELECCION_H
#define CAJASELECCION_H

#include <QRectF>
#include <QPainter>

class CajaSeleccion
{
public:
    CajaSeleccion();

    QRectF seleccion;
    bool mostrar = false;

    void pintar(QPainter* painter);
};

#endif // CAJASELECCION_H
