#ifndef TILE_H
#define TILE_H

#include <QPainter>
#include <QImage>
#include <QPointF>
#include "herramientaconversorcoordenadas.h"
#include "herramientasvisor.h"

class Tile
{
public:
    Tile(QString pathFichero, double latitud, double longitud, double altura, double anguloLente);
    ~Tile();

    QString pathFichero;
    QImage* imagenOriginal;
    QImage* imagen;
    QImage imagenEscalada;
    double latitud;
    double longitud;
    double altura;
    double anguloLente;
    QPointF punto;

    int x, y;

    void pintar(QPainter* painter);
    void pintarResaltada(QPainter* painter);
    void pintarStitching(QPainter* painter, QPointF puntoReferencia, int ancho, int alto);

};

#endif // TILE_H
