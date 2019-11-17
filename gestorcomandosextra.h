#ifndef GESTORCOMANDOSEXTRA_H
#define GESTORCOMANDOSEXTRA_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QTextStream>
#include "comandoextra.h"
#include "gestorconfiguracion.h"

class GestorComandosExtra
{
private:
    GestorComandosExtra();
    static GestorComandosExtra* instancia;
    void leerComandos();
    void procesarComando(QTextStream* stream);



public:
    static GestorComandosExtra* getInstancia();

    QList<ComandoExtra> comandos;

    ComandoExtra* getComandoEnFly(QString linea);

};

#endif // GESTORCOMANDOSEXTRA_H
