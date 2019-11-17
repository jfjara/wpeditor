#ifndef VISORALTURASWIDGET_H
#define VISORALTURASWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QWheelEvent>
#include "waypoint.h"
#include "waypointaltura.h"
#include "lineasalturadialog.h"
#include "configuracionvisordialog.h"
#include "datosproyecto.h"
#include "gestorwaypoints.h"
#include "idioma.h"
#include "constantes.h"
#include "formateador.h"


class VisorAlturasWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VisorAlturasWidget(DatosProyecto* datosProyecto, GestorWayPoints* gestorWayPoints, QWidget *parent = 0);

    DatosProyecto* datosProyecto;
    GestorWayPoints* gestorWayPoints;
    QList<WayPoint*> listaWP;
    QList<WayPointAltura*> listaWPAlturas;
    QAction* accionConfiguracionVisor = Q_NULLPTR;
    QAction* accionLineasAlturas = Q_NULLPTR;
    static double offsetX;
    int heightVisor;
    int widthVisor;
    int widthVisorAnterior;

    bool addImagenMapa(QString fichero);
    void setWayPoints(QList<WayPoint*> lista);
    void addWayPoint(WayPoint* wp);
    void limpiarWayPoints();
    void borrarWP(WayPoint* wp);
    void borrarWPAltura(WayPoint* wp);
    double getMaximaAlturaWP();
    void actualizarElementosVisor();
    void traducir();
    float getAngulo(float x1, float y1, float x2, float y2);
    WayPointAltura* getWayPointEnPunto(QPointF punto);
    void ocultarDistanciaDeWayPointsAltura();

protected:

    QPointF puntoAnclajeRaton;
    QPointF posicionPuntero;
    bool clickIzquierdoPulsado = false;    

    void paintEvent(QPaintEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent* ev) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    bool verNombres = false;
    bool verAlturas = true;
    bool distanciasReales = true;
    double factorEscala = 1.0;
    WayPointAltura* wayPointSeleccionado = Q_NULLPTR;

    void desmarcarWayPoints();
    void pintarLineasUnionWayPoints(QPainter* painter, WayPointAltura* wp1, WayPointAltura* wp2);
    void pintarLineasAltura(QPainter* painter, int width, int height, int alturaMaxima);
    void pintarGrafica(QPainter* painter);
    void pintarLineaMaximaAltura(QPainter* painter);

signals:
    void wapypointSeleccionado(int orden);

public slots:
    void mostrarMenuContextual(const QPoint &);
    void mostrarVentanaLineas();
    void mostrarConfiguracionVisor();
};

#endif // VISORALTURASWIDGET_H
