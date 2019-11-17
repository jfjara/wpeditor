#include "tile.h"
#include <QDebug>

Tile::Tile(QString pathFichero, double latitud, double longitud, double altura, double anguloLente)
{
    this->pathFichero = pathFichero;
    //imagenOriginal = QImage(pathFichero);
    imagen = new QImage(pathFichero);
    this->latitud = latitud;
    this->longitud = longitud;
    this->altura = altura;


    //angulo de la camara
    //double anguloLente = 82.8;
    this->anguloLente = anguloLente;

    double h = altura / sin(90 - anguloLente);
    double metros = (sqrtf((h * h) - (altura * altura)) * 2) / 2;
    int w = HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, metros);

    double pxHipotenusa = sqrtf((imagen->width() * imagen->width()) + (imagen->height() * imagen->height()));
    int pw = (pxHipotenusa * w) / imagen->width();

    *imagen = imagen->scaled(pw, pw, Qt::KeepAspectRatio);
    imagenEscalada = *imagen;
    QTransform rotating;
    rotating.rotate(90);
    imagenEscalada = imagenEscalada.transformed(rotating);
}

Tile::~Tile()
{
    delete imagen;
}

void Tile::pintar(QPainter* painter)
{
    if (imagen == Q_NULLPTR) {
        return;
    }
    QPointF puntoConvertido = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud, latitud);
    puntoConvertido.setX(puntoConvertido.x() - imagenEscalada.width() / 2);
    puntoConvertido.setY(puntoConvertido.y() - imagenEscalada.height() / 2);
    punto.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoConvertido.x());
    punto.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoConvertido.y());
    painter->drawImage(punto, imagenEscalada);
}

void Tile::pintarStitching(QPainter* painter, QPointF puntoReferencia, int ancho, int alto)
{
    QPointF puntoConvertido = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud, latitud);
//    QTransform rotating;
//    rotating.rotate(90);
//    imagenOriginal = new QImage(pathFichero);
//    int w = imagenOriginal->width() / HerramientasVisor::getInstancia()->factorEscalado;
//    QImage imagenEscalada1 = imagenOriginal->scaled(w, w, Qt::KeepAspectRatio);
//    imagenEscalada1 = imagenEscalada1.transformed(rotating);

    puntoConvertido.setX(puntoConvertido.x() - puntoReferencia.x() - imagenEscalada.width() / 2);
    puntoConvertido.setY(puntoConvertido.y() - puntoReferencia.y() - imagenEscalada.height() / 2);
    painter->drawImage(puntoConvertido, imagenEscalada);

}

void Tile::pintarResaltada(QPainter* painter)
{
    if (imagen == Q_NULLPTR) {
        return;
    }
    painter->save();
    QPointF puntoConvertido = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, longitud, latitud);
    puntoConvertido.setX(puntoConvertido.x() - imagenEscalada.width() / 2);
    puntoConvertido.setY(puntoConvertido.y() - imagenEscalada.height() / 2);
    punto.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + puntoConvertido.x());
    punto.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() +  puntoConvertido.y());
    painter->drawImage(punto, imagenEscalada);

    QRectF cajaMarca(punto, QSize(imagenEscalada.width(), imagenEscalada.height()));
    painter->setPen(QPen(QColor(0xFF, 0xFF, 0x00, 0xFF), 2));
    painter->drawRect(cajaMarca);
    painter->restore();
}
