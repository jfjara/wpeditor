#ifndef GESTORULTIMASMAPAS_H
#define GESTORULTIMASMAPAS_H

#include <QFile>
#include <QTextStream>
#include <QList>
#include "imagenmapa.h"
#include "simplecrypt.h"

class GestorUltimasMapas
{
public:
    QList<ImagenMapa*> mapasPrevios;

    static GestorUltimasMapas* getInstancia();
    void leerDatos();
    void guardar();
    void limpiar();
    void add(ImagenMapa* mapa);

private:
    GestorUltimasMapas();
    static GestorUltimasMapas* instancia;
};

#endif // GESTORULTIMASMAPAS_H
