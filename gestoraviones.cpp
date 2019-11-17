#include "gestoraviones.h"

GestorAviones::GestorAviones()
{
    leerConfiguracion();
}

GestorAviones::~GestorAviones()
{
    for (int i = 0; i < listaAviones.size(); i++) {
        delete listaAviones[i];
    }
}

Avion* GestorAviones::crearAvion(QString propiedades)
{
    Avion* avion;
    if (propiedades.isEmpty()) {
        return Q_NULLPTR;
    }
    try {
        QStringList array = propiedades.split("$");
        avion = new Avion();
        avion->empresa = array[AVION_POSICION_EMPRESA];
        avion->modelo = array[AVION_POSICION_MODELO];
        avion->version = array[AVION_POSICION_VERSION];
        avion->pitchMaximo = QString(array[AVION_POSICION_PITCH_MAXIMO]).toDouble();
        avion->pitchMinimo = QString(array[AVION_POSICION_PITCH_MINIMO]).toDouble();
        avion->velocidadCrucero = QString(array[AVION_POSICION_VELOCIDAD_CRUCERO]).toDouble();
        avion->recorridoCircuit = QString(array[AVION_POSICION_RECORRIDO_CIRCUIT]).toDouble();
        avion->baseCircuit =  QString(array[AVION_POSICION_BASE_CIRCUIT]).toDouble();
        avion->rutaImagen = array[AVION_POSICION_RUTAIMAGEN];
        avion->pathHeader = array[AVION_POSICION_HEADER];
        avion->pathFooter = array[AVION_POSICION_FOOTER];
    } catch (...) {
        return Q_NULLPTR;
    }
    return avion;
}

void GestorAviones::leerConfiguracion()
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QFile file(QString("./aviones.cfg"));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);

        while (!stream.atEnd()) {
            QString linea = stream.readLine();
            QString textoDescifrado = crypto.decryptToString(linea);
            Avion* avion = crearAvion(textoDescifrado);
            if (avion != Q_NULLPTR) {
                listaAviones.push_back(avion);
            }
        }
        file.close();
    }
}
