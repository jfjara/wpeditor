#ifndef GESTORTILES_H
#define GESTORTILES_H

#include "tile.h"
#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "camara.h"
#include <QObject>
#include <QImageWriter>
#include <QImageReader>
#include <QPicture>

class GestorTiles : public QObject
{
    Q_OBJECT
public:
    //GestorTiles() { }
    GestorTiles(QString pathCarpetaImagenes, QString pathFicheroReferencias, Camara* camara);

    QString pathCarpetaImagenes;
    QString pathFicheroReferencias;
    Camara* camara;
    double anguloLente;
    bool cancelar = false;
    int indiceImagenResaltada = -1;

    QList<Tile*> tiles;
    void leerFicheroReferencias();
    void pintar(QPainter* painter);
    void marcarImagen(int i);
    void eliminarImagenSeleccionada();
    void eliminarImagenPorNombre(QString nombre);
    QImage obtenerMosaico(int anchoMapa, int altoMapa);

private:

    double obtenerCoordenadaDerecha();
    double obtenerCoordenadaIzquierda();
    double obtenerCoordenadaArriba();
    double obtenerCoordenadaAbajo();


    Tile* obtenerTile(QString linea);


signals:
    void enviarTotalImagenes(int total);
    void imagenProcesada();

};

#endif // GESTORTILES_H
