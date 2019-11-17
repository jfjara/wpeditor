#include "herramientaconversorcoordenadas.h"
#include <QDebug>

HerramientaConversorCoordenadas* HerramientaConversorCoordenadas::instancia = Q_NULLPTR;

HerramientaConversorCoordenadas::HerramientaConversorCoordenadas()
{
}

HerramientaConversorCoordenadas* HerramientaConversorCoordenadas::getInstancia()
{
    if (instancia == Q_NULLPTR) {
        instancia = new HerramientaConversorCoordenadas();
    }
    return instancia;
}

void HerramientaConversorCoordenadas::LLtoSwissGrid(const double Lat, const double Long,
             double &SwissNorthing, double &SwissEasting)
{
//converts lat/long to Swiss Grid coords.  Equations from "Supplementary PROJ.4 Notes-
//Swiss Oblique Mercator Projection", August 5, 1995, Release 4.3.3, by Gerald I. Evenden
//Lat and Long are in decimal degrees
//This transformation is, of course, only valid in Switzerland
    //Written by Chuck Gantz- chuck.gantz@globalstar.com
    double a = ellipsoid[3].EquatorialRadius; //Bessel ellipsoid
    double eccSquared = ellipsoid[3].eccentricitySquared;
    double ecc = sqrt(eccSquared);

    double LongOrigin = 7.43958333; //E7d26'22.500"
    double LatOrigin = 46.95240556; //N46d57'8.660"

    double LatRad = Lat*deg2rad;
    double LongRad = Long*deg2rad;
    double LatOriginRad = LatOrigin*deg2rad;
    double LongOriginRad = LongOrigin*deg2rad;

    double c = sqrt(1+((eccSquared * pow(cos(LatOriginRad), 4)) / (1-eccSquared)));

    double equivLatOrgRadPrime = asin(sin(LatOriginRad) / c);

    //eqn. 1
    double K = log(tan(FOURTHPI + equivLatOrgRadPrime/2))
                -c*(log(tan(FOURTHPI + LatOriginRad/2))
                    - ecc/2 * log((1+ecc*sin(LatOriginRad)) / (1-ecc*sin(LatOriginRad))));


    double LongRadPrime = c*(LongRad - LongOriginRad); //eqn 2
    double w = c*(log(tan(FOURTHPI + LatRad/2))
                - ecc/2 * log((1+ecc*sin(LatRad)) / (1-ecc*sin(LatRad)))) + K; //eqn 1
    double LatRadPrime = 2 * (atan(exp(w)) - FOURTHPI); //eqn 1

    //eqn 3
    double sinLatDoublePrime = cos(equivLatOrgRadPrime) * sin(LatRadPrime)
                                - sin(equivLatOrgRadPrime) * cos(LatRadPrime) * cos(LongRadPrime);
    double LatRadDoublePrime = asin(sinLatDoublePrime);

    //eqn 4
    double sinLongDoublePrime = cos(LatRadPrime)*sin(LongRadPrime) / cos(LatRadDoublePrime);
    double LongRadDoublePrime = asin(sinLongDoublePrime);

    double R = a*sqrt(1-eccSquared) / (1-eccSquared*sin(LatOriginRad) * sin(LatOriginRad));

    SwissNorthing = R*log(tan(FOURTHPI + LatRadDoublePrime/2)) + 200000.0; //eqn 5
    SwissEasting = R*LongRadDoublePrime + 600000.0; //eqn 6

}


void HerramientaConversorCoordenadas::SwissGridtoLL(const double SwissNorthing, const double SwissEasting,
                    double& Lat, double& Long)
{
    double a = ellipsoid[3].EquatorialRadius; //Bessel ellipsoid
    double eccSquared = ellipsoid[3].eccentricitySquared;
    double ecc = sqrt(eccSquared);

    double LongOrigin = 7.43958333; //E7d26'22.500"
    double LatOrigin = 46.95240556; //N46d57'8.660"

    double LatOriginRad = LatOrigin*deg2rad;
    double LongOriginRad = LongOrigin*deg2rad;

    double R = a*sqrt(1-eccSquared) / (1-eccSquared*sin(LatOriginRad) * sin(LatOriginRad));

    double LatRadDoublePrime = 2*(atan(exp((SwissNorthing - 200000.0)/R)) - FOURTHPI); //eqn. 7
    double LongRadDoublePrime = (SwissEasting - 600000.0)/R; //eqn. 8 with equation corrected


    double c = sqrt(1+((eccSquared * pow(cos(LatOriginRad), 4)) / (1-eccSquared)));
    double equivLatOrgRadPrime = asin(sin(LatOriginRad) / c);

    double sinLatRadPrime = cos(equivLatOrgRadPrime)*sin(LatRadDoublePrime)
                            + sin(equivLatOrgRadPrime)*cos(LatRadDoublePrime)*cos(LongRadDoublePrime);
    double LatRadPrime = asin(sinLatRadPrime);

    double sinLongRadPrime = cos(LatRadDoublePrime)*sin(LongRadDoublePrime)/cos(LatRadPrime);
    double LongRadPrime = asin(sinLongRadPrime);

    Long = (LongRadPrime/c + LongOriginRad) * rad2deg;

    Lat = NewtonRaphson(LatRadPrime) * rad2deg;

}

double HerramientaConversorCoordenadas::NewtonRaphson(const double initEstimate)
{
    double Estimate = initEstimate;
    double tol = 0.00001;
    double corr;

    double eccSquared = ellipsoid[3].eccentricitySquared;
    double ecc = sqrt(eccSquared);

    double LatOrigin = 46.95240556; //N46d57'8.660"
    double LatOriginRad = LatOrigin*deg2rad;

    double c = sqrt(1+((eccSquared * pow(cos(LatOriginRad), 4)) / (1-eccSquared)));

    double equivLatOrgRadPrime = asin(sin(LatOriginRad) / c);

    //eqn. 1
    double K = log(tan(FOURTHPI + equivLatOrgRadPrime/2))
                -c*(log(tan(FOURTHPI + LatOriginRad/2))
                    - ecc/2 * log((1+ecc*sin(LatOriginRad)) / (1-ecc*sin(LatOriginRad))));
    double C = (K - log(tan(FOURTHPI + initEstimate/2)))/c;

    do
    {
        corr = CorrRatio(Estimate, C);
        Estimate = Estimate - corr;
    }
    while (fabs(corr) > tol);

    return Estimate;
}



double HerramientaConversorCoordenadas::CorrRatio(double LatRad, const double C)
{
    double eccSquared = ellipsoid[3].eccentricitySquared;
    double ecc = sqrt(eccSquared);
    double corr = (C + log(tan(FOURTHPI + LatRad/2))
                - ecc/2 * log((1+ecc*sin(LatRad)) / (1-ecc*sin(LatRad)))) * (((1-eccSquared*sin(LatRad)*sin(LatRad)) * cos(LatRad)) / (1-eccSquared));

    return corr;
}

void HerramientaConversorCoordenadas::LLtoUTM(int ReferenceEllipsoid, const double Lat, const double Long,
             double &UTMNorthing, double &UTMEasting, char* UTMZone)
{
//converts lat/long to UTM coords.  Equations from USGS Bulletin 1532
//East Longitudes are positive, West longitudes are negative.
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees
    //Written by Chuck Gantz- chuck.gantz@globalstar.com

    double a = ellipsoid[ReferenceEllipsoid].EquatorialRadius;
    double eccSquared = ellipsoid[ReferenceEllipsoid].eccentricitySquared;
    double k0 = 0.9996;

    double LongOrigin;
    double eccPrimeSquared;
    double N, T, C, A, M;

//Make sure the longitude is between -180.00 .. 179.9
    double LongTemp = (Long+180)-int((Long+180)/360)*360-180; // -180.00 .. 179.9;

    double LatRad = Lat*deg2rad;
    double LongRad = LongTemp*deg2rad;
    double LongOriginRad;
    int    ZoneNumber;

    ZoneNumber = int((LongTemp + 180)/6) + 1;

    if( Lat >= 56.0 && Lat < 64.0 && LongTemp >= 3.0 && LongTemp < 12.0 )
        ZoneNumber = 32;

  // Special zones for Svalbard
    if( Lat >= 72.0 && Lat < 84.0 )
    {
      if(      LongTemp >= 0.0  && LongTemp <  9.0 ) ZoneNumber = 31;
      else if( LongTemp >= 9.0  && LongTemp < 21.0 ) ZoneNumber = 33;
      else if( LongTemp >= 21.0 && LongTemp < 33.0 ) ZoneNumber = 35;
      else if( LongTemp >= 33.0 && LongTemp < 42.0 ) ZoneNumber = 37;
     }
    LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone
    LongOriginRad = LongOrigin * deg2rad;

    //compute the UTM Zone from the latitude and longitude
    sprintf(UTMZone, "%d%c", ZoneNumber, UTMLetterDesignator(Lat));

    eccPrimeSquared = (eccSquared)/(1-eccSquared);

    N = a/sqrt(1-eccSquared*sin(LatRad)*sin(LatRad));
    T = tan(LatRad)*tan(LatRad);
    C = eccPrimeSquared*cos(LatRad)*cos(LatRad);
    A = cos(LatRad)*(LongRad-LongOriginRad);

    M = a*((1	- eccSquared/4		- 3*eccSquared*eccSquared/64	- 5*eccSquared*eccSquared*eccSquared/256)*LatRad
                - (3*eccSquared/8	+ 3*eccSquared*eccSquared/32	+ 45*eccSquared*eccSquared*eccSquared/1024)*sin(2*LatRad)
                                    + (15*eccSquared*eccSquared/256 + 45*eccSquared*eccSquared*eccSquared/1024)*sin(4*LatRad)
                                    - (35*eccSquared*eccSquared*eccSquared/3072)*sin(6*LatRad));

    UTMEasting = (double)(k0*N*(A+(1-T+C)*A*A*A/6
                    + (5-18*T+T*T+72*C-58*eccPrimeSquared)*A*A*A*A*A/120)
                    + 500000.0);

    UTMNorthing = (double)(k0*(M+N*tan(LatRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                 + (61-58*T+T*T+600*C-330*eccPrimeSquared)*A*A*A*A*A*A/720)));
    if(Lat < 0)
        UTMNorthing += 10000000.0; //10000000 meter offset for southern hemisphere
}

char HerramientaConversorCoordenadas::UTMLetterDesignator(double Lat)
{
//This routine determines the correct UTM letter designator for the given latitude
//returns 'Z' if latitude is outside the UTM limits of 84N to 80S
    //Written by Chuck Gantz- chuck.gantz@globalstar.com
    char LetterDesignator;

    if((84 >= Lat) && (Lat >= 72)) LetterDesignator = 'X';
    else if((72 > Lat) && (Lat >= 64)) LetterDesignator = 'W';
    else if((64 > Lat) && (Lat >= 56)) LetterDesignator = 'V';
    else if((56 > Lat) && (Lat >= 48)) LetterDesignator = 'U';
    else if((48 > Lat) && (Lat >= 40)) LetterDesignator = 'T';
    else if((40 > Lat) && (Lat >= 32)) LetterDesignator = 'S';
    else if((32 > Lat) && (Lat >= 24)) LetterDesignator = 'R';
    else if((24 > Lat) && (Lat >= 16)) LetterDesignator = 'Q';
    else if((16 > Lat) && (Lat >= 8)) LetterDesignator = 'P';
    else if(( 8 > Lat) && (Lat >= 0)) LetterDesignator = 'N';
    else if(( 0 > Lat) && (Lat >= -8)) LetterDesignator = 'M';
    else if((-8> Lat) && (Lat >= -16)) LetterDesignator = 'L';
    else if((-16 > Lat) && (Lat >= -24)) LetterDesignator = 'K';
    else if((-24 > Lat) && (Lat >= -32)) LetterDesignator = 'J';
    else if((-32 > Lat) && (Lat >= -40)) LetterDesignator = 'H';
    else if((-40 > Lat) && (Lat >= -48)) LetterDesignator = 'G';
    else if((-48 > Lat) && (Lat >= -56)) LetterDesignator = 'F';
    else if((-56 > Lat) && (Lat >= -64)) LetterDesignator = 'E';
    else if((-64 > Lat) && (Lat >= -72)) LetterDesignator = 'D';
    else if((-72 > Lat) && (Lat >= -80)) LetterDesignator = 'C';
    else LetterDesignator = 'Z'; //This is here as an error flag to show that the Latitude is outside the UTM limits

    return LetterDesignator;
}


void HerramientaConversorCoordenadas::UTMtoLL(int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, const char* UTMZone,
              double& Lat,  double& Long )
{
//converts UTM coords to lat/long.  Equations from USGS Bulletin 1532
//East Longitudes are positive, West longitudes are negative.
//North latitudes are positive, South latitudes are negative
//Lat and Long are in decimal degrees.
    //Written by Chuck Gantz- chuck.gantz@globalstar.com

    double k0 = 0.9996;
    double a = ellipsoid[ReferenceEllipsoid].EquatorialRadius;
    double eccSquared = ellipsoid[ReferenceEllipsoid].eccentricitySquared;
    double eccPrimeSquared;
    double e1 = (1-sqrt(1-eccSquared))/(1+sqrt(1-eccSquared));
    double N1, T1, C1, R1, D, M;
    double LongOrigin;
    double mu, phi1, phi1Rad;
    double x, y;
    int ZoneNumber;
    char* ZoneLetter;
    int NorthernHemisphere; //1 for northern hemispher, 0 for southern

    x = UTMEasting - 500000.0; //remove 500,000 meter offset for longitude
    y = UTMNorthing;

    ZoneNumber = strtoul(UTMZone, &ZoneLetter, 10);
    if((*ZoneLetter - 'N') >= 0)
        NorthernHemisphere = 1;//point is in northern hemisphere
    else
    {
        NorthernHemisphere = 0;//point is in southern hemisphere
        y -= 10000000.0;//remove 10,000,000 meter offset used for southern hemisphere
    }

    LongOrigin = (ZoneNumber - 1)*6 - 180 + 3;  //+3 puts origin in middle of zone

    eccPrimeSquared = (eccSquared)/(1-eccSquared);

    M = y / k0;
    mu = M/(a*(1-eccSquared/4-3*eccSquared*eccSquared/64-5*eccSquared*eccSquared*eccSquared/256));

    phi1Rad = mu	+ (3*e1/2-27*e1*e1*e1/32)*sin(2*mu)
                + (21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(4*mu)
                +(151*e1*e1*e1/96)*sin(6*mu);
    phi1 = phi1Rad*rad2deg;

    N1 = a/sqrt(1-eccSquared*sin(phi1Rad)*sin(phi1Rad));
    T1 = tan(phi1Rad)*tan(phi1Rad);
    C1 = eccPrimeSquared*cos(phi1Rad)*cos(phi1Rad);
    R1 = a*(1-eccSquared)/pow(1-eccSquared*sin(phi1Rad)*sin(phi1Rad), 1.5);
    D = x/(N1*k0);

    Lat = phi1Rad - (N1*tan(phi1Rad)/R1)*(D*D/2-(5+3*T1+10*C1-4*C1*C1-9*eccPrimeSquared)*D*D*D*D/24
                    +(61+90*T1+298*C1+45*T1*T1-252*eccPrimeSquared-3*C1*C1)*D*D*D*D*D*D/720);
    Lat = Lat * rad2deg;

    Long = (D-(1+2*T1+C1)*D*D*D/6+(5-2*C1+28*T1-3*C1*C1+8*eccPrimeSquared+24*T1*T1)
                    *D*D*D*D*D/120)/cos(phi1Rad);
    Long = LongOrigin + Long * rad2deg;

}


void HerramientaConversorCoordenadas::establecerDatosUTM(double x1, double y1, double x2, double y2, double width, double height)
{
    widthPx = width;
    heightPx = height;

}

void HerramientaConversorCoordenadas::establecerDatosLatitudLongitud(double lat1, double lng1, double lat2, double lng2, double width, double height)
{
    widthPx = width;
    heightPx = height;
    yReferencia1 = lat1;
    xReferencia1 = lng1;
    yReferencia2 = lat2;
    xReferencia2 = lng2;
    proporcionY = (yReferencia1 - yReferencia2) / height;
    proporcionX = (xReferencia1 - xReferencia2) / width;
    if (proporcionY < 0) proporcionY = proporcionY * -1.0;
    if (proporcionX < 0) proporcionX = proporcionX * -1.0;

    dYTotal = yReferencia2 - yReferencia1;
    if (dYTotal < 0) dYTotal = dYTotal * -1.0;

    dXTotal = (xReferencia2 - xReferencia1);
    if (dXTotal < 0) dXTotal = dXTotal * -1.0;

}

QPointF HerramientaConversorCoordenadas::convertirCoordenada(Enumerados::Coordenadas tipo, double x, double y, QString zona)
{
    QPointF resultado;
    if (tipo == Enumerados::LatLng) {
        resultado.setX(convertirPixelXALongitud(x));
        resultado.setY(convertirPixelYALatitud(y));
    } else {
        return convertirPixelAUTM(x, y, zona);
    }
    return resultado;
}



double HerramientaConversorCoordenadas::convertirPixelXALongitud(double x)
{
    return xReferencia1 + (x * proporcionX);
}

double HerramientaConversorCoordenadas::convertirPixelYALatitud(double y)
{
    return yReferencia1 - (y * proporcionY);
}

QPointF HerramientaConversorCoordenadas::convertirLatLonAUTM(double lat, double lon, QString& utmZona)
{
    double UTMNorthing, UTMEasting, SwissNorthing, SwissEasting;
    char utmZ[4];

    LLtoUTM(23, lat, lon, UTMNorthing, UTMEasting, utmZ);

    LLtoSwissGrid(lat, lon, SwissNorthing, SwissEasting);

    utmZona.append(utmZ);
    QPoint resultado;
    resultado.setX(UTMEasting);
    resultado.setY(UTMNorthing);
    return resultado;
}

QPointF HerramientaConversorCoordenadas::convertirPixelAUTM(double x, double y, QString& zona)
{
    char utm[4];
    double UTMNorthing, UTMEasting;
    double ln = convertirPixelXALongitud(x);
    double lt = convertirPixelYALatitud(y);    
    LLtoUTM(23, lt, ln, UTMNorthing, UTMEasting, utm);
    zona = QString(utm);
    QPoint resultado;
    resultado.setX(UTMEasting);
    resultado.setY(UTMNorthing);
    return resultado;
}

QPointF HerramientaConversorCoordenadas::convertirUTMALatLon(double x, double y, QString utmZona)
{
    double lat, lon, x1, y1;
    UTMtoLL(23, y, x, utmZona.toLatin1().data(), lat, lon);
    QPointF punto = QPointF(lon, lat);
    return punto;
}

QPointF HerramientaConversorCoordenadas::getPixel(Enumerados::Coordenadas tipo, double x, double y)
{
    QPointF punto;
    if (tipo == Enumerados::LatLng) {
        punto.setX(convertirLongitudEnPixelX(x));
        punto.setY(convertirLatitudEnPixelY(y));
    } else {

    }
    return punto;
}

double HerramientaConversorCoordenadas::convertirLatitudEnPixelY(double latitud)
{
    double dLatitud = yReferencia1 - latitud;
    double y =  ((dLatitud * heightPx) / dYTotal);
    return y;
}

double HerramientaConversorCoordenadas::convertirLongitudEnPixelX(double longitud)
{
    double dLongitud = longitud - xReferencia1;
    double x = (dLongitud * widthPx) / dXTotal;
    return x;
}

double HerramientaConversorCoordenadas::getRadianes(double valor)
{
    return (PI / 180.0) * valor;
}

double HerramientaConversorCoordenadas::getPixeles(Enumerados::Coordenadas tipo, double metros)
{
    double totalMetros = getDistanciaEntreCoordenadas(tipo, xReferencia1, yReferencia1, xReferencia2, yReferencia1);
    double pixelesCalculados = ceil((metros * widthPx) / totalMetros);
    return pixelesCalculados;
}

double HerramientaConversorCoordenadas::convertirAreaPixelesEnHectareas(QPolygonF poligono)
{
    double area = 0;
    if (poligono.size() > 2)
    {
        for (int i = 0; i <= poligono.size() - 1; i++)
        {
            QPointF p2;
            if (i == poligono.size() - 1) {
                p2 = poligono.at(0);
            } else {
                p2 = poligono.at(i + 1);
            }
            QPointF p1 = poligono.at(i);
            double lng1 = convertirPixelXALongitud(p1.x());
            double lng2 = convertirPixelXALongitud(p2.x());
            double lat1 = convertirPixelYALatitud(p1.y());
            double lat2 = convertirPixelYALatitud(p2.y());
            area += getRadianes(lng2 - lng1) * (2 + sin(getRadianes(lat1)) + sin(getRadianes(lat2)));
        }
        area = area * pow(RADIO_TIERRA_KM * 1000, 2) / 2;
    }

    if (area < 0) area = area * -1;
    return area;
}

double HerramientaConversorCoordenadas::getDistanciaEntreCoordenadas(Enumerados::Coordenadas tipo, double x1, double y1, double x2, double y2)
{
    if (tipo == Enumerados::LatLng || tipo == Enumerados::UTM) {
        double lat1rad = getRadianes(y1);
        double lat2rad = getRadianes(y2);
        double a = pow(sin(getRadianes(y2 - y1) / 2), 2) + cos(lat1rad) * cos(lat2rad) * pow(sin(getRadianes(x2-x1)/2), 2);
        double c = 2.0 * atan2(sqrt(a), sqrt(1-a));
        return c * RADIO_TIERRA_KM * 1000;
    } else if (tipo == Enumerados::Pixeles) {
        QLineF linea(x1, y1, x2, y2);
        return linea.length();
    }
    return 0.0;
}

QPointF HerramientaConversorCoordenadas::conversionPuntoACoordenada(QPointF pixel)
{
    QPointF puntoCoordenadaFinal;
    QPointF puntoReal(pixel.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(),
                              pixel.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
    double latitud = HerramientaConversorCoordenadas::getInstancia()->convertirPixelYALatitud(puntoReal.y());
    double longitud = HerramientaConversorCoordenadas::getInstancia()->convertirPixelXALongitud(puntoReal.x());
    puntoCoordenadaFinal.setY(latitud);
    puntoCoordenadaFinal.setX(longitud);
    return  puntoCoordenadaFinal;
}

QString HerramientaConversorCoordenadas::getLatitudEnTexto(double latitud)
{
    QString resultado = "";
    if (latitud < 0) {
        resultado = QString::number(latitud * -1) + QString("S");
    } else {
        resultado = QString::number(latitud) + QString("N");
    }
    return resultado;
}

QString HerramientaConversorCoordenadas::getLongitudEnTexto(double longitud)
{
    QString resultado = "";
    if (longitud < 0) {
        resultado = QString::number(longitud * -1) + QString("W");
    } else {
        resultado = QString::number(longitud) + QString("E");
    }
    return resultado;
}

double HerramientaConversorCoordenadas::convertirAngulo(qreal angulo)
{    
    angulo = -angulo + 90;
    if (angulo < 0) {
        angulo = angulo + 360;
    }    
    return angulo;
}

double HerramientaConversorCoordenadas::deconvertirAngulo(qreal angulo)
{
    double resultado = 0.0;
    if (angulo >= 0 && angulo <= 90) {
        resultado = 90 - angulo;
    } else if (angulo > 90 && angulo <= 180) {
        resultado = 360 - (angulo - 90);
    } else if (angulo > 180 && angulo <= 270) {
        resultado = 270 - (angulo - 180);
    } else {
        resultado = 180 - (angulo - 270);
    }
    return resultado;
}

void HerramientaConversorCoordenadas::establecerDatos(Enumerados::Coordenadas tipo, double lat1, double lng1, double lat2, double lng2, double width, double height)
{
    if (tipo == Enumerados::LatLng) {
        establecerDatosLatitudLongitud(lat1, lng1, lat2, lng2, width, height);
    } else {

    }
}

double HerramientaConversorCoordenadas::getProporcionHorizontal(Enumerados::Coordenadas tipo)
{
    if (tipo == Enumerados::LatLng) {
        return getProporcionX();
    } else {

    }
}

double HerramientaConversorCoordenadas::getProporcionVertical(Enumerados::Coordenadas tipo)
{
    if (tipo == Enumerados::LatLng) {
        return getProporcionY();
    } else {

    }
}

