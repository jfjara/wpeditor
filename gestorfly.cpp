#include "gestorfly.h"
#include <QDebug>


int GestorFly::altura = 70;

GestorFly::GestorFly()
{

}

int GestorFly::leer(QString pathFichero, QList<WayPoint*> &waypoints)
{

    QFile file(pathFichero);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        bool bloqueEncontrado = false;
        int contador = 1;
        while (!stream.atEnd()) {
            QString linea = stream.readLine();            
            if (linea.toUpper().startsWith("IMPERIAL")) {
                waypoints.clear();
                file.close();
                return -2;
            }

            if (linea.toUpper().startsWith("TAKEOFF")) {
                bloqueEncontrado = true;
            }
            if (bloqueEncontrado) {
                WayPoint* wp = Q_NULLPTR;
                int error = procesarLinea(linea, wp, waypoints);
                if (error == -1) {
                    waypoints.clear();
                    file.close();                    
                    return -1;
                }
                if (wp != Q_NULLPTR) {
                    wp->orden = contador;
                    waypoints.push_back(wp);
                    contador++;
                }
            }
            if (linea.toUpper().contains("FIXED")) {
                break;
            }
        }
    }
    file.close();
    return 0;

}

double GestorFly::getLatitud(QString cadena)
{    
    cadena = cadena.trimmed();
    cadena = cadena.remove(cadena.size() - 1, 1);
    QChar letra = cadena[cadena.size() - 1];
    cadena = cadena.remove(cadena.size() - 1, 1);
    int valor = 1;
    if (letra.toUpper() == "S") {
        valor = -1;
    }
    return cadena.toDouble() * valor;
}

double GestorFly::getLongitud(QString cadena)
{
    cadena = cadena.trimmed();
    QChar letra = cadena[cadena.size() - 1];
    int valor = 1;
    if (letra.toUpper() == "W") {
        valor = -1;
    }
    cadena = cadena.remove(cadena.size() - 1, 1);
    double v = cadena.toDouble();
    return v * valor;
}

int GestorFly::procesarLinea(QString linea, WayPoint* &wp, QList<WayPoint*> &waypoints)
{    
    if (linea.toUpper().startsWith("FROMTO")) {
        wp = new WayPoint();
        QStringList lineaArray = linea.split("(");
        if (lineaArray.size() != 2) {
            return -1;
        }
        QStringList coordenadasArray = lineaArray[1].split(",");
        if (coordenadasArray.size() != 2) {
            return -1;
        }
        wp->tipo = Enumerados::WayPoint;
        wp->modo = Enumerados::FromTo;
        wp->coordenadaY = getLatitud(coordenadasArray[1]);
        wp->coordenadaX = getLongitud(coordenadasArray[0]);
        wp->altura = GestorFly::altura;
    } else if (linea.toUpper().startsWith("FLYTO")) {
        wp = new WayPoint();
        QStringList lineaArray = linea.split("(");
        if (lineaArray.size() != 2) {
            return -1;
        }
        QStringList coordenadasArray = lineaArray[1].split(",");
        if (coordenadasArray.size() != 2) {
            return -1;
        }
        wp->tipo = Enumerados::WayPoint;
        wp->modo = Enumerados::FlyTo;
        wp->coordenadaY = getLatitud(coordenadasArray[1]);
        wp->coordenadaX = getLongitud(coordenadasArray[0]);
        wp->altura = GestorFly::altura;
    } else if (linea.toUpper().startsWith("CIRCLELEFT")) {
        wp = new WayPoint();
        wp->tipo = Enumerados::Circulo;
        //wp->modo = Enumerados::Circulo;
        wp->direccionCirculo = Enumerados::Izquierda;
        QStringList lineaArray = linea.split("(");
        if (lineaArray.size() != 2) {
            return -1;
        }
        QStringList parametrosArray = lineaArray[1].split(",");
        /*if (parametrosArray.size() != 3) {
            return -1;
        }*/
        wp->coordenadaX = getLongitud(parametrosArray[0]);
        wp->coordenadaY = getLatitud(parametrosArray[1]);
        wp->diametro = parametrosArray[2].toInt() * 2;
        if (parametrosArray.size() == 6) {
            wp->nivelado = parametrosArray[3].toInt();
            wp->cuartosCirculo = parametrosArray[4].toInt();
            wp->heading = parametrosArray[5].toInt();
        }
        wp->altura = GestorFly::altura;
    } else if (linea.toUpper().startsWith("CIRCLERIGHT")) {
        wp = new WayPoint();
        wp->tipo = Enumerados::Circulo;
        //wp->modo = Enumerados::Circulo;
        wp->direccionCirculo = Enumerados::Derecha;
        QStringList lineaArray = linea.split("(");
        if (lineaArray.size() != 2) {
            return -1;
        }
        QStringList parametrosArray = lineaArray[1].split(",");        
        wp->coordenadaX = getLongitud(parametrosArray[0]);
        wp->coordenadaY = getLatitud(parametrosArray[1]);
        wp->diametro = parametrosArray[2].toInt() * 2;
        if (parametrosArray.size() == 6) {
            wp->nivelado = parametrosArray[3].toInt();
            wp->cuartosCirculo = parametrosArray[4].toInt();
            wp->heading = parametrosArray[5].toInt();
        }
        wp->altura = GestorFly::altura;
    } else if (linea.toUpper().startsWith("CIRCUIT")) {
        wp = new WayPoint();
        wp->tipo = Enumerados::Circuit;        
        QStringList lineaArray = linea.split("(");
        if (lineaArray.size() != 2) {
            return -1;
        }
        QStringList parametrosArray = lineaArray[1].split(",");
        if (parametrosArray.size() != 4) {
            return -1;
        }
        wp->coordenadaX = getLongitud(parametrosArray[0]);
        wp->coordenadaY = getLatitud(parametrosArray[1]);
        wp->headingCircuit = parametrosArray[2].toInt();
        wp->uDerecha = true;
        if (wp->headingCircuit < 0) {
            wp->headingCircuit  = wp->headingCircuit  * -1;
            wp->uDerecha = false;
        }

        wp->altura = 0;
    } else if (linea.toUpper().startsWith("CLIMB")) {
        QStringList lineaArray = linea.split("climb");
        GestorFly::altura = lineaArray[1].trimmed().toInt();
    } else if (GestorComandosExtra::getInstancia()->getComandoEnFly(linea) != Q_NULLPTR) {
        ComandoExtra* comandoExtra = GestorComandosExtra::getInstancia()->getComandoEnFly(linea);
        if (waypoints.size() > 0) {
            ComandoExtra* comandoExtraNuevo = new ComandoExtra();
            QStringList listaPartesCE = linea.split(" ");
            comandoExtraNuevo->comando = comandoExtra->comando;
            comandoExtraNuevo->modo = comandoExtra->modo;
            comandoExtraNuevo->nombre = comandoExtra->nombre;
            comandoExtraNuevo->valor1 = listaPartesCE[1].toInt();
            comandoExtraNuevo->valor2 = listaPartesCE[2].toInt();
            waypoints[waypoints.size() - 1]->comandosExtras.push_back(comandoExtraNuevo);
        }

    }
    return 0;
}

void GestorFly::incluirFichero(QTextStream* streamFly, QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);

        while (!stream.atEnd()) {
            QString linea = stream.readLine();
            *streamFly << linea << endl;
        }
        file.close();
    }
}

bool GestorFly::exportar(QString filename, Enumerados::Coordenadas tipo, QList<WayPoint*> waypoints, Avion* avion)
{
    if (waypoints.size() == 0) {
        return false;
    }
    QFile file(filename);
    if ( file.open(QIODevice::ReadWrite) )
    {
        try {
            QTextStream stream( &file );

            incluirFichero(&stream, avion->pathHeader);
            int altura = 0;
            stream << "metric" << endl;
            for (int i = 0; i < waypoints.size(); i++) {
                WayPoint* wp = waypoints[i];

                QString resultado;
                if (altura != wp->altura && wp->tipo != Enumerados::Circuit) {
                    altura = wp->altura;
                    resultado.append("climb " + QString::number(wp->altura));
                    stream << resultado << endl;
                }

                resultado = "";

                QString x, y;
                if (tipo == Enumerados::LatLng || tipo == Enumerados::UTM) {
                    QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(Enumerados::LatLng, wp->punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), wp->punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
                    QString letraLatitud = "N";
                    QString letraLongitud = "E";
                    double lat, lon;
                    lat = coordenada.y();
                    lon = coordenada.x();
                    if (coordenada.x() < 0) {
                         letraLongitud = "W";
                        lon *= -1;
                    }
                    if (coordenada.y() < 0) {
                        lat *= -1;
                        letraLatitud= "S";
                    }
                    y = QString::number(lat) + letraLatitud;
                    x = QString::number(lon) + letraLongitud;
                } else if (tipo == Enumerados::Pixeles) {
                    x = QString::number(wp->punto.x());
                    y = QString::number(wp->punto.y());
                }

                switch (wp->tipo) {                
                case Enumerados::WayPoint:
                    if (wp->modo == Enumerados::FromTo) {
                        resultado.append("fromTo (");
                    } else {
                        resultado.append("flyTo (");
                    }
                    resultado.append(x + ", " + y + ")");
                    break;
                case Enumerados::Circulo:
                    if (wp->direccionCirculo == Enumerados::Derecha) {
                        resultado.append("circleRight (");
                    } else {
                        resultado.append("circleLeft (");
                    }

                    resultado.append(x + ", " + y + "), " + QString::number(wp->diametro / 2));
                    if (wp->nivelado != 0 || wp->cuartosCirculo != 0 || wp->heading != 0) {
                        resultado.append(", " + QString::number(wp->nivelado) + ", " + QString::number(wp->cuartosCirculo) + ", " + QString::number(wp->heading));
                    }
                    break;
                case Enumerados::Circuit:
                    resultado.append("circuit (");

                    double h = wp->headingCircuit;
                    if (!wp->uDerecha)
                    {
                        h *= -1;
                    }

                    resultado.append(x + ", " + y + "), " + QString::number(h) + ", " + QString::number(wp->altura));
                    break;               
                }
                stream << resultado << endl;

                if (wp->comandosExtras.size() > 0) {

                    for (int j = 0; j < wp->comandosExtras.size(); j++) {
                        QString comandoExtraCadena;
                        comandoExtraCadena = wp->comandosExtras[j]->comando + " " + QString::number(wp->comandosExtras[j]->valor1) + " " + QString::number(wp->comandosExtras[j]->valor2);
                        stream << comandoExtraCadena << endl;
                    }

                }

            }
            stream << "repeat -1" << endl;
            incluirFichero(&stream, avion->pathFooter);
        } catch (...) {
            return false;
        }
    } else {
        return false;
    }
    file.close();
    return true;
}
