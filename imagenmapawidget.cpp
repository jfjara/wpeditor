#include "imagenmapawidget.h"
#include "ui_imagenmapawidget.h"
#include <QMessageBox>

ImagenMapaWidget::ImagenMapaWidget(QString rutaHorizon, ImagenMapa* imagen, QString zona, QWidget *parent, Enumerados::Coordenadas tipoCoordenada) :
    QWidget(parent),
    ui(new Ui::ImagenMapaWidget)
{
    this->rutaHorizon = rutaHorizon;
    this->imagen = imagen;
    this->tipoCoordenada = tipoCoordenada;
    this->zona = zona;
    ui->setupUi(this);

    ui->pathFichero->setText(imagen->pathFichero);

    if (tipoCoordenada == Enumerados::UTM) {
        QPointF punto1 = HerramientaConversorCoordenadas::getInstancia()->convertirLatLonAUTM(imagen->latitud1, imagen->longitud1, zona);
        QPointF punto2 = HerramientaConversorCoordenadas::getInstancia()->convertirLatLonAUTM(imagen->latitud2, imagen->longitud2, zona);
        imagen->latitud1 = punto1.y();
        imagen->longitud1 = punto1.x();
        imagen->latitud2 = punto2.y();
        imagen->longitud2 = punto2.x();
        ui->lat1Label->setText("y1:");
        ui->lat2Label->setText("y2:");
        ui->lon1Label->setText("x1:");
        ui->lon2Label->setText("x2:");
        ui->lat1->setText(QString::number(imagen->latitud1, 'f', 0));
        ui->lat2->setText(QString::number(imagen->latitud2, 'f', 0));
        ui->lng1->setText(QString::number(imagen->longitud1, 'f', 0));
        ui->lng2->setText(QString::number(imagen->longitud2, 'f', 0));
    } else {
        ui->lat1->setText(QString::number(imagen->latitud1));
        ui->lat2->setText(QString::number(imagen->latitud2));
        ui->lng1->setText(QString::number(imagen->longitud1));
        ui->lng2->setText(QString::number(imagen->longitud2));
    }

    connect(ui->borrarBoton, SIGNAL(clicked(bool)), this, SLOT(borrarImagen()));
    connect(ui->editarBoton, SIGNAL(clicked(bool)), this, SLOT(editarImagen()));
    connect(ui->exportarBoton, SIGNAL(clicked(bool)), this, SLOT(exportar()));

    ui->borrarBoton->setToolTip(Idioma::getInstancia()->getTexto("general.eliminar"));
    ui->editarBoton->setToolTip(Idioma::getInstancia()->getTexto("general.editar"));


#ifdef MODO_PORTATIL

    ui->borrarBoton->setMinimumHeight(48);
    ui->editarBoton->setMinimumHeight(48);
    ui->exportarBoton->setMinimumHeight(48);
    ui->borrarBoton->setMinimumWidth(48);
    ui->editarBoton->setMinimumWidth(48);
    ui->exportarBoton->setMinimumWidth(48);


#endif
}

ImagenMapaWidget::~ImagenMapaWidget()
{
    delete ui;
}

void ImagenMapaWidget::borrarImagen()
{
    emit borrarImagen(imagen);
}

void ImagenMapaWidget::editarImagen()
{
    emit editarImagen(imagen);
}

void ImagenMapaWidget::exportar()
{
    QString rutaCarpeta = QFileDialog::getExistingDirectory(this, Idioma::getInstancia()->getTexto("ventana.mapas.horizon.seleccioncarpeta"),
                                                rutaHorizon + "/maps",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if (rutaCarpeta == "") {
        return;
    }

    if (!imagen->exportarAHorizon(rutaCarpeta))
    {
        QMessageBox::critical(this, Idioma::getInstancia()->getTexto("ventana.exportar.horizon.error.titulo"), Idioma::getInstancia()->getTexto("ventana.exportar.horizon.error.mensaje"));
        return;
    }
    QMessageBox::information(this, Idioma::getInstancia()->getTexto("ventana.exportar.horizon.titulo"), Idioma::getInstancia()->getTexto("ventana.exportar.horizon.mensaje"));
}
