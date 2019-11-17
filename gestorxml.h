#ifndef GESTORXML_H
#define GESTORXML_H

#include <QString>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFile>
#include <QDateTime>
#include "datosproyecto.h"
#include "lineaaltura.h"
#include "comandoextra.h"
#include "herramientasvisor.h"

class GestorXML
{
public:
    GestorXML();

    static bool guardarProyecto(QString nombreFichero, DatosProyecto proyecto);
    static bool leerProyecto(QString nombreFichero, DatosProyecto* proyecto);
    static void leerNodoDatosProyecto(QXmlStreamReader* Rxml, DatosProyecto* proyecto);
    static void leerNodoConfiguracionVisorAlturas(QXmlStreamReader* Rxml, DatosProyecto* proyecto);
    static void leerNodoLineas(QXmlStreamReader* Rxml, DatosProyecto* proyecto);
    static void leerNodoWayPoints(QXmlStreamReader* Rxml, DatosProyecto* proyecto);
    static void leerNodoComandosExtra(QXmlStreamReader* Rxml, WayPoint* wp);
    static void leerNodoImagenesMapa(QXmlStreamReader* Rxml, DatosProyecto* proyecto);
    static int leerValorEntero(QXmlStreamReader* Rxml);
    static QString leerValorCadena(QXmlStreamReader* Rxml);
    static double leerValorDouble(QXmlStreamReader* Rxml);
    static bool leerValorBooleano(QXmlStreamReader* Rxml);
    static Enumerados::TipoWayPoint getTipoWayPoint(QString tipo);
    static Enumerados::ModoWayPoint getModoWayPoint(QString modo);
    static Enumerados::DireccionCirculo getDireccionCirculo(QString direccion);
    static Enumerados::Coordenadas getTipoCoordenada(QString tipo);

};

#endif // GESTORXML_H
