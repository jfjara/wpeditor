#ifndef GESTORGRIDS_H
#define GESTORGRIDS_H

#include <QPointF>
#include <QList>
#include <QPainter>
#include <grid.h>
#include "herramientaconversorcoordenadas.h"
#include "operacionrealizada.h"
#include "gestorhistoricooperaciones.h"

class GestorGrids
{
public:
    GestorGrids();

    bool modoCrear = false;
    bool estaDesplazandoPuntoPoligono = false;
    QList<Grid*> grids;    

    void realizarOperacionHistorica(OperacionRealizada* operacion);
    void activarCreacionGrid(bool valor);
    void addPuntoGrid(QPointF punto);
    void addPuntoVirtual(QPointF punto);
    void desplazaPuntoPoligono(QPointF punto);
    Grid* getGridMarcado();
    bool marcarGrid(QPointF puntoPulsado);
    bool marcarPuntoGrid(QPointF puntoPulsado);
    void marcarTodosGrids(bool valor);
    void eliminarGridMarcados();
    void eliminarTodosGrids();    
    void pintar(QPainter* painter);
    void seleccionarGridsPorCaja(QRectF caja);

private:
    void deshacerCrear(Grid* grid);
    void deshacerBorrar(Grid* grid);
};

#endif // GESTORGRIDS_H
