#ifndef IDIOMA_H
#define IDIOMA_H

#include "enumerados.h"
#include "properties.h"

class Idioma
{
private:
    Idioma();
    static Idioma* instancia;

    Properties* propiedades = Q_NULLPTR;
    Enumerados::IdiomasAplicacion idiomaSeleccionado = Enumerados::Castellano;

public:
    static Idioma* getInstancia();
    void setIdioma(Enumerados::IdiomasAplicacion idioma);
    QString getTexto(QString clave);
};

#endif // IDIOMA_H
