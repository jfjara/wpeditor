#include "idioma.h"

Idioma* Idioma::instancia = Q_NULLPTR;

Idioma::Idioma()
{
}

Idioma* Idioma::getInstancia()
{
    if (instancia == Q_NULLPTR) {
        instancia = new Idioma();
    }
    return instancia;
}

void Idioma::setIdioma(Enumerados::IdiomasAplicacion idioma)
{
    this->idiomaSeleccionado = idioma;    
    if (propiedades != Q_NULLPTR) {
        delete propiedades;
    }
    switch (idiomaSeleccionado) {
    case Enumerados::Castellano:
        propiedades = new Properties("language/castellano.cfg");
        break;
    case Enumerados::English:
        propiedades = new Properties("./language/english.cfg");
    default:
        break;
    }
}

QString Idioma::getTexto(QString clave)
{
    if (propiedades == Q_NULLPTR) {
        return clave;
    }
    return propiedades->getValor(clave);
}
