#include "gestortiles.h"
#include <QDebug>

GestorTiles::GestorTiles(QString pathCarpetaImagenes, QString pathFicheroReferencias, Camara* camara)
{
    this->pathCarpetaImagenes = pathCarpetaImagenes;
    this->pathFicheroReferencias = pathFicheroReferencias;
    this->camara = camara;
    double diagonalSensorMM = sqrtf((camara->widthInMM * camara->widthInMM) + (camara->heightInMM * camara->heightInMM));
    double f = diagonalSensorMM / (2.0 * camara->lensFocalLengthInMM);
    anguloLente = 2 * (atan(f) * 180.0) / PI;
}

void GestorTiles::pintar(QPainter* painter)
{
    for (int i = 0; i < tiles.size(); i++) {
        if (i != indiceImagenResaltada) {
            tiles[i]->pintar(painter);
        }
    }
    if (indiceImagenResaltada > -1) {
        tiles[indiceImagenResaltada]->pintarResaltada(painter);
    }
}

void GestorTiles::eliminarImagenPorNombre(QString nombre)
{
    for (int i = 0; i < tiles.size(); i++) {
        QStringList cachos = tiles[i]->pathFichero.split("\\");
        if (cachos[cachos.size() - 1] == nombre) {
            tiles.removeAt(i);
            indiceImagenResaltada = -1;
            return;
        }
    }
}

void GestorTiles::eliminarImagenSeleccionada()
{
    tiles.removeAt(indiceImagenResaltada);
    indiceImagenResaltada = -1;
}

void GestorTiles::marcarImagen(int i)
{
    indiceImagenResaltada = i;
}

Tile* GestorTiles::obtenerTile(QString linea)
{
    QStringList objetos = linea.split(",");
    if (objetos.size() != 4) {
        return Q_NULLPTR;
    }
    return new Tile(pathCarpetaImagenes + "\\" + objetos[0], objetos[2].toDouble(), objetos[1].toDouble(), objetos[3].toDouble(), anguloLente);
}

void GestorTiles::leerFicheroReferencias()
{
    QDir directorio(pathCarpetaImagenes);
    emit enviarTotalImagenes(directorio.count());

    QFile file(pathFicheroReferencias);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            if (cancelar) {
                tiles.clear();
                break;
            }
            QString linea = stream.readLine();
            Tile* tile = obtenerTile(linea);
            if (tile != Q_NULLPTR) {
                tiles.push_back(tile);
                emit imagenProcesada();
            }
        }
        file.close();
    }    
}

QImage GestorTiles::obtenerMosaico(int anchoMapa, int altoMapa)
{
    double este = obtenerCoordenadaDerecha();
    double oeste = obtenerCoordenadaIzquierda();
    double norte = obtenerCoordenadaArriba();
    double sur = obtenerCoordenadaAbajo();

    //calcular el factor de escalado
    double escaladoActual =  HerramientasVisor::getInstancia()->factorEscalado;


    QImage* primera = new QImage(tiles[0]->pathFichero);

    double escaladoOriginal = (primera->height() * HerramientasVisor::getInstancia()->factorEscalado) / tiles[0]->imagenEscalada.width();
    HerramientasVisor::getInstancia()->factorEscalado = 1;

    delete primera;

    bool tamCorrecto = false;
    int alto, ancho;
    QPointF esi, eid;

//    while (!tamCorrecto) {
        HerramientaConversorCoordenadas::getInstancia()->establecerDatos(Enumerados::LatLng,
                    HerramientaConversorCoordenadas::getInstancia()->yReferencia1,
                    HerramientaConversorCoordenadas::getInstancia()->xReferencia1,
                    HerramientaConversorCoordenadas::getInstancia()->yReferencia2,
                    HerramientaConversorCoordenadas::getInstancia()->xReferencia2,
                    anchoMapa *  HerramientasVisor::getInstancia()->factorEscalado,
                    altoMapa *  HerramientasVisor::getInstancia()->factorEscalado
                    );
        HerramientaConversorCoordenadas::getInstancia()->setWidth(anchoMapa * HerramientasVisor::getInstancia()->factorEscalado);
        HerramientaConversorCoordenadas::getInstancia()->setHeight(altoMapa * HerramientasVisor::getInstancia()->factorEscalado);

        esi = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, oeste, norte);
        eid = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, este, sur);

        ancho = abs(eid.x() - esi.x());
        alto = abs(esi.y() - eid.y());

//        if (ancho < 20000 && alto < 20000) {
//            tamCorrecto = true;
//        } else {
//            HerramientasVisor::getInstancia()->factorEscalado = HerramientasVisor::getInstancia()->factorEscalado - 0.1;
//        }
//    }

    QImage* mosaico = new QImage(ancho, alto, QImage::Format_RGB32);
    QImageWriter writer;
    writer.setFormat("JPG");
    writer.setFileName("c:\\salida\\imagen.jpg");

    QPixmap* pixmap = new QPixmap(ancho, alto);
    QPainter painter(pixmap);

    //QPainter painter(mosaico);
    for (int i = 0; i < tiles.size(); i++) {
        tiles[i]->pintarStitching(&painter, esi, ancho, alto);
        //delete tiles[i]->imagenOriginal;
    }
    //writer.write(*mosaico);
    pixmap->save("c:\\salida\\imagen.jpg");

    HerramientasVisor::getInstancia()->factorEscalado = escaladoActual;

    HerramientaConversorCoordenadas::getInstancia()->establecerDatos(Enumerados::LatLng,
                HerramientaConversorCoordenadas::getInstancia()->yReferencia1,
                HerramientaConversorCoordenadas::getInstancia()->xReferencia1,
                HerramientaConversorCoordenadas::getInstancia()->yReferencia2,
                HerramientaConversorCoordenadas::getInstancia()->xReferencia2,
                anchoMapa *  HerramientasVisor::getInstancia()->factorEscalado,
                altoMapa *  HerramientasVisor::getInstancia()->factorEscalado
                );
    HerramientaConversorCoordenadas::getInstancia()->setWidth(anchoMapa * HerramientasVisor::getInstancia()->factorEscalado);
    HerramientaConversorCoordenadas::getInstancia()->setHeight(altoMapa * HerramientasVisor::getInstancia()->factorEscalado);



    return QImage();

}

double GestorTiles::obtenerCoordenadaDerecha()
{
    double resultado = -1000;
    for (int i = 0; i < tiles.size(); i++) {
        int px = tiles[i]->punto.x() + tiles[i]->imagenEscalada.width();
        int py = tiles[i]->punto.y() + tiles[i]->imagenEscalada.height();
        QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(QPointF(px, py));
        if (coordenada.x() > resultado) {
            resultado = coordenada.x();
        }
    }
    return resultado;
}

double GestorTiles::obtenerCoordenadaIzquierda()
{
    double resultado = 1000;
    for (int i = 0; i < tiles.size(); i++) {
        int px = tiles[i]->punto.x() - tiles[i]->imagenEscalada.width() / 2;
        int py = tiles[i]->punto.y() - tiles[i]->imagenEscalada.height() / 2;
        QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(QPointF(px, py));
        if (coordenada.x() < resultado) {
            resultado = coordenada.x();
        }
    }
    return resultado;
}

double GestorTiles::obtenerCoordenadaArriba()
{
    double resultado = -1000;
    for (int i = 0; i < tiles.size(); i++) {
        int px = tiles[i]->punto.x() - tiles[i]->imagenEscalada.width() / 2;
        int py = tiles[i]->punto.y();// - tiles[i]->imagenEscalada.height() / 2;
        QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(QPointF(px, py));
        if (coordenada.y() > resultado) {
            resultado = coordenada.y();
        }
    }
    return resultado;
}

double GestorTiles::obtenerCoordenadaAbajo()
{
    double resultado = 1000;

    for (int i = 0; i < tiles.size(); i++) {
        int px = tiles[i]->punto.x() + tiles[i]->imagenEscalada.width();
        int py = tiles[i]->punto.y() + tiles[i]->imagenEscalada.height();
        QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->conversionPuntoACoordenada(QPointF(px, py));
        if (coordenada.y() < resultado) {
            resultado = coordenada.y();
        }
    }
    return resultado;
}






