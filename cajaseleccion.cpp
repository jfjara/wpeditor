#include "cajaseleccion.h"

CajaSeleccion::CajaSeleccion()
{

}

void CajaSeleccion::pintar(QPainter* painter)
{
    if (!mostrar) {
        return;
    }
    painter->save();
    QPen pen(QColor(0xFF, 0xFF, 0x00), 1);
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(0xFF, 0xFF, 0x00, 0x11)));
    painter->drawRect(seleccion);
    painter->restore();
}
