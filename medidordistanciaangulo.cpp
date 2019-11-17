#include "medidordistanciaangulo.h"
#include <QDebug>

MedidorDistanciaAngulo::MedidorDistanciaAngulo()
{
}

void MedidorDistanciaAngulo::marcar(QRectF caja)
{
    QLineF lineaSuperior(caja.topLeft(), caja.topRight());
    QLineF lineaInferior(caja.bottomLeft(), caja.bottomRight());
    QLineF lineaDerecha(caja.topRight(), caja.bottomRight());
    QLineF lineaIzquierda(caja.topRight(), caja.bottomRight());

    puntoAnclaje = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud1, latitud1);
    puntoFinal = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud2, latitud2);

    puntoAnclaje.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoAnclaje.x());
    puntoAnclaje.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoAnclaje.y());
    puntoFinal.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoFinal.x());
    puntoFinal.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoFinal.y());
    QLineF linea(puntoAnclaje, puntoFinal);

    QPointF punto;
    if (linea.intersect(lineaSuperior, &punto) == QLineF::BoundedIntersection) {
        seleccionado = true;
        return;
    }
    if (linea.intersect(lineaInferior, &punto) == QLineF::BoundedIntersection) {
        seleccionado = true;
        return;
    }
    if (linea.intersect(lineaDerecha, &punto) == QLineF::BoundedIntersection) {
        seleccionado = true;
        return;
    }
    if (linea.intersect(lineaIzquierda, &punto) == QLineF::BoundedIntersection) {
        seleccionado = true;
        return;
    }

    if (caja.contains(puntoAnclaje)) {
        seleccionado = true;
        return;
    }

    if (caja.contains((puntoFinal))) {
        seleccionado = true;
        return;
    }

}

void MedidorDistanciaAngulo::marcar(QPointF punto)
{

    puntoAnclaje = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud1, latitud1);
    puntoFinal = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud2, latitud2);

    puntoAnclaje.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoAnclaje.x());
    puntoAnclaje.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoAnclaje.y());
    puntoFinal.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoFinal.x());
    puntoFinal.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoFinal.y());

    QLineF linea(puntoAnclaje, puntoFinal);

    QLineF polygonLine;
    QPolygonF poligonoCaja;
    poligonoCaja << QPointF(punto.x() - 5, punto.y() - 5);
    poligonoCaja << QPointF(punto.x() + 5, punto.y() - 5);
    poligonoCaja << QPointF(punto.x() + 5, punto.y() + 5);
    poligonoCaja << QPointF(punto.x() - 5, punto.y() + 5);

    for (int i = 0; i < poligonoCaja.size() - 1; i++) {

        polygonLine = QLineF(poligonoCaja[i], poligonoCaja[i+1]);

        if (linea.intersect(polygonLine, &punto) == QLineF::BoundedIntersection) {
            seleccionado = true;
            return;
        }
    }
    seleccionado = false;
}

void MedidorDistanciaAngulo::pintar(QPainter* painter)
{
    puntoAnclaje = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud1, latitud1);
    puntoFinal = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud2, latitud2);

    puntoAnclaje.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoAnclaje.x());
    puntoAnclaje.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoAnclaje.y());
    puntoFinal.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoFinal.x());
    puntoFinal.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoFinal.y());

    painter->save();
    QLineF linea(puntoAnclaje, puntoFinal);
    if (seleccionado) {
        painter->setPen(QPen(QColor(0xFF, 0xFF, 0x00), 4));
    } else {
        painter->setPen(QPen(QColor(0xFF, 0x00, 0xFF), 4));
    }
    painter->setBrush(QBrush(QColor(0xFF, 0x00, 0xFF)));
    painter->drawLine(linea);

    int size = 10;
    qreal Pi = 3.1415;
    double angle = ::acos(linea.dx() / linea.length());
    if (linea.dy() >= 0)
           angle = (Pi * 2) - angle;

    QPointF arrowP11 = linea.p2() - QPointF(sin(angle + Pi / 3) * size,
                                   cos(angle + Pi / 3) * size);
    QPointF arrowP12 = linea.p2() - QPointF(sin(angle + Pi - Pi / 3) * size,
                                   cos(angle + Pi - Pi / 3) * size);

    QPolygonF flecha1;
    flecha1 << linea.p2() << arrowP11 << arrowP12;
    QPointF arrowP21 = linea.p1() + QPointF(sin(angle + Pi / 3) * size,
                                   cos(angle + Pi / 3) * size);
    QPointF arrowP22 = linea.p1() + QPointF(sin(angle + Pi - Pi / 3) * size,
                                   cos(angle + Pi - Pi / 3) * size);

    QPolygonF flecha2;
    flecha2 << linea.p1() << arrowP21 << arrowP22;

    painter->drawPolygon(flecha1);
    painter->drawPolygon(flecha2);

    QRectF caja(linea.center().x() - 70, linea.center().y() - 20, 140, 20);
    QPointF coordenada1 = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, puntoAnclaje.x(), puntoAnclaje.y());
    QPointF coordenada2 = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, puntoFinal.x(), puntoFinal.y());
    double lng1 = coordenada1.x();
    double lng2 = coordenada2.x();
    double lat1 = coordenada1.y();
    double lat2 = coordenada2.y();
    double distancia = HerramientaConversorCoordenadas::getInstancia()->getDistanciaEntreCoordenadas(Enumerados::LatLng, lng1, lat1, lng2, lat2);

    painter->setPen(QPen(QColor(0xBB, 0xBB, 0xBB), 1));
    painter->setBrush(QBrush(QColor(0xBB, 0xBB, 0xBB, 0xBB)));
    painter->drawRect(caja);
    painter->setPen(QPen(QColor(0x00, 0x00, 0x00), 4));
    painter->setFont(QFont("Arial", 10, QFont::Bold));

    double angulo = HerramientaConversorCoordenadas::getInstancia()->convertirAngulo(linea.angle());
    painter->drawText(caja, QString::number(distancia, 'f', 1) + "m @ " + QString::number((int)angulo) + "º", Qt::AlignHCenter | Qt::AlignVCenter);
    painter->restore();
}
