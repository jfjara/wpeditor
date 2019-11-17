#ifndef IMAGENMAPA_H
#define IMAGENMAPA_H

#include <QImage>
#include <QPainter>
#include <QPointF>
#include <QTextStream>
#include <QFile>
#include "herramientasvisor.h"
#include "herramientaconversorcoordenadas.h"

class ImagenMapa : public QObject
{
public:
    ImagenMapa();

    QString pathFichero;
    double latitud1, longitud1;
    double latitud2, longitud2;
    int widthOriginal, heigthOriginal;
    QImage* imagen;
    QPointF punto;

    void desplazar(int dx, int dy);
    void pintar(QPainter* painter);
    void setImagen(QString pathFichero);
    bool exportarAHorizon(QString rutaHorizon);
    QString rellenarLineaPlantilla(QString linea);
    QString getNombreFicheroInfo();    
};

#endif // IMAGENMAPA_H
