#ifndef GESTORHISTORICOOPERACIONES_H
#define GESTORHISTORICOOPERACIONES_H

#include <QObject>
#include <QList>
#include "operacionrealizada.h"

class GestorHistoricoOperaciones
{
private:
    static GestorHistoricoOperaciones* instancia;
    GestorHistoricoOperaciones();

    QList<OperacionRealizada*> pila;
public:
    static GestorHistoricoOperaciones* getInstancia();

    void apilar(OperacionRealizada* operacion);
    OperacionRealizada* desapilar();
    bool hayHistorico();


};

#endif // GESTORHISTORICOOPERACIONES_H
