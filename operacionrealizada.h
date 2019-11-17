#ifndef OPERACIONREALIZADA_H
#define OPERACIONREALIZADA_H

#include <QList>
#include "enumerados.h"
#include "waypoint.h"
#include "grid.h"

class OperacionRealizada
{
public:
    OperacionRealizada();

    Enumerados::AccionRealizada accion;
    Enumerados::Objeto tipoObjeto;
    QList<WayPoint*> listaWaypoints;
    Grid* grid;

};

#endif // OPERACIONREALIZADA_H
