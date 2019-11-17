#include "medidorescala.h"

MedidorEscala::MedidorEscala()
{

}

void MedidorEscala::pintar(QPainter* painter, int widthVisor, int heigthVisor)
{
    int distancia = 100;
    int pixeles = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, distancia);

    if (pixeles < 60) {
        distancia = 200;
        pixeles = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, distancia);
    }

    if (pixeles < 60 && distancia == 200) {
        distancia = 1000;
        pixeles = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, distancia);
    }

    painter->save();
    painter->setPen(QPen(QColor(0xFF, 0xFF, 0xFF), 4));
    painter->setBrush(QBrush(QColor(0xFF, 0xFF, 0xFF, 0xFF)));
    QPointF puntoInicial(widthVisor - pixeles - 30, heigthVisor - 40);
    QPointF puntoFinal(widthVisor - 30, heigthVisor - 40);
    QLineF linea(puntoInicial, puntoFinal);
    painter->drawLine(linea);

    QLineF extremoIzquierdo(puntoInicial, QPointF(widthVisor - pixeles - 30, heigthVisor - 50));
    QLineF extremoDerecho(puntoFinal, QPointF(widthVisor - 30, heigthVisor - 50));
    painter->drawLine(extremoIzquierdo);
    painter->drawLine(extremoDerecho);

    QRectF caja = QRectF(linea.center().x() - 15, linea.center().y() - 12, 50, 10);
    painter->setPen(QPen(QColor(0xFF, 0xFF, 0xFF), 2));
    painter->setFont(QFont("Arial", 7, QFont::Bold));
    painter->drawText(caja, QString::number(distancia) + QString(" m."),  Qt::AlignLeft | Qt::AlignVCenter);

    painter->restore();
}
