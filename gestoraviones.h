#ifndef GESTORAVIONES_H
#define GESTORAVIONES_H

#include <QList>
#include <QFile>
#include <QTextStream>
#include "avion.h"
#include "constantes.h"
#include "simplecrypt.h"

class GestorAviones
{
public:
    GestorAviones();
    ~GestorAviones();

    QList<Avion*> listaAviones;

    Avion* crearAvion(QString propiedades);
    void leerConfiguracion();

private:
    bool esModificacion = false;
};

#endif // GESTORAVIONES_H
