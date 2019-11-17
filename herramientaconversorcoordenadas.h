#ifndef HERRAMIENTACONVERSORCOORDENADAS_H
#define HERRAMIENTACONVERSORCOORDENADAS_H

#include <QObject>
#include <QString>
#include <QLineF>
#include <QPointF>
#include <QPolygonF>
#include <QImage>
#include "enumerados.h"
#include "math.h"
#include "herramientasvisor.h"
//#include "utmconversion.h"


#include <cmath>			//2010-08-11: was <math.h>
#include <cstdio>			//2010-08-11: was <stdio.h>
#include <cstdlib>			//2010-08-11: was <stdlib.h>
#include <cstring>			//2010-08-11: was <string.h>
#include <cctype>			//2010-08-11: was <ctype.h>
#include <iostream>			//2010-08-11: was <iostream.h>
#include <iomanip>			//2010-08-11: was <iomanip.h>
using namespace std;			//2010-08-11: added
//const double PI       =	4*atan(1);	//Gantz used: PI=3.14159265;


#define PI 3.1415926535
#define RADIO_TIERRA_KM 6371.0
//const double deg2rad  =	PI/180;
//const double rad2deg  =	180/PI;
const double k0       =	0.9996;

class Ellipsoid{
public:
    //Ellipsoid(){};
    Ellipsoid(int Id, char* name, double radius, double ecc)
        {
            id = Id; ellipsoidName = name;
            EquatorialRadius = radius; eccentricitySquared = ecc;
        }

        int id;
        char* ellipsoidName;
        double EquatorialRadius;
        double eccentricitySquared;
};


//const double PI = 3.14159265;
const double FOURTHPI = PI / 4;
const double deg2rad = PI / 180;
const double rad2deg = 180.0 / PI;

static Ellipsoid ellipsoid[] =
{//  id, Ellipsoid name, Equatorial Radius, square of eccentricity
    Ellipsoid( -1, "Placeholder", 0, 0),//placeholder only, To allow array indices to match id numbers
    Ellipsoid( 1, "Airy", 6377563, 0.00667054),
    Ellipsoid( 2, "Australian National", 6378160, 0.006694542),
    Ellipsoid( 3, "Bessel 1841", 6377397, 0.006674372),
    Ellipsoid( 4, "Bessel 1841 (Nambia) ", 6377484, 0.006674372),
    Ellipsoid( 5, "Clarke 1866", 6378206, 0.006768658),
    Ellipsoid( 6, "Clarke 1880", 6378249, 0.006803511),
    Ellipsoid( 7, "Everest", 6377276, 0.006637847),
    Ellipsoid( 8, "Fischer 1960 (Mercury) ", 6378166, 0.006693422),
    Ellipsoid( 9, "Fischer 1968", 6378150, 0.006693422),
    Ellipsoid( 10, "GRS 1967", 6378160, 0.006694605),
    Ellipsoid( 11, "GRS 1980", 6378137, 0.00669438),
    Ellipsoid( 12, "Helmert 1906", 6378200, 0.006693422),
    Ellipsoid( 13, "Hough", 6378270, 0.00672267),
    Ellipsoid( 14, "International", 6378388, 0.00672267),
    Ellipsoid( 15, "Krassovsky", 6378245, 0.006693422),
    Ellipsoid( 16, "Modified Airy", 6377340, 0.00667054),
    Ellipsoid( 17, "Modified Everest", 6377304, 0.006637847),
    Ellipsoid( 18, "Modified Fischer 1960", 6378155, 0.006693422),
    Ellipsoid( 19, "South American 1969", 6378160, 0.006694542),
    Ellipsoid( 20, "WGS 60", 6378165, 0.006693422),
    Ellipsoid( 21, "WGS 66", 6378145, 0.006694542),
    Ellipsoid( 22, "WGS-72", 6378135, 0.006694318),
    Ellipsoid( 23, "WGS-84", 6378137, 0.00669438)
};



class HerramientaConversorCoordenadas
{
public:
    static HerramientaConversorCoordenadas* getInstancia();
    ~HerramientaConversorCoordenadas();

    double yReferencia1 = 0.0;
    double xReferencia1 = 0.0;
    double yReferencia2 = 0.0;
    double xReferencia2 = 0.0;

    double dXTotal = 0.0;
    double dYTotal = 0.0;

    QImage imagenReescalada;

    //UTMConversion conversionUTM;


//    void DatumConvert(int dIn, double LatIn, double LongIn, double HtIn, int dTo,  double& LatTo, double& LongTo, double& HtTo);
//    void LLtoUTM(int eId, double Lat, double Long,  double& Northing, double& Easting, int& Zone);
//    void UTMtoLL(int eId, double Northing, double Easting, int Zone,  double& Lat, double& Long);


    void LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long,
                 double &UTMNorthing, double &UTMEasting, char* UTMZone);
    void UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, const char* UTMZone,
                  double& Lat,  double& Long );
    char UTMLetterDesignator(double Lat);
    void LLtoSwissGrid(const double Lat, const double Long,
                 double &SwissNorthing, double &SwissEasting);
    void SwissGridtoLL(const double SwissNorthing, const double SwissEasting,
                        double& Lat, double& Long);

    double CorrRatio(double LatRad, const double C);
    double NewtonRaphson(const double initEstimate);
    //void LLtoSwissGrid(const double Lat, const double Long, double &SwissNorthing, double &SwissEasting);
    //void SwissGridtoLL(const double SwissNorthing, const double SwissEasting, double& Lat, double& Long);


    void establecerDatos(Enumerados::Coordenadas tipo, double lat1, double lng1, double lat2, double lng2, double width, double height);

    QPointF convertirUTMALatLon(double x, double y, QString utmZona);
    QPointF convertirLatLonAUTM(double lat, double lon, QString& utmZona);

    QPointF convertirCoordenada(Enumerados::Coordenadas tipo, double x, double y, QString zona = "");

    double getProporcionHorizontal(Enumerados::Coordenadas tipo);
    double getProporcionVertical(Enumerados::Coordenadas tipo);

    double getProporcionY() { return proporcionY; }
    double getProporcionX() { return proporcionX; }

    QPointF getPixel(Enumerados::Coordenadas tipo, double x, double y);

    QString getLatitudEnTexto(double latitud);
    QString getLongitudEnTexto(double longitud);

    double getDistanciaEntreCoordenadas(Enumerados::Coordenadas tipo, double x1, double y1, double x2, double y2);

    double getPixeles(Enumerados::Coordenadas tipo, double metros);

    double convertirAreaPixelesEnHectareas(QPolygonF poligono);
    double getWidth() { return widthPx; }
    double getHeight() { return heightPx; }
    double getYReferencia1() { return yReferencia1; }
    double getYReferencia2() { return yReferencia2; }
    double getXReferencia1() { return xReferencia1; }
    double getXReferencia2() { return xReferencia2; }
    double convertirAngulo(qreal angulo);
    double deconvertirAngulo(qreal angulo);
    double getRadianes(double valor);
    QPointF conversionPuntoACoordenada(QPointF pixel);
    void setWidth(double w) {
        widthPx = w;
        proporcionX = (xReferencia1 - xReferencia2) / w;
        if (proporcionX < 0) proporcionX = proporcionX * -1.0;
    }
    void setHeight(double h) {
        heightPx = h;
        proporcionY = (yReferencia1 - yReferencia2) / h;
         if (proporcionY < 0) proporcionY = proporcionY * -1.0;
    }

private:
    HerramientaConversorCoordenadas();
    static HerramientaConversorCoordenadas* instancia;

   // double proporcionLatitud = 0.0;
   // double proporcionLongitud = 0.0;
    double proporcionX = 0.0;
    double proporcionY = 0.0;
    double heightPx = 0.0;
    double widthPx = 0.0;

    void establecerDatosLatitudLongitud(double lat1, double lng1, double lat2, double lng2, double width, double height);
    void establecerDatosUTM(double x1, double y1, double x2, double y2, double width, double height);
    double convertirLatitudEnPixelY(double latitud);
    double convertirLongitudEnPixelX(double longitud);
    double convertirPixelXALongitud(double x);
    double convertirPixelYALatitud(double y);

    QPointF convertirPixelAUTM(double x, double y, QString& zona);


};

#endif // HERRAMIENTACONVERSORCOORDENADAS_H
