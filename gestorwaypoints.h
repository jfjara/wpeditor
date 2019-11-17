#ifndef GESTORWAYPOINTS_H
#define GESTORWAYPOINTS_H

#include <QList>
#include <QPointF>
#include <QPainter>
#include "math.h"
#include "lineaaterrizaje.h"
#include "enumerados.h"
#include "waypoint.h"
#include "herramientasvisor.h"
#include "herramientaconversorcoordenadas.h"
#include "operacionrealizada.h"
#include "gestorhistoricooperaciones.h"
#include "datosproyecto.h"

class GestorWayPoints
{
public:
    GestorWayPoints(DatosProyecto* proyecto);

    int alturaWaypoints = ALTURA_DEFECTO;
    QList<WayPoint*> wayPoints;
    bool mostrarLineas = true;
    bool modoCrear = false;
    DatosProyecto* proyecto;

    WayPoint* clonarWayPoint(WayPoint* wp);
    void realizarOperacionHistorica(OperacionRealizada* operacion);
    void seleccionarPuntosPorCaja(QRectF caja);
    void crearProtocoloAterrizaje(LineaAterrizaje* aterrizaje);
    void crearWayPoint(QPointF punto);
    void crearWayPoint(QPointF punto, int altura);
    WayPoint* crearWayPoint(int posicion, QPointF punto, int altura);
    WayPoint* crearWayPointAnteriorAlMarcado();
    int marcaWayPoint(QPointF puntoPulsado);
    void marcaWayPoint(int orden);
    bool hayWayPoint(QPointF punto);
    void marcarTodosWayPoint(bool valor);
    void desplazarWayPoint(QPointF punto);
    int marcarSiguienteWayPoint();
    int marcarAnteriorWayPoint();
    QList<WayPoint*> eliminarWayPointMarcados();
    int borrarPuntosDespuesLanding();
    WayPoint* getWayPointMarcado();
    WayPoint* getUltimoWayPoint() { return wayPoints.last(); }
    WayPoint* getCircuit();
    bool hayWayPointMarcado();
    double getDistanciaTotalVuelo();
    void refrescarWayPoints();
    void reorganizarOrdenWP();
    void pintar(QPainter* painter,  Enumerados::Coordenadas tipo, bool verDistanciasEntrePuntos, bool verLineasDireccion,  bool distanciaTotalRecorrido);
    void pintarLinea( Enumerados::Coordenadas tipo, WayPoint* wp1, WayPoint* wp2, QPainter* painter, bool verDistanciasEntrePuntos, bool distanciaTotalRecorrido, double recorrido);
    void pintarLineaRepeticion(WayPoint* wp1, WayPoint* wp2, QPainter* painter, bool verDistanciasEntrePuntos, bool distanciaTotalRecorrido, double recorrido);

private:
    double calcularDistanciaWP(Enumerados::Coordenadas tipo, WayPoint* wp1, WayPoint* wp2);
    void deshacerBorrar(QList<WayPoint*> lista);
    void deshacerCrear(WayPoint* wp);
    void deshacerModificar(WayPoint* wp);
    void deshacerBorrar(WayPoint* wp);
};

#endif // GESTORWAYPOINTS_H
