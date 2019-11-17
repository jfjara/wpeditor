#ifndef LINEAALTURA_H
#define LINEAALTURA_H

#include <QString>
#include <QColor>
#include <QPointF>
#include <QPainter>

class LineaAltura
{
public:
    LineaAltura();

    QString nombre;
    QColor color;
    QPointF inicio;
    QPointF fin;
    int altura;
    int id;

    void pintar(QPainter* painter, int w, int h, int maxAltura, bool pintarNombre);
};

#endif // LINEAALTURA_H
