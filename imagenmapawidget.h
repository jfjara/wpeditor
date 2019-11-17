#ifndef IMAGENMAPAWIDGET_H
#define IMAGENMAPAWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include "imagenmapa.h"
#include "idioma.h"
#include "constantes.h"
#include "herramientaconversorcoordenadas.h"

namespace Ui {
class ImagenMapaWidget;
}

class ImagenMapaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImagenMapaWidget(QString rutaHorizon, ImagenMapa* imagen, QString zona, QWidget *parent = 0, Enumerados::Coordenadas tipoCoordenada = Enumerados::LatLng);
    ~ImagenMapaWidget();

    QString rutaHorizon;
    ImagenMapa* imagen = Q_NULLPTR;
    Enumerados::Coordenadas tipoCoordenada = Enumerados::LatLng;
    QString zona;

signals:
    void borrarImagen(ImagenMapa*);
    void editarImagen(ImagenMapa*);
public slots:
    void borrarImagen();
    void editarImagen();
    void exportar();

private:
    Ui::ImagenMapaWidget *ui;
};

#endif // IMAGENMAPAWIDGET_H
