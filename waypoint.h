#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QObject>
#include <QPainter>
#include <QPointF>
#include "enumerados.h"
#include "herramientasvisor.h"
#include "herramientaconversorcoordenadas.h"
#include "constantes.h"
#include "comandoextra.h"
#include "lineaaterrizaje.h"


class WayPoint
{   

public:
    WayPoint();
    WayPoint(QPointF punto, int altura);

    bool bloqueado = false;
    double coordenadaY;
    double coordenadaX;
    QPointF punto;

    QRectF cajaEnvolvente;
    Enumerados::TipoWayPoint tipo =  Enumerados::TipoWayPoint::WayPoint;
    Enumerados::ModoWayPoint modo = Enumerados::FromTo;
    bool marcado = false;
    double altura = ALTURA_DEFECTO;
    int orden;
    Enumerados::DireccionCirculo direccionCirculo = Enumerados::Izquierda;
    int diametro = DIAMETRO_WP_CIRCULO;
    int nivelado = 0;
    int cuartosCirculo = 1;
    double heading = 0;
    double headingCircuit;
    bool uDerecha = true;
    double distanciaWPAnterior = 0.0; //en metros
    int recorridoAterrizaje = 0; //en metros
    int indiceRepeticion = -1;
    LineaAterrizaje* aterrizaje = Q_NULLPTR;

    QList<ComandoExtra*> comandosExtras;

    void refrescarPosicion();
    void pintar(QPainter* painter);
    QString toString();

    int radioMarcaWP = RADIO_MARCA_WAYPOINT;
    int diametroMarcaWP = DIAMETRO_MARCA_WAYPOINT;
};

#endif // WAYPOINT_H
