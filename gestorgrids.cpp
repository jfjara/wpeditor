#include "gestorgrids.h"

GestorGrids::GestorGrids()
{
}

void GestorGrids::deshacerCrear(Grid* grid)
{
    grids.removeLast();
}

void GestorGrids::deshacerBorrar(Grid* grid)
{
    grids.push_back(grid);
}

void GestorGrids::realizarOperacionHistorica(OperacionRealizada* operacion)
{
    switch (operacion->accion) {
    case Enumerados::Crear:
        deshacerCrear(operacion->grid);
        break;
    case Enumerados::Borrar:
        deshacerBorrar(operacion->grid);
        break;
    }
}

void GestorGrids::activarCreacionGrid(bool valor)
{
    if (!modoCrear && valor) {
        grids.push_back(new Grid());
        modoCrear = valor;
        return;
    } else if (modoCrear && !valor) {
        modoCrear = valor;
        grids[grids.size() - 1]->quitarPuntoVirtual();
    }
}

void GestorGrids::seleccionarGridsPorCaja(QRectF caja)
{
    for (int i = 0; i < grids.size(); i++) {
        grids[i]->marcado = false;
        if (grids[i]->hayInterseccionPoligonoConCaja(caja)) {
            grids[i]->marcado = true;
        }
    }
}

Grid* GestorGrids::getGridMarcado()
{
    for (int i = 0; i < grids.size(); i++) {
        if (grids[i]->marcado) {
            return grids[i];
        }
    }
    return Q_NULLPTR;
}

bool GestorGrids::marcarGrid(QPointF puntoPulsado)
{
    for (int i = grids.size() - 1; i >= 0; i--) {
        if (grids[i]->estaPuntoContenido(puntoPulsado)) {
            grids[i]->marcado = !grids[i]->marcado;
            return true;
        }
    }
    return false;
}

bool GestorGrids::marcarPuntoGrid(QPointF puntoPulsado)
{
    for (int i = grids.size() - 1; i >= 0; i--) {
        if (grids[i]->esPuntoDePoligono(puntoPulsado)) {
            estaDesplazandoPuntoPoligono = true;
            grids[i]->marcado = true;
            return true;
        }
    }
    estaDesplazandoPuntoPoligono = false;
}

void GestorGrids::marcarTodosGrids(bool valor)
{
    for (int i = grids.size() - 1; i >= 0; i--) {
        grids[i]->marcado = valor;
    }
}

void GestorGrids::eliminarTodosGrids()
{
    for (int i =0; i < grids.size(); i++) {
        grids.removeAt(i);
        i--;
    }
}

void GestorGrids::eliminarGridMarcados()
{
    for (int i =0; i < grids.size(); i++) {
        if (grids[i]->marcado) {
            OperacionRealizada* operacion = new OperacionRealizada();
            operacion->accion = Enumerados::Borrar;
            operacion->tipoObjeto = Enumerados::ObjetoGrid;
            operacion->grid = grids[i];
            GestorHistoricoOperaciones::getInstancia()->apilar(operacion);
            grids.removeAt(i);
            i--;
        }
    }
}

void GestorGrids::desplazaPuntoPoligono(QPointF punto)
{
    for (int i =0; i < grids.size(); i++) {
        if (grids[i]->marcado) {
            grids[i]->desplazaPunto(punto);
            return;
        }
    }
}

void GestorGrids::addPuntoVirtual(QPointF punto)
{
    if (grids.size() == 0) {
        return;
    }
    grids[grids.size() - 1]->addPuntoVirtual(punto);
}

void GestorGrids::addPuntoGrid(QPointF punto)
{
    if (grids.size() == 0) {
        return;
    }
    grids[grids.size() - 1]->addPunto(punto);
}

void GestorGrids::pintar(QPainter* painter)
{
    for (int i = 0; i < grids.size(); i++) {
        grids[i]->pintar(painter);
    }
}
