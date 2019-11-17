#ifndef DATOSPROYECTO_H
#define DATOSPROYECTO_H

#include <QString>
#include <QList>
#include "enumerados.h"
#include "waypoint.h"
#include "lineaaltura.h"
#include "imagenmapa.h"

class DatosProyecto
{
public:
    DatosProyecto();

    QString nombre;
    QString localizacion;
    QString descripcion;
    Enumerados::Coordenadas tipoCoordenada = Enumerados::LatLng;
    QString letraZona = "S";
    QString numeroZona = "30";

    QList<WayPoint*> waypointsLista;
    QList<LineaAltura*> lineasAltura;
    QList<ImagenMapa*> imagenesMapa;
    QString nombreFichero;
    QString rutaCarpetaMapasHorizon;
    bool guardado = false;    
    int numeroAvion = 0;
    int maximaInclinacion = 50;
    int maximaDeclinacion = 50;
    int alturaMaximaPermitida = 120;
    int alturaMaximaVisor = 150;
    int alturaWaypoints = ALTURA_DEFECTO;
    bool aterrizajeCirculo = true;

    void limpiar();

};

#endif // DATOSPROYECTO_H
