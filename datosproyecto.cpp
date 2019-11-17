#include "datosproyecto.h"

DatosProyecto::DatosProyecto()
{

}

void DatosProyecto::limpiar()
{
    nombre = "";
    localizacion = "";
    QString descripcion;
    waypointsLista.clear();
    lineasAltura.clear();
    imagenesMapa.clear();
    nombreFichero = "";
    rutaCarpetaMapasHorizon = "";
    tipoCoordenada = Enumerados::Pixeles;

    guardado = false;

    //configuracion
    maximaInclinacion = 50;
    maximaDeclinacion = 50;
    alturaMaximaPermitida = 120;
    alturaMaximaVisor = 150;
}
