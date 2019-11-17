#include "gestorultimasmapas.h"

GestorUltimasMapas* GestorUltimasMapas::instancia = Q_NULLPTR;

GestorUltimasMapas::GestorUltimasMapas()
{
    leerDatos();
}

GestorUltimasMapas* GestorUltimasMapas::getInstancia()
{
    if (instancia == Q_NULLPTR) {
        instancia = new GestorUltimasMapas();
    }
    return instancia;
}

void GestorUltimasMapas::leerDatos()
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QFile file(QString("./maps.data"));
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);


        while (!stream.atEnd()) {
            QString linea = stream.readLine();
            QString textoDescifrado = crypto.decryptToString(linea);
            QStringList array = textoDescifrado.split("$");
            if (array.size() != 5) {
                continue;
            }
            ImagenMapa* mapa = new ImagenMapa();
            mapa->pathFichero = array[0];
            mapa->latitud1 = array[1].toDouble();
            mapa->longitud1 = array[2].toDouble();
            mapa->latitud2 = array[3].toDouble();
            mapa->longitud2 = array[4].toDouble();
            mapa->setImagen(mapa->pathFichero);
            mapasPrevios.push_back(mapa);
        }
        file.close();
    }
}

void GestorUltimasMapas::limpiar()
{
    mapasPrevios.clear();
    guardar();
}

void GestorUltimasMapas::add(ImagenMapa* mapa)
{
    if (mapasPrevios.size() == 10) {
        mapasPrevios.pop_front();
    }

    for (int i = 0; i < mapasPrevios.size(); i++) {
        if (mapa->pathFichero == mapasPrevios[i]->pathFichero) {
            return;
        }
    }

    mapasPrevios.push_back(mapa);
    guardar();
}

void GestorUltimasMapas::guardar()
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QFile file("./maps.data");
    if ( file.open(QIODevice::WriteOnly))
    {
        try {
            QTextStream stream( &file );

            for (int i = 0; i < mapasPrevios.size() && i < 10; i++) {
                QString linea = mapasPrevios[i]->pathFichero + "$" + QString::number(mapasPrevios[i]->latitud1, 'f', 6) + "$" + QString::number(mapasPrevios[i]->longitud1, 'f', 6) + "$" + QString::number(mapasPrevios[i]->latitud2, 'f', 6) + "$" + QString::number(mapasPrevios[i]->longitud2, 'f', 6);
                QString encriptado = crypto.encryptToString(linea);
                stream << encriptado << endl;
            }

        } catch (...) {
            //mostrar error
            return;
        }
    }
    file.close();
}
