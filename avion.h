#ifndef AVION_H
#define AVION_H

#include <QString>

class Avion
{
public:
    Avion();

    QString empresa;
    QString modelo;
    QString version;
    QString rutaImagen;

    double velocidadCrucero;
    double pitchMaximo;
    double pitchMinimo;
    double baseCircuit;
    double recorridoCircuit;

    QString pathHeader;
    QString pathFooter;

    QString getPropiedades();

};

#endif // AVION_H
