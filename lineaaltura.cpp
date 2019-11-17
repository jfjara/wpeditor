#include "lineaaltura.h"
#include "visoralturaswidget.h"

LineaAltura::LineaAltura()
{
}

void LineaAltura::pintar(QPainter* painter, int w, int h, int maxAltura, bool pintarNombre)
{
    int py = h - (h * altura / maxAltura);
    painter->save();
    painter->setPen(QPen(color, 1, Qt::DashLine));
    painter->drawLine(VisorAlturasWidget::offsetX + 30, py + 30, VisorAlturasWidget::offsetX + 30 + w, py + 30);

    QRectF caja = QRectF(VisorAlturasWidget::offsetX + 2, py + 15, 28, 30);
    painter->setFont(QFont("Arial", 7, QFont::Bold));
    painter->drawText(caja, QString::number(altura) + "m",  Qt::AlignLeft | Qt::AlignVCenter);

    if (pintarNombre) {
        QRectF cajaNombre = QRectF(VisorAlturasWidget::offsetX + 32, py , 50, 30);
        painter->drawText(cajaNombre, nombre,  Qt::AlignLeft | Qt::AlignVCenter);
    }

    painter->restore();
}
