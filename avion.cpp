#include "avion.h"

Avion::Avion()
{

}

QString Avion::getPropiedades()
{
    QString resultado;
    resultado = empresa + " $" + modelo + " $" + version + " $" + rutaImagen + "$" + QString::number(pitchMaximo, 'f', 2) + " $" + QString::number(pitchMinimo, 'f', 2) + " $" + QString::number(velocidadCrucero, 'f', 2) + " $" + QString::number(baseCircuit, 'f', 2) + " $" + QString::number(recorridoCircuit, 'f', 2) + " $" + pathHeader + "$" + pathFooter;
    return resultado;
}
