#ifndef MEDIDORESCALA_H
#define MEDIDORESCALA_H

#include <QPainter>
#include <QPointF>
#include <QLineF>
#include "herramientaconversorcoordenadas.h"

class MedidorEscala
{
public:
    MedidorEscala();

    void pintar(QPainter* painter, int w, int h);
};

#endif // MEDIDORESCALA_H
