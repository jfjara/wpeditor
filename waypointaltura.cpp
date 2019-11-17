#include "waypointaltura.h"
#include "visoralturaswidget.h"
#include <Qdebug>

WayPointAltura::WayPointAltura()
{
#ifdef MODO_PORTATIL
    diametroMarcaWP = DIAMETRO_MARCA_WAYPOINT_PORTATIL;
    radioMarcaWP = RADIO_MARCA_WAYPOINT_PORTATIL;
#endif
}

void WayPointAltura::setWayPoint(WayPoint* wp, int height,int maximaAltura)
{
    wayPoint = wp;
    posY = wayPoint->altura * height / maximaAltura;
    posY = posY - 30;
    posY = height - posY;
    cajaEnvolvente = QRectF(0, posY - diametroMarcaWP/2, diametroMarcaWP, diametroMarcaWP);
}

void WayPointAltura::pintar(QPainter* painter, int posX, bool verAltura)
{    
    cajaEnvolvente = QRectF(VisorAlturasWidget::offsetX + posX - diametroMarcaWP/2, posY - diametroMarcaWP/2, diametroMarcaWP, diametroMarcaWP);
    painter->save();
    if (wayPoint->tipo ==  Enumerados::TipoWayPoint::WayPoint) {
        if (wayPoint->modo == Enumerados::FromTo) {
            painter->setPen(QPen(QColor(0x00, 0x7F, 0x0E), 2));
            painter->setBrush(QBrush(Qt::green));
        } else {
            painter->setPen(QPen(QColor(0xFF, 0xD8, 0x00), 2));
            painter->setBrush(QBrush(Qt::yellow));
        }
    } else if (wayPoint->tipo == Enumerados::TipoWayPoint::Circuit) {
        painter->setPen(QPen(QColor(0x7F, 0x00, 0x00), 2));
        painter->setBrush(QBrush(QColor(0xFF, 0x00, 0x00)));
    } else if (wayPoint->tipo == Enumerados::TipoWayPoint::Circulo) {
        painter->setPen(QPen(QColor(0x57, 0x00, 0x7F), 2));
        painter->setBrush(QBrush(QColor(0xB2, 0x00, 0xFF)));
    }

    painter->drawEllipse(cajaEnvolvente);
    painter->setPen(QPen(QColor(0x00, 0x00, 0x00), 2));
    painter->setFont(QFont("Arial", 8, QFont::Bold));
    painter->drawText(cajaEnvolvente, QString::number(wayPoint->orden),  Qt::AlignHCenter | Qt::AlignVCenter);

    if (verAltura) {
        QRectF caja = QRectF(cajaEnvolvente.x(), cajaEnvolvente.y() + diametroMarcaWP, diametroMarcaWP + 5, diametroMarcaWP);
        painter->setPen(QPen(QColor(0xFF, 0xFF, 0xFF), 2));
        painter->setFont(QFont("Arial", 7, QFont::Bold));
        painter->drawText(caja, QString::number(wayPoint->altura) + QString("m"),  Qt::AlignLeft | Qt::AlignVCenter);
    }

    if (verDistancia) {
        int offset = diametroMarcaWP;
        if (verAltura) {
            offset = diametroMarcaWP * 2;
        }
        QRectF caja = QRectF(cajaEnvolvente.x(), cajaEnvolvente.y() + offset, diametroMarcaWP + 25, diametroMarcaWP);
        painter->setPen(QPen(QColor(0xFF, 0xFF, 0xFF), 2));
        painter->setFont(QFont("Arial", 7, QFont::Bold));
        painter->drawText(caja, QString::number(distanciaAcumulada) + QString("m"),  Qt::AlignLeft | Qt::AlignVCenter);
    }

    painter->restore();
}

void WayPointAltura::recalcularPosicionEnVisor(int height, int maximaAltura)
{
    posY = wayPoint->altura * height / maximaAltura;
    posY = posY - 30;
    posY = height - posY;
}

int WayPointAltura::getAltura(int dy,int height, int maximaAltura)
{
    int posYCalculada = posY + dy;
    return maximaAltura - ( posYCalculada - 30) * maximaAltura / height;
}

void WayPointAltura::desplazarEjeY(int dy, int height, int maximaAltura)
{
    posY = posY + dy;
    wayPoint->altura = maximaAltura - ( posY - 30) * maximaAltura / height;
}


