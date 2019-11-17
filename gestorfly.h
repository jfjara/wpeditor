#ifndef GESTORFLY_H
#define GESTORFLY_H

#include <QList>
#include <QString>
#include <QTextStream>
#include "enumerados.h"
#include "waypoint.h"
#include "herramientaconversorcoordenadas.h"
#include "gestorcomandosextra.h"
#include "avion.h"

class GestorFly
{
public:
    GestorFly();

    static int altura;
    static int leer(QString pathFichero, QList<WayPoint*> &waypoints);
    static bool exportar(QString fichero, Enumerados::Coordenadas tipo, QList<WayPoint*> waypoints, Avion* avion);

private:
    static void incluirFichero(QTextStream* stream, QString path);
    static int procesarLinea(QString linea, WayPoint* &wp, QList<WayPoint*> &waypoints);
    static double getLatitud(QString cadena);
    static double getLongitud(QString cadena);

};

#endif // GESTORFLY_H
