#ifndef COMANDOEXTRA_H
#define COMANDOEXTRA_H

#include <QObject>
#include "enumerados.h"

class ComandoExtra
{
public:
    ComandoExtra();

    QString nombre;
    QString comando;
    int modo;
    int valor1;
    int valor2;

};

#endif // COMANDOEXTRA_H
