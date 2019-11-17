#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QPolygonF>
#include <QPainter>
#include "waypoint.h"
#include "enumerados.h"
#include "HerramientaConversorCoordenadas.h"

class Grid
{
public:
    Grid();

    bool marcado = false;
    bool mostrarCajaEnvolvente = false;
    int camaraSeleccionada = 0;
    bool dibujarRecorridoCalculado = false;
    QList<QPointF> puntosRecorridoCalculado;
    QList<QPointF> coordenadas;
    QList<QPointF> gridPoints;
    int altura;
    int angulo = 0;
    int overshot = 100;
    int overshotLateral = 0;
    int espacioLineas = 20;
    int solapeDelantero = 0;
    int solapeLateral = 0;
    double gsd;
    bool mostrarPrevia = false;
    int posicionPuntoRedimension = -1;

    void addPunto(QPointF punto, int posicion);
    void addPunto(QPointF punto);
    void addPuntoVirtual(QPointF punto);
    void desplazaPunto(QPointF punto);
    void quitarPuntoVirtual();
    int getNumeroPuntos();
    bool estaPuntoContenido(QPointF);
    bool esPuntoDePoligono(QPointF);
    QList<QPointF> getRutaCalculada();
    void predibujarRecorrido(QPainter* painter);
    void crearRecorridoCajaEnvolvente(Enumerados::RecorridoGrid recorrido, Enumerados::DireccionHorizontalGrid direccionHorizontal, Enumerados::DireccionVerticalGrid direccionVertical, int distanciaEntreLineas, int overshot, int overshotLat);
    qreal getArea();
    void pintar(QPainter* painter);
    void crearRecorrido(int angulo);
    void crearRecorrido();
    void ajustarDireccionLinea(const QList<QLineF>& lineList, QList<QLineF>& resultLines);
    void interseccionLineasConPoligono(const QList<QLineF>& lineList, const QPolygonF& polygon, QList<QLineF>& resultLines);
    QPointF rotarPunto(int angulo, const QPointF& point, const QPointF& origin);
    bool hayInterseccionPoligonoConCaja(QRectF caja);
    double calcularDistanciaRecorrido();
    int obtenerAnguloRecorridoMasOptimo();

private:
    QPolygonF poligono;
    QRectF cajaGirada;

    void crearRecorridoHorizontalHaciaDerechaYArriba(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot,int overshotLat);
    void crearRecorridoHorizontalHaciaDerechaYAbajo(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat);
    void crearRecorridoHorizontalHaciaIzquierdaYArriba(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat);
    void crearRecorridoHorizontalHaciaIzquierdaYAbajo(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat);
    void crearRecorridoVerticalHaciaDerechaYArriba(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat);
    void crearRecorridoVerticalHaciaDerechaYAbajo(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat);
    void crearRecorridoVerticalHaciaIzquierdaYArriba(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat);
    void crearRecorridoVerticalHaciaIzquierdaYAbajo(QRectF caja, int numeroIteraciones, int espacioEntreLineas, int overshot, int overshotLat);

};

#endif // GRID_H
