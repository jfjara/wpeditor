#include "camara.h"
#include <QDebug>

Camara::Camara()
{

}

double Camara::getPixelsInMM()
{
    return widthInMM / widthInPixels;
}

double Camara::getCameraShutterSpeedInSeconds()
{
    return (getImageryGSDInCm() / 2.0) / (double)platformGroundSpeedMetersSec;
}

double Camara::getImageryGSDInCm()
{
    return getPixelsInMM() * (double)fligthHeightInMeters / lensFocalLengthInMM * 100;
}

int Camara::getWidthInMeters()
{
    return getImageryGSDInCm() * widthInPixels / 100;
}

int Camara::getLengthInMeters()
{
    return heightInPixels * getImageryGSDInCm() / 100;
}

int Camara::getFlightLineSpacingInMeters()
{
    double v = getWidthInMeters();
    return  v * (100-sideLapInPercent) / 100;
}

int Camara::getNumberOfFlightsLinesForAOI()
{
    return ceil(AOIWidthInMeters/getFlightLineSpacingInMeters()+1);
}

int Camara::getDistanceBetweenImagesPerFlightLine()
{
    return getLengthInMeters() * (100-forwardOverlapInPercent)/100;
}

int Camara::getNumberOfImagesPerFlightLine()
{
    return ceil((AOILengthInMeters / getDistanceBetweenImagesPerFlightLine()) + 1) + 4;
}

int Camara::getTotalNumberOfImagesForAOI()
{
    return getNumberOfFlightsLinesForAOI() * getNumberOfImagesPerFlightLine();
}

double Camara::getCycleTimeForShootingInSeconds()
{
    return (double)(getDistanceBetweenImagesInMeters() / platformGroundSpeedMetersSec);
}

int Camara::getDistanceBetweenImagesInMeters()
{
    return getLengthInMeters() * (100 - forwardOverlapInPercent) / 100;
}

int Camara::getNumberImagesPerHa()
{
    return getTotalNumberOfImagesForAOI() / (AOIWidthInMeters * AOILengthInMeters) * 10000;
}

int Camara::getImageStorageInGB()
{
    return imageResolutionInMP * 1000000 * 1.3/1073741824 * getTotalNumberOfImagesForAOI();
}

int Camara::getTotalDistanceInMeters()
{
    return getDistanceBetweenImagesInMeters() * getTotalNumberOfImagesForAOI();
}

int Camara::getTotalFlightTimeInMinutes()
{
    return getTotalDistanceInMeters() / platformGroundSpeedMetersSec / 60;
}

QString Camara::getPropiedadesEnTexto()
{
    QString resultado;
    resultado = marca + " $" + modelo + " $" + notas + " $" + QString::number(widthInPixels) + " $" + QString::number(heightInPixels) + " $" + QString::number(widthInMM, 'f', 2) + " $" + QString::number(heightInMM, 'f', 2) + " $" + QString::number(lensFocalLengthInMM, 'f', 2) + " $" + QString::number(platformGroundSpeedMetersSec, 'f', 2) + " $" + QString::number(fligthHeightInMeters) + " $" + QString::number(forwardOverlapInPercent) + " $" + QString::number(sideLapInPercent) + " $" + QString::number(AOIWidthInMeters) + " $" + QString::number(AOILengthInMeters);
    return resultado;
}
