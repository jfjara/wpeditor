#include "gestorhistoricooperaciones.h"

GestorHistoricoOperaciones* GestorHistoricoOperaciones::instancia = Q_NULLPTR;

GestorHistoricoOperaciones::GestorHistoricoOperaciones()
{
}

GestorHistoricoOperaciones* GestorHistoricoOperaciones::getInstancia()
{
    if (instancia == Q_NULLPTR) {
        instancia = new GestorHistoricoOperaciones();
    }
    return instancia;
}

void GestorHistoricoOperaciones::apilar(OperacionRealizada* operacion)
{
    pila.push_front(operacion);
}

OperacionRealizada* GestorHistoricoOperaciones::desapilar()
{
    if (pila.size() <= 0) {
        return Q_NULLPTR;
    }
    OperacionRealizada* operacion = pila[0];
    pila.pop_front();
    return operacion;
}

bool GestorHistoricoOperaciones::hayHistorico()
{
    if (pila.size() > 0) {
        return true;
    }
    return false;
}
