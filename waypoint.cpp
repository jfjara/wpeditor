#include "waypoint.h"
#include <QDebug>

WayPoint::WayPoint()
{
#ifdef MODO_PORTATIL
    diametroMarcaWP = DIAMETRO_MARCA_WAYPOINT_PORTATIL;
    radioMarcaWP = RADIO_MARCA_WAYPOINT_PORTATIL;
#endif
}

WayPoint::WayPoint(QPointF punto, int altura)
{    
    this->altura = altura;
    //QPointF puntoReal(punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(),
    //                          punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
    QPointF puntoReal = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
    //latitud = HerramientaConversorCoordenadas::getInstancia()->convertirPixelYALatitud(puntoReal.y());
    //longitud= HerramientaConversorCoordenadas::getInstancia()->convertirPixelXALongitud(puntoReal.x());
    coordenadaY = puntoReal.y();
    coordenadaX= puntoReal.x();
    refrescarPosicion();

#ifdef MODO_PORTATIL
    diametroMarcaWP = DIAMETRO_MARCA_WAYPOINT_PORTATIL;
    radioMarcaWP = RADIO_MARCA_WAYPOINT_PORTATIL;
#endif
}

void WayPoint::refrescarPosicion()
{
    QPointF puntoConvertido = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, coordenadaX, coordenadaY);
     punto.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoConvertido.x());
     punto.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoConvertido.y());
     cajaEnvolvente.setLeft((punto.x() - radioMarcaWP));
     cajaEnvolvente.setRight((punto.x() + radioMarcaWP));
     cajaEnvolvente.setTop((punto.y() - radioMarcaWP));
     cajaEnvolvente.setBottom((punto.y() + radioMarcaWP));
}

void WayPoint::pintar(QPainter* painter)
{
    //refrescarPosicion();

    painter->save();
    if (tipo ==  Enumerados::TipoWayPoint::WayPoint) {

        if (modo == Enumerados::FromTo) {
            painter->setPen(QPen(QColor(0x00, 0x7F, 0x0E), 2));
            painter->setBrush(QBrush(Qt::green));
        } else {
            painter->setPen(QPen(QColor(0xFF, 0xD8, 0x00), 2));
            painter->setBrush(QBrush(Qt::yellow));
        }

    } else if (tipo == Enumerados::TipoWayPoint::Circuit) {
        painter->setPen(QPen(QColor(0x7F, 0x00, 0x00), 2));
        painter->setBrush(QBrush(QColor(0xFF, 0x00, 0x00)));
        if (aterrizaje == Q_NULLPTR) {
            aterrizaje = new LineaAterrizaje();
        }

        aterrizaje->pintar(painter, punto, headingCircuit, uDerecha);

    } else if (tipo == Enumerados::TipoWayPoint::Circulo) {
        painter->setPen(QPen(QColor(0x57, 0x00, 0x7F), 2));
        painter->setBrush(QBrush(QColor(0xB2, 0x00, 0xFF)));
    }


    painter->drawEllipse((punto.x() - radioMarcaWP) ,
                         (punto.y() - radioMarcaWP) ,
                         diametroMarcaWP,
                         diametroMarcaWP);
    painter->setPen(QPen(QColor(0x00, 0x00, 0x00), 2));
    painter->setFont(QFont("Arial", 8, QFont::Bold));
    painter->drawText(cajaEnvolvente, QString::number(orden),  Qt::AlignHCenter | Qt::AlignVCenter);
    painter->restore();


    if (tipo == Enumerados::TipoWayPoint::Circulo) {
        painter->save();
        painter->setPen(QPen(QColor(0xFF, 0x7F, 0xED, 0xBB), 2));
        double metrosRadio = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, diametro) / 2;
        painter->drawEllipse(punto.x() - metrosRadio , punto.y() - metrosRadio , metrosRadio * 2, metrosRadio * 2);

        //dibujar la direccion del circulo
        int p1x = punto.x()  ;
        int p1y = (punto.y() + metrosRadio) ;
        qreal Pi = 3.1415;
        QLineF lineaTangente(QPointF(p1x, p1y), QPointF(p1x + 10, p1y));

        double angle = ::acos(lineaTangente.dx() / lineaTangente.length());
        if (lineaTangente.dy() >= 0)
               angle = (Pi * 2) - angle;

        int size = 10;
        QPolygonF flecha;

        if (direccionCirculo == Enumerados::Izquierda) {
            QPointF arrowP1 = QPointF(p1x, p1y) - QPointF(sin(angle + Pi / 3) * size,
                                           cos(angle + Pi / 3) * size);
            QPointF arrowP2 = QPointF(p1x, p1y) - QPointF(sin(angle + Pi - Pi / 3) * size,
                                           cos(angle + Pi - Pi / 3) * size);
            flecha << QPointF(p1x, p1y) << arrowP1 << arrowP2;
        } else {
            QPointF arrowP1 = QPointF(p1x, p1y) + QPointF(sin(angle + Pi / 3) * size,
                                           cos(angle + Pi / 3) * size);
            QPointF arrowP2 = QPointF(p1x, p1y) + QPointF(sin(angle + Pi - Pi / 3) * size,
                                           cos(angle + Pi - Pi / 3) * size);
            flecha << QPointF(p1x, p1y) << arrowP1 << arrowP2;
        }

        painter->setBrush(QBrush(QColor(0xFF, 0x7F, 0xED, 0xAA)));
        painter->drawPolygon(flecha);
        painter->restore();
    }

    if (marcado) {
        painter->save();
        painter->setPen(QPen(QColor(0xFF, 0xFF, 0x00), 2));
        painter->setBrush(QColor(0xFF, 0xFF, 0x00, 0x66));
        QRectF cajaMarcado(cajaEnvolvente.x() - 5, cajaEnvolvente.y() - 5, cajaEnvolvente.width() + 10, cajaEnvolvente.height() + 10);
        painter->drawRect(cajaMarcado);
        painter->restore();
    }
}

QString WayPoint::toString()
{
    QString resultado = QString::number(orden) + "-";

    switch (tipo) {   
    case Enumerados::WayPoint:

        if (modo == Enumerados::FromTo) {
            resultado.append("FromTo (");
        } else if (modo == Enumerados::FlyTo) {
            resultado.append("FlyTo (");
        } else {
            resultado.append("Waypoint (");
        }
        resultado.append(QString::number(punto.x()) + ", " + QString::number(punto.y()) + ")");
        break;
    case Enumerados::Circulo:
        resultado.append("Circle (");
        resultado.append(QString::number(punto.x()) + ", " + QString::number(punto.y()) + ")");
        break;
    case Enumerados::Circuit:
        resultado.append("Circuit (");
        resultado.append(QString::number(punto.x()) + ", " + QString::number(punto.y()) + ")");
        break;
    default:
        break;
    }

    return resultado;
}
