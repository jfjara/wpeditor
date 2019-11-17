#ifndef VISORWIDGET_H
#define VISORWIDGET_H

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include "gestortiles.h"
#include "gestorwaypoints.h"
#include "gestorgrids.h"
#include "waypoint.h"
#include "imagenmapa.h"
#include "waypointdockwidget.h"
#include "herramientasvisor.h"
#include "herramientaconversorcoordenadas.h"
#include "datosproyecto.h"
#include "medidordistanciaangulo.h"
//#include "lineaaterrizaje.h"
#include "idioma.h"
#include "cajaseleccion.h"
#include "medidorescala.h"
#include "operacionrealizada.h"
#include "gestorhistoricooperaciones.h"


class VisorWidget : public QWidget
{
    Q_OBJECT
public:

    explicit VisorWidget(DatosProyecto* proyecto, GestorWayPoints* gestorWayPoints, GestorGrids* gestorGrids, QWidget *parent = 0);

    DatosProyecto* proyecto;
    GestorWayPoints* gestorWayPoints;
    GestorGrids* gestorGrids;
    GestorTiles* gestorTiles = Q_NULLPTR;
    QList<MedidorDistanciaAngulo*> medidores;
    //LineaAterrizaje* aterrizaje = Q_NULLPTR;
    QPointF posicionPuntero;
    QImage rosaDeLosVientos;    
    CajaSeleccion cajaSeleccion;
    bool mostrarLeyenda = true;
    bool verDistanciasEntrePuntos = true;
    bool verLineasDireccion = true;
    bool distanciaTotalRecorrido = false;
    bool seleccionarWaypoints = false;
    bool waypointModificado = false;
    MedidorEscala medidorEscala;

    bool verMosaico = true;

    OperacionRealizada* operacionModificacionWP = Q_NULLPTR;

#ifdef MODO_PORTATIL
    QImage zoomMas;
    QImage zoomMenos;
    QRectF cajaZoomMas = QRectF(QPointF(5, 5), QSizeF(48, 48));
    QRectF cajaZoomMenos = QRectF(QPointF(5, 55), QSizeF(48, 48));

    void acercarVisor();
    void alejarVisor();
#endif

    void realizarZoom(double delta, int px, int py);
    void activarCreacionWP(bool valor);
    void activarCreacionGrid(bool valor);    
    void activarMedirDistanciaYAngulo(bool valor);
    void crearNuevoWPAntesDelMarcado();
    void landingEstablecido();
    void marcarWayPoint(int orden);
    void modificaAterrizaje(bool derecha, int pitch);
    void establecerAterrizaje();
    void establecerAterrizajeDesdeFly();
    void pintarPosicionPuntero(QPainter* painter);
    void desplazarImagenesMapa(int dx, int dy);
    void eliminarWPSeleccionados();
    void eliminarMarcadoresSeleccionados();
    void keyPressEvent(QKeyEvent *ev);

private:    
    QPointF puntoCrearWPMenuContextual;
    QPointF puntoAnclajeRaton;
    QPointF puntoAnterior;
    bool clickIzquierdoPulsado = false;
    bool banderaOcultarMenuContextual = false;
    bool desplazarVisor = true;
    bool medirDistanciasYAngulos = false;

    void crearImagen(QPixmap &px);
    void pintarBarraAuxiliar(QPainter* painter);
    //void pintarAterrizaje(QPainter* painter);
    void pintarLeyenda(QPainter* painter);
    void crearPuntoDesdeMenuContextual(QPointF punto, Enumerados::TipoWayPoint comando, Enumerados::ModoWayPoint tipo);

protected:
    void paintEvent(QPaintEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* event) override;    

signals:
    void mostrarInformacionGrid(Grid*);
    void mostrarInformacionWP(WayPoint*);
    void ocultarInformacion();
    void waypointAdd(WayPoint*);
    void waypointDel(WayPoint*);
    void actualizarListaDespuesDeLanding(int);
    void waypointUpdate();
    void waypointVisorMarcado(int);
    void wpEliminado(int i);
    void teclaPulsada(QKeyEvent *ev);
    void finalizarEdicion();

public slots:
    void mostrarMenuContextual(const QPoint &);
    void crearWPFromToMenuContextual();
    void crearWPFlyToMenuContextual();
    void crearCircleMenuContextual();
    void crearCircuitMenuContextual();

};

#endif // VISORWIDGET_H
