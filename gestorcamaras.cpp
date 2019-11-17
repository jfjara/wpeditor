#include "gestorcamaras.h"
#include <QDebug>

GestorCamaras::GestorCamaras()
{
    leerConfiguracion();
}

GestorCamaras::~GestorCamaras()
{
    for (int i = 0; i < camaras.size(); i++) {
        delete camaras[i];
    }
}

void GestorCamaras::leerConfiguracion()
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QFile file(QString(RUTA_FICHERO_CAMARAS));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);

        while (!stream.atEnd()) {
            QString linea = stream.readLine();
            QString textoDescifrado = crypto.decryptToString(linea);
            Camara* camara = crearCamara(textoDescifrado);
            if (camara != Q_NULLPTR) {
                camaras.push_back(camara);
            }
        }
        file.close();
    }
}

Camara* GestorCamaras::crearCamara(QString propiedades)
{
    Camara* camara;
    if (propiedades.isEmpty()) {
        return Q_NULLPTR;
    }
    try {
        QStringList array = propiedades.split("$");
        camara = new Camara();
        camara->marca = array[CAM_POSICION_MARCA];
        camara->modelo = array[CAM_POSICION_MODELO];
        camara->notas = array[CAM_POSICION_NOTAS];
        camara->widthInPixels = QString(array[CAM_POSICION_WIDTH_PIXEL]).toInt();
        camara->heightInPixels = QString(array[CAM_POSICION_HEIGHT_PIXEL]).toInt();
        camara->widthInMM = QString(array[CAM_POSICION_WIDTH_MM]).toDouble();
        camara->heightInMM = QString(array[CAM_POSICION_HEIGHT_MM]).toDouble();
        camara->lensFocalLengthInMM =  QString(array[CAM_POSICION_LENS_FOCAL_LENGTH_MM]).toDouble();
        camara->platformGroundSpeedMetersSec = QString(array[CAM_POSICION_PLATFORM_GROUND_SPEED]).toDouble();
        camara->fligthHeightInMeters = QString(array[CAM_POSICION_FLIGHT_HEIGHT_METERS]).toInt();
        camara->forwardOverlapInPercent = QString(array[CAM_POSICION_FORWARD_OVERLAP]).toInt();
        camara->sideLapInPercent = QString(array[CAM_POSICION_SIDE_OVERLAP]).toInt();
        camara->AOIWidthInMeters = QString(array[CAM_POSICION_AOI_WIDTH_METERS]).toInt();
        camara->AOILengthInMeters = QString(array[CAM_POSICION_AOI_LENGTH_METERS]).toInt();
    } catch (...) {
        return Q_NULLPTR;
    }
    return camara;
}
