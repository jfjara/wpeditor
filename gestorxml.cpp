#include "gestorxml.h"
#include <QDebug>

GestorXML::GestorXML()
{

}

bool GestorXML::guardarProyecto(QString nombreFichero, DatosProyecto proyecto)
{
    QFile fichero(nombreFichero);
    fichero.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&fichero);
    xmlWriter.writeStartDocument();
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartElement("proyecto");

    xmlWriter.writeStartElement("datosproyecto");
    xmlWriter.writeAttribute("fechaCreacion", QDateTime::currentDateTime().toString("d/M/yyyy hh:mm:ss"));
    xmlWriter.writeTextElement("nombre", proyecto.nombre );
    xmlWriter.writeTextElement("localizacion", proyecto.localizacion);
    xmlWriter.writeTextElement("descripcion", proyecto.descripcion);
    if (proyecto.tipoCoordenada == Enumerados::LatLng) {
        xmlWriter.writeTextElement("tipoCoordenada", "LatLon");
    } else {
        xmlWriter.writeTextElement("tipoCoordenada", "UTM");
        xmlWriter.writeTextElement("letraZona", proyecto.letraZona);
        xmlWriter.writeTextElement("numeroZona", proyecto.numeroZona);
    }

    xmlWriter.writeTextElement("numeroAvion", QString::number(proyecto.numeroAvion));
    xmlWriter.writeTextElement("alturaWaypoints", QString::number(proyecto.alturaWaypoints));
    xmlWriter.writeTextElement("dx", QString::number(HerramientasVisor::getInstancia()->getDesplazamientoX()));
    xmlWriter.writeTextElement("dy", QString::number(HerramientasVisor::getInstancia()->getDesplazamientoY()));
    xmlWriter.writeTextElement("factorEscalado", QString::number(HerramientasVisor::getInstancia()->factorEscalado));
    xmlWriter.writeTextElement("aterrizajeCirculo", proyecto.aterrizajeCirculo ? "1" : "0");

    int enumCoordenadaIdx = Enumerados::staticMetaObject.indexOfEnumerator("Coordenadas");
    QString tipoCoordenada = QString::fromUtf8(Enumerados::staticMetaObject.enumerator(enumCoordenadaIdx).key(proyecto.tipoCoordenada));
    //xmlWriter.writeTextElement("tipoCoordenada", tipoCoordenada);

    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("mapas");
    for (int i = 0; i < proyecto.imagenesMapa.size(); i++) {
        xmlWriter.writeStartElement("mapa");
        xmlWriter.writeTextElement("pathFichero",  proyecto.imagenesMapa[i]->pathFichero);
        xmlWriter.writeTextElement("latitud1",  QString::number(proyecto.imagenesMapa[i]->latitud1));
        xmlWriter.writeTextElement("longitud1",  QString::number(proyecto.imagenesMapa[i]->longitud1));
        xmlWriter.writeTextElement("latitud2",  QString::number(proyecto.imagenesMapa[i]->latitud2));
        xmlWriter.writeTextElement("longitud2",  QString::number(proyecto.imagenesMapa[i]->longitud2));
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("configuracionVisorAlturas");
    xmlWriter.writeTextElement("alturaMaximaVisor", QString::number(proyecto.alturaMaximaVisor));
    xmlWriter.writeTextElement("alturaMaximaPermitida", QString::number(proyecto.alturaMaximaPermitida));
    xmlWriter.writeTextElement("maximaInclinacion", QString::number(proyecto.maximaInclinacion));
    xmlWriter.writeTextElement("maximaDeclinacion", QString::number(proyecto.maximaDeclinacion));
    xmlWriter.writeStartElement("lineas");
    for (int i = 0; i < proyecto.lineasAltura.size(); i++) {
        xmlWriter.writeStartElement("linea");
        xmlWriter.writeTextElement("nombre",  proyecto.lineasAltura[i]->nombre );
        xmlWriter.writeTextElement("altura", QString::number(proyecto.lineasAltura[i]->altura) );
        xmlWriter.writeTextElement("color", proyecto.lineasAltura[i]->color.name() );
        xmlWriter.writeTextElement("id", QString::number(proyecto.lineasAltura[i]->id) );
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    xmlWriter.writeEndElement();  //datos proyecto

    xmlWriter.writeStartElement("plandevuelo");
    xmlWriter.writeStartElement("waypoints");

    for (int i = 0; i < proyecto.waypointsLista.size(); i++) {
        xmlWriter.writeStartElement("waypoint");

        int enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("TipoWayPoint");
        QString tipo = QString::fromUtf8(Enumerados::staticMetaObject.enumerator(enumIdx).key(proyecto.waypointsLista[i]->tipo));
        xmlWriter.writeTextElement("tipo", tipo);
        xmlWriter.writeTextElement("bloqueado", proyecto.waypointsLista[i]->bloqueado ?  QString::number(1) :  QString::number(0));

        xmlWriter.writeTextElement("puntox", QString::number(proyecto.waypointsLista[i]->coordenadaX));
        xmlWriter.writeTextElement("puntoy", QString::number(proyecto.waypointsLista[i]->coordenadaY));

        int enumModoIdx = Enumerados::staticMetaObject.indexOfEnumerator("ModoWayPoint");
        QString modo = QString::fromUtf8(Enumerados::staticMetaObject.enumerator(enumModoIdx).key(proyecto.waypointsLista[i]->modo));
        xmlWriter.writeTextElement("modo", modo );

        xmlWriter.writeTextElement("altura", QString::number(proyecto.waypointsLista[i]->altura));
        xmlWriter.writeTextElement("orden", QString::number(proyecto.waypointsLista[i]->orden));
        xmlWriter.writeTextElement("indiceRepeticion", QString::number(proyecto.waypointsLista[i]->indiceRepeticion));

        int enumDireccionIdx = Enumerados::staticMetaObject.indexOfEnumerator("DireccionCirculo");
        QString direccion = QString::fromUtf8(Enumerados::staticMetaObject.enumerator(enumDireccionIdx).key(proyecto.waypointsLista[i]->direccionCirculo));
        xmlWriter.writeTextElement("direccionCirculo", direccion);

        xmlWriter.writeTextElement("diametro", QString::number(proyecto.waypointsLista[i]->diametro));
        xmlWriter.writeTextElement("nivelado", QString::number(proyecto.waypointsLista[i]->nivelado));
        xmlWriter.writeTextElement("cuartosCirculo", QString::number(proyecto.waypointsLista[i]->cuartosCirculo));
        xmlWriter.writeTextElement("heading", QString::number(proyecto.waypointsLista[i]->heading));
        xmlWriter.writeTextElement("headingCircuit", QString::number(proyecto.waypointsLista[i]->headingCircuit));
        int derecha = 1;
        if (!proyecto.waypointsLista[i]->uDerecha) {
            derecha = 0;
        }
        xmlWriter.writeTextElement("uDerecha", QString::number(derecha));

        xmlWriter.writeStartElement("comandosExtra");
        for (int j = 0; j < proyecto.waypointsLista[i]->comandosExtras.size(); j++) {
            xmlWriter.writeStartElement("comandoExtra");
            xmlWriter.writeTextElement("nombre",   proyecto.waypointsLista[i]->comandosExtras[j]->nombre);
            xmlWriter.writeTextElement("comando",  proyecto.waypointsLista[i]->comandosExtras[j]->comando);
            xmlWriter.writeTextElement("modo",  QString::number(proyecto.waypointsLista[i]->comandosExtras[j]->modo));
            xmlWriter.writeTextElement("valor1", QString::number( proyecto.waypointsLista[i]->comandosExtras[j]->valor1));
            xmlWriter.writeTextElement("valor2", QString::number( proyecto.waypointsLista[i]->comandosExtras[j]->valor2));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
    }

    // TODO: grids!
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();

    fichero.close();
    return true;
}


int GestorXML::leerValorEntero(QXmlStreamReader* Rxml)
{
    while (!Rxml->atEnd()) {
        if(Rxml->isEndElement())
        {
            Rxml->readNext();
            break;
        }
        else if(Rxml->isStartElement())
        {
            QString valor = Rxml->readElementText();   //Get the xml value
            Rxml->readNext();
            return valor.toInt();
        }
        else if(Rxml->isCharacters())
        {
            Rxml->readNext();
        }
        else
        {
            Rxml->readNext();
        }
    }
    return -1;
}

QString GestorXML::leerValorCadena(QXmlStreamReader* Rxml)
{
    while (!Rxml->atEnd()) {
        if(Rxml->isEndElement())
        {
            Rxml->readNext();
            break;
        }
        else if(Rxml->isStartElement())
        {
            QString valor = Rxml->readElementText();   //Get the xml value
            Rxml->readNext();
            return valor;
        }
        else if(Rxml->isCharacters())
        {
            Rxml->readNext();
        }
        else
        {
            Rxml->readNext();
        }
    }
    return QString();
}

double GestorXML::leerValorDouble(QXmlStreamReader* Rxml)
{
    while (!Rxml->atEnd()) {
        if(Rxml->isEndElement())
        {
            Rxml->readNext();
            break;
        }
        else if(Rxml->isStartElement())
        {
            QString valor = Rxml->readElementText();   //Get the xml value
            Rxml->readNext();
            return valor.toDouble();
        }
        else if(Rxml->isCharacters())
        {
            Rxml->readNext();
        }
        else
        {
            Rxml->readNext();
        }
    }
    return -1;
}

bool GestorXML::leerValorBooleano(QXmlStreamReader* Rxml)
{
    while (!Rxml->atEnd()) {
        if(Rxml->isEndElement())
        {
            Rxml->readNext();
            break;
        }
        else if(Rxml->isStartElement())
        {
            QString valor = Rxml->readElementText();   //Get the xml value
            Rxml->readNext();
            return valor.toInt() == 1 ? true : false;
        }
        else if(Rxml->isCharacters())
        {
            Rxml->readNext();
        }
        else
        {
            Rxml->readNext();
        }
    }
    return false;
}

void GestorXML::leerNodoDatosProyecto(QXmlStreamReader* Rxml, DatosProyecto* proyecto)
{
    Rxml->readNext();
    while (!Rxml->atEnd()) {
        if (Rxml->isEndElement()) {
            return;
        } else if (Rxml->isCharacters()) {
            Rxml->readNext();
        } else if (Rxml->isStartElement()) {
            if (Rxml->name() == "nombre") {
                proyecto->nombre = leerValorCadena(Rxml);
            } else if (Rxml->name() == "localizacion") {
                proyecto->localizacion = leerValorCadena(Rxml);
            } else if (Rxml->name() == "descripcion") {
                proyecto->descripcion = leerValorCadena(Rxml);
            } else if (Rxml->name() == "tipoCoordenada") {                                 
                QString tipo = leerValorCadena(Rxml);
                if (tipo == "LatLon") {
                     proyecto->tipoCoordenada = Enumerados::LatLng;
                } else if (tipo == "UTM") {
                    proyecto->tipoCoordenada = Enumerados::UTM;
                }
            } else if (Rxml->name() == "letraZona") {
                proyecto->letraZona = leerValorCadena(Rxml);
            } else if (Rxml->name() == "numeroZona") {
                proyecto->numeroZona = leerValorCadena(Rxml);
            } else if (Rxml->name() == "dx") {
                int dx = leerValorEntero(Rxml);
                HerramientasVisor::getInstancia()->acumularDesplazamiento(dx, 0);
            } else if (Rxml->name() == "dy") {
                int dy = leerValorEntero(Rxml);
                HerramientasVisor::getInstancia()->acumularDesplazamiento(0, dy);
            } else if (Rxml->name() == "factorEscalado") {
                HerramientasVisor::getInstancia()->factorEscalado = leerValorDouble(Rxml);
            } else if (Rxml->name() == "numeroAvion") {
                proyecto->numeroAvion = leerValorEntero(Rxml);
            } else if (Rxml->name() == "alturaWaypoints") {
                proyecto->alturaWaypoints = leerValorEntero(Rxml);
            } else if (Rxml->name() == "aterrizajeCirculo") {
                proyecto->aterrizajeCirculo = leerValorBooleano(Rxml);
            }
            Rxml->readNext();
        } else {
            Rxml->readNext();
        }
    }
}


Enumerados::Coordenadas GestorXML::getTipoCoordenada(QString tipo)
{
    int enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("Coordenadas");
    QMetaEnum meta = Enumerados::staticMetaObject.enumerator(enumIdx);
    for (int i = 0; i < meta.keyCount(); i++) {
        QString clave = QString::fromUtf8(meta.key(i));
        if (clave == tipo) {
            int valor = meta.keyToValue(clave.toUtf8().data());
            return static_cast<Enumerados::Coordenadas>(valor);
        }
    }
    return Enumerados::Pixeles;
}

Enumerados::ModoWayPoint GestorXML::getModoWayPoint(QString modo)
{
    int enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("ModoWayPoint");
    QMetaEnum meta = Enumerados::staticMetaObject.enumerator(enumIdx);
    for (int i = 0; i < meta.keyCount(); i++) {
        QString clave = QString::fromUtf8(meta.key(i));
        if (clave == modo) {
            int valor = meta.keyToValue(clave.toUtf8().data());
            return static_cast<Enumerados::ModoWayPoint>(valor);
        }
    }
    return Enumerados::FromTo;

}

Enumerados::DireccionCirculo GestorXML::getDireccionCirculo(QString direccion)
{
    int enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("DireccionCirculo");
    QMetaEnum meta = Enumerados::staticMetaObject.enumerator(enumIdx);
    for (int i = 0; i < meta.keyCount(); i++) {
        QString clave = QString::fromUtf8(meta.key(i));
        if (clave == direccion) {
            int valor = meta.keyToValue(clave.toUtf8().data());
            return static_cast<Enumerados::DireccionCirculo>(valor);
        }
    }
    return Enumerados::Izquierda;
}

Enumerados::TipoWayPoint GestorXML::getTipoWayPoint(QString tipo)
{
    int enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("TipoWayPoint");
    QMetaEnum meta = Enumerados::staticMetaObject.enumerator(enumIdx);
    for (int i = 0; i < meta.keyCount(); i++) {
        QString clave = QString::fromUtf8(meta.key(i));
        if (clave == tipo) {
            int valor = meta.keyToValue(clave.toUtf8().data());
            return static_cast<Enumerados::TipoWayPoint>(valor);
        }
    }
    return Enumerados::WayPoint;
}


void GestorXML::leerNodoConfiguracionVisorAlturas(QXmlStreamReader* Rxml, DatosProyecto* proyecto)
{
    Rxml->readNext();
    while (!Rxml->atEnd()) {
        if (Rxml->isEndElement()) {
            return;
        } else if (Rxml->isCharacters()) {
            Rxml->readNext();
        } else if (Rxml->isStartElement()) {
            if (Rxml->name() == "alturaMaximaVisor") {
                proyecto->alturaMaximaVisor = leerValorEntero(Rxml);
            } else if (Rxml->name() == "alturaMaximaPermitida") {
                proyecto->alturaMaximaPermitida = leerValorEntero(Rxml);
            } else if (Rxml->name() == "maximaInclinacion") {
                proyecto->maximaInclinacion = leerValorEntero(Rxml);
            } else if (Rxml->name() == "maximaDeclinacion") {
                proyecto->maximaDeclinacion = leerValorEntero(Rxml);
            } else if (Rxml->name() == "lineas") {
                leerNodoLineas(Rxml, proyecto);
            }
            Rxml->readNext();
        } else {
            Rxml->readNext();
        }
    }
}

void GestorXML::leerNodoImagenesMapa(QXmlStreamReader* Rxml, DatosProyecto* proyecto)
{
    ImagenMapa* mapa = Q_NULLPTR;
    Rxml->readNext();
    while (!Rxml->atEnd()) {
        if (Rxml->isStartElement()) {
            if (Rxml->name() == "mapa") {
                Rxml->readNext();
                mapa = new ImagenMapa();
            } else if (Rxml->name() == "pathFichero") {                
                mapa->setImagen(leerValorCadena(Rxml));
            } else if (Rxml->name() == "latitud1") {
                mapa->latitud1= leerValorDouble(Rxml);
            } else if (Rxml->name() == "longitud1") {
                mapa->longitud1 = leerValorDouble(Rxml);
            } else if (Rxml->name() == "latitud2") {
                mapa->latitud2= leerValorDouble(Rxml);
            } else if (Rxml->name() == "longitud2") {
                mapa->longitud2 = leerValorDouble(Rxml);
            } else {
                Rxml->readNext();
            }
        } else if (Rxml->isEndElement()) {
            if (Rxml->name() == "mapa") {
                //mapa->desplazar(-HerramientasVisor::getInstancia()->getDesplazamientoX(), -HerramientasVisor::getInstancia()->getDesplazamientoY());
                proyecto->imagenesMapa.push_back(mapa);
                if (proyecto->imagenesMapa.size() == 1) {
                    HerramientaConversorCoordenadas::getInstancia()->establecerDatos(Enumerados::LatLng,
                                proyecto->imagenesMapa[0]->latitud1, proyecto->imagenesMapa[0]->longitud1,
                                proyecto->imagenesMapa[0]->latitud2, proyecto->imagenesMapa[0]->longitud2,
                                proyecto->imagenesMapa[0]->widthOriginal, proyecto->imagenesMapa[0]->heigthOriginal
                                );

//                    HerramientaConversorCoordenadas::getInstancia()->establecerDatosLatitudLongitud(
//                                proyecto->imagenesMapa[0]->latitud1, proyecto->imagenesMapa[0]->longitud1,
//                                proyecto->imagenesMapa[0]->latitud2, proyecto->imagenesMapa[0]->longitud2,
//                                proyecto->imagenesMapa[0]->widthOriginal, proyecto->imagenesMapa[0]->heigthOriginal
//                                );
                }
             }
            if (Rxml->name() == "mapas") {
                return;
            }
             Rxml->readNext();
        } else {
             Rxml->readNext();
        }
    }
}

void GestorXML::leerNodoLineas(QXmlStreamReader* Rxml, DatosProyecto* proyecto)
{
    LineaAltura* linea = Q_NULLPTR;
    Rxml->readNext();
    while (!Rxml->atEnd()) {
        if (Rxml->isStartElement()) {
            if (Rxml->name() == "linea") {
                Rxml->readNext();
                linea = new LineaAltura();
            } else if (Rxml->name() == "id") {
                linea->id = leerValorEntero(Rxml);
            } else if (Rxml->name() == "nombre") {
                linea->nombre = leerValorCadena(Rxml);
            } else if (Rxml->name() == "altura") {
                linea->altura = leerValorEntero(Rxml);
            } else if (Rxml->name() == "color") {
                QString colorNombre = leerValorCadena(Rxml);
                linea->color = QColor(colorNombre);
            } else {
                Rxml->readNext();
            }

        } else if (Rxml->isEndElement()) {            
            if (Rxml->name() == "linea") {
                proyecto->lineasAltura.push_back(linea);
             }
            if (Rxml->name() == "lineas") {
                return;
            }
             Rxml->readNext();
        } else {
             Rxml->readNext();
        }
    }
}

void GestorXML::leerNodoComandosExtra(QXmlStreamReader* Rxml, WayPoint* wp)
{
    ComandoExtra* ce = Q_NULLPTR;
    Rxml->readNext();
    while (!Rxml->atEnd()) {
        if (Rxml->isStartElement()) {
            if (Rxml->name() == "comandoExtra") {
                Rxml->readNext();
                ce = new ComandoExtra();
            } else if (Rxml->name() == "nombre") {
                ce->nombre = leerValorCadena(Rxml);
            } else if (Rxml->name() == "comando") {
                ce->comando = leerValorCadena(Rxml);
            } else if (Rxml->name() == "modo") {
                ce->modo = leerValorEntero(Rxml);
            } else if (Rxml->name() == "valor1") {
                ce->valor1 = leerValorEntero(Rxml);
            } else if (Rxml->name() == "valor2") {
                ce->valor2 = leerValorEntero(Rxml);
            } else {
                Rxml->readNext();
            }
        } else if (Rxml->isEndElement()) {
            if (Rxml->name() == "comandoExtra") {
                wp->comandosExtras.push_back(ce);               
             }
            if (Rxml->name() == "comandosExtra") {
                return;
            }
             Rxml->readNext();
        } else {
             Rxml->readNext();
        }
    }
}

void GestorXML::leerNodoWayPoints(QXmlStreamReader* Rxml, DatosProyecto* proyecto)
{
    WayPoint* wp = Q_NULLPTR;
    Rxml->readNext();
    while (!Rxml->atEnd()) {
        if (Rxml->isStartElement()) {
            if (Rxml->name() == "waypoint") {                
                Rxml->readNext();
                wp = new WayPoint();
            } else if (Rxml->name() == "tipo") {
                QString tipo = leerValorCadena(Rxml);
                wp->tipo = getTipoWayPoint(tipo);
            } else if (Rxml->name() == "bloqueado") {
                wp->bloqueado = leerValorBooleano(Rxml);
            } else if (Rxml->name() == "puntox") {
                double lng = leerValorDouble(Rxml);
                wp->coordenadaX = lng;
                wp->punto.setX(HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, lng, 0).x());
                //wp->punto.setX(HerramientaConversorCoordenadas::getInstancia()->convertirLongitudEnPixelX(lng));
            } else if (Rxml->name() == "puntoy") {
                double lat = leerValorDouble(Rxml);
                wp->coordenadaY = lat;
                wp->punto.setY(HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, 0, lat).y());
                //wp->punto.setY(HerramientaConversorCoordenadas::getInstancia()->convertirLatitudEnPixelY(lat));
            } else if (Rxml->name() == "modo") {
                QString modo = leerValorCadena(Rxml);
                wp->modo = getModoWayPoint(modo);
            } else if (Rxml->name() == "altura") {
                wp->altura = leerValorDouble(Rxml);
            } else if (Rxml->name() == "orden") {
                wp->orden = leerValorEntero(Rxml);
            } else if (Rxml->name() == "") {
                wp->indiceRepeticion = leerValorEntero(Rxml);
            } else if (Rxml->name() == "direccionCirculo") {
                QString direccion = leerValorCadena(Rxml);
                wp->direccionCirculo = getDireccionCirculo(direccion);
            } else if (Rxml->name() == "diametro") {
                wp->diametro = leerValorDouble(Rxml);
            } else if (Rxml->name() == "nivelado") {
                wp->nivelado = leerValorEntero(Rxml);
            } else if (Rxml->name() == "cuartosCirculo") {
                wp->cuartosCirculo = leerValorEntero(Rxml);
            } else if (Rxml->name() == "heading") {
                wp->heading = leerValorDouble(Rxml);
            } else if (Rxml->name() == "headingCircuit") {
                wp->headingCircuit = leerValorDouble(Rxml);
            } else if (Rxml->name() == "uDerecha") {
                wp->uDerecha = leerValorEntero(Rxml) == 1 ? true : false;
            } else if (Rxml->name() == "comandosExtra") {
                leerNodoComandosExtra(Rxml, wp);
            } else {
                Rxml->readNext();                
            }

        } else if (Rxml->isEndElement()) {
            if (Rxml->name() == "waypoint") {
                proyecto->waypointsLista.push_back(wp);
             }
             Rxml->readNext();
        } else {
             Rxml->readNext();
        }
    }
}

bool GestorXML::leerProyecto(QString nombreFichero, DatosProyecto* proyecto)
{
    QFile file(nombreFichero);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        return false;
    }

    try {
        QXmlStreamReader Rxml;
        Rxml.setDevice(&file);
        Rxml.readNext();
        while (!Rxml.atEnd()) {
            if (Rxml.isStartElement()) {
                if(Rxml.name() == "proyecto") {
                    Rxml.readNext();
                } else if (Rxml.name() == "datosproyecto") {
                    GestorXML::leerNodoDatosProyecto(&Rxml, proyecto);
                    Rxml.readNext();
                } else if (Rxml.name() == "mapas") {
                    GestorXML::leerNodoImagenesMapa(&Rxml, proyecto);
                } else if (Rxml.name() == "configuracionVisorAlturas") {
                    GestorXML::leerNodoConfiguracionVisorAlturas(&Rxml, proyecto);
                } else if (Rxml.name() == "waypoints") {
                    GestorXML::leerNodoWayPoints(&Rxml, proyecto);
                } else {
                    Rxml.readNext();
                }
            } else {
                Rxml.readNext();
            }
        }
        file.close();
        if (Rxml.hasError()) {
            return false;
        } else if (file.error() != QFile::NoError) {
            return false;
        }

    } catch (...) {
        return false;
    }




    return true;
}
