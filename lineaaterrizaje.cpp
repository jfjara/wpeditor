#include "lineaaterrizaje.h"

LineaAterrizaje::LineaAterrizaje()
{
    //this->wp = wp;
    mostrada = true;
    //angulo = wp->headingCircuit;
    //aterrizajeADerecha = wp->uDerecha;
}

void LineaAterrizaje::pintarAterrizajeEnUDerecha(QPainter* painter, QPointF punto, double angulo)
{
    double anguloInversa = 180 + angulo;
    if (anguloInversa > 360) {
        anguloInversa = anguloInversa - 360;
    }

    double anguloReal = HerramientaConversorCoordenadas::getInstancia()->deconvertirAngulo(anguloInversa);
    QLineF linea;
    linea.setP1(punto);
    linea.setAngle(anguloReal);

    double hipotenusa = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, recorrido);
    linea.setLength(hipotenusa);

    painter->save();
    painter->setPen(QPen(QColor(0xFF, 0x00, 0x00), 2));
    painter->drawLine(linea);

    // lineaBase
    QLineF lineaBase;
    lineaBase.setP1(linea.p2());
    lineaBase.setLength(HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, base));
    lineaBase.setAngle(90 + anguloReal);
    painter->drawLine(lineaBase);

    //recorrido inicio
    QLineF lineaRecorrido;
    lineaRecorrido.setP1(lineaBase.p2());
    lineaRecorrido.setLength(hipotenusa);
    lineaRecorrido.setAngle(HerramientaConversorCoordenadas::getInstancia()->deconvertirAngulo(angulo));  
    painter->drawLine(lineaRecorrido);

    pintarFlechaDireccion(painter, linea);
    pintarFlechaDireccion(painter, lineaBase);
    pintarFlechaDireccion(painter, lineaRecorrido);

    painter->restore();
}

void LineaAterrizaje::pintarFlechaDireccion(QPainter* painter, QLineF linea)
{
    int size = 15;
    qreal Pi = 3.1415;

    double angle = ::acos(linea.dx() / linea.length());
    if (linea.dy() >= 0)
           angle = (Pi * 2) - angle;

    QPointF arrowP1, arrowP2;

    arrowP1 = QPointF(linea.center().x(), linea.center().y()) + QPointF(sin(angle + Pi / 3) * size,
                               cos(angle + Pi / 3) * size);
    arrowP2 = QPointF(linea.center().x(), linea.center().y()) + QPointF(sin(angle + Pi - Pi / 3) * size,
                               cos(angle + Pi - Pi / 3) * size);

    QPolygonF flecha;
    flecha << linea.center() << arrowP1 << arrowP2;
    painter->setBrush(QBrush(QColor(0xFF, 0x00, 0x00)));
    painter->drawPolygon(flecha);
}

void LineaAterrizaje::pintarAterrizajeEnUIzquierda(QPainter* painter, QPointF punto, double angulo)
{
    double anguloInversa = 180 + angulo;
    if (anguloInversa > 360) {
        anguloInversa = anguloInversa - 360;
    }

    double anguloReal = HerramientaConversorCoordenadas::getInstancia()->deconvertirAngulo(anguloInversa);
    QLineF linea;
    linea.setP1(punto);
    linea.setAngle(anguloReal);

    double hipotenusa = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, recorrido);
    linea.setLength(hipotenusa);

    painter->save();
    painter->setPen(QPen(QColor(0xFF, 0x00, 0x00), 2));
    painter->drawLine(linea);

    // lineaBase
    QLineF lineaBase;
    lineaBase.setP1(linea.p2());
    lineaBase.setLength(HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, base));
    lineaBase.setAngle(fabs((270 + anguloReal)));
    painter->drawLine(lineaBase);

    //recorrido inicio
    QLineF lineaRecorrido;
    lineaRecorrido.setP1(lineaBase.p2());
    lineaRecorrido.setLength(hipotenusa);
    lineaRecorrido.setAngle(HerramientaConversorCoordenadas::getInstancia()->deconvertirAngulo(angulo));
    painter->drawLine(lineaRecorrido);

    pintarFlechaDireccion(painter, linea);
    pintarFlechaDireccion(painter, lineaBase);
    pintarFlechaDireccion(painter, lineaRecorrido);

    painter->restore();
}

void LineaAterrizaje::pintar(QPainter* painter, QPointF punto, double angulo, bool aterrizajeADerecha)
{

    if (aterrizajeEnU) {
        if (aterrizajeADerecha) {
            pintarAterrizajeEnUDerecha(painter, punto, angulo);
        } else {
            pintarAterrizajeEnUIzquierda(painter, punto, angulo);
        }
    } else {
        //ver otras opciones
    }

}
