#ifndef GESTORCAMARAS_H
#define GESTORCAMARAS_H

#include <QList>
#include <QFile>
#include <QTextStream>
#include "camara.h"
#include "constantes.h"
#include "simplecrypt.h"

class GestorCamaras
{
public:
    GestorCamaras();
    ~GestorCamaras();

    QList<Camara*> camaras;

    void leerConfiguracion();
    Camara* crearCamara(QString propiedades);
};

#endif // GESTORCAMARAS_H
