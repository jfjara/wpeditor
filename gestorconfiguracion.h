#ifndef GESTORCONFIGURACION_H
#define GESTORCONFIGURACION_H

#include <QFile>
#include <QTextStream>
#include "constantes.h"
#include "idioma.h"
#include "enumerados.h"
#include "simplecrypt.h"

class GestorConfiguracion
{
public:
    Enumerados::IdiomasAplicacion idioma;
    QString rutaHorizon;

    static GestorConfiguracion* getInstancia();
    void leerConfiguracion();
    void guardar();

private:
    GestorConfiguracion();
    static GestorConfiguracion* instancia;

};

#endif // GESTORCONFIGURACION_H
