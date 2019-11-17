#ifndef CAMARA_H
#define CAMARA_H

#include <QString>

class Camara
{
public:
    Camara();

    //general
    QString marca;
    QString modelo;
    QString notas;

    int widthInPixels;
    int heightInPixels;
    double widthInMM;
    double heightInMM;
    double lensFocalLengthInMM;
    double platformGroundSpeedMetersSec;
    int fligthHeightInMeters;
    int forwardOverlapInPercent;
    int sideLapInPercent;
    int AOIWidthInMeters;
    int AOILengthInMeters;
    double gsd;
    int imageResolutionInMP;


    double getPixelsInMM();
    double getCameraShutterSpeedInSeconds();
    double getImageryGSDInCm();
    int getWidthInMeters();
    int getLengthInMeters();
    int getFlightLineSpacingInMeters();
    int getNumberOfFlightsLinesForAOI();
    int getDistanceBetweenImagesPerFlightLine();
    int getNumberOfImagesPerFlightLine();
    int getTotalNumberOfImagesForAOI();

    double getCycleTimeForShootingInSeconds();
    int getDistanceBetweenImagesInMeters();
    int getNumberImagesPerHa();
    int getImageStorageInGB();
    int getTotalDistanceInMeters();
    int getTotalFlightTimeInMinutes();

    QString getNombreCamara() { return marca + "/" + modelo; }
    QString getPropiedadesEnTexto();
};

#endif // CAMARA_H
