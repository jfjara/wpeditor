#include "configuracionimagenesmapadialog.h"
#include "ui_configuracionimagenesmapadialog.h"

ConfiguracionImagenesMapaDialog::ConfiguracionImagenesMapaDialog(QString rutaHorizon, QList<ImagenMapa*> listaMapas, QString zona, QWidget *parent, Enumerados::Coordenadas tipoCoordenada) :
    QDialog(parent),
    ui(new Ui::ConfiguracionImagenesMapaDialog)
{
    ui->setupUi(this);
    this->rutaHorizon = rutaHorizon;
    this->listaMapas = listaMapas;
    this->tipoCoordenada = tipoCoordenada;
    this->zona = zona;

    espaciador = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
    for (int i = 0; i < listaMapas.size(); i++) {
        crearWidget(listaMapas[i]);
    }

    connect(ui->addImagenBoton, SIGNAL(clicked(bool)), this, SLOT(addImagen()));
    connect(ui->seleccionarImagenBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarImagen()));
    connect(ui->mapasHorizonBoton, SIGNAL(clicked(bool)), this, SLOT(abrirVentanaMapasHorizon()));
    connect(ui->botonera->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(aceptar()));   

    setWindowTitle(Idioma::getInstancia()->getTexto("mapas.titulo"));
    if (tipoCoordenada == Enumerados::LatLng) {
        ui->lat1Label->setText(Idioma::getInstancia()->getTexto("mapas.lat1"));
        ui->lng1Label->setText(Idioma::getInstancia()->getTexto("mapas.lng1"));
        ui->lat2Label->setText(Idioma::getInstancia()->getTexto("mapas.lat2"));
        ui->lng2Label->setText(Idioma::getInstancia()->getTexto("mapas.lng2"));
    } else {
        ui->lat1Label->setText(Idioma::getInstancia()->getTexto("mapas.utmy1"));
        ui->lng1Label->setText(Idioma::getInstancia()->getTexto("mapas.utmx1"));
        ui->lat2Label->setText(Idioma::getInstancia()->getTexto("mapas.utmy2"));
        ui->lng2Label->setText(Idioma::getInstancia()->getTexto("mapas.utmx2"));
    }
    ui->addImagenBoton->setText(Idioma::getInstancia()->getTexto("mapas.addimagen"));
    ui->seleccionarImagenBoton->setText(Idioma::getInstancia()->getTexto("mapas.seleccionar"));
    ui->mapasHorizonBoton->setText(Idioma::getInstancia()->getTexto("mapas.horizon"));

#ifdef MODO_PORTATIL
    ui->seleccionarImagenBoton->setMinimumHeight(48);
    ui->addImagenBoton->setMinimumHeight(48);
    ui->mapasHorizonBoton->setMinimumHeight(48);
#endif
}

ConfiguracionImagenesMapaDialog::~ConfiguracionImagenesMapaDialog()
{
    delete ui;
    //delete espaciador;
}

void ConfiguracionImagenesMapaDialog::abrirVentanaMapasHorizon()
{    
    MapasHorizonDialog dialogo(rutaHorizon + "/maps");
    dialogo.setModal(true);
    dialogo.exec();

    if (dialogo.aceptado) {
        for (int i = 0; i < dialogo.mapasSeleccionados.size(); i++) {
            listaMapas.push_back(dialogo.mapasSeleccionados[i]);
            crearWidget(dialogo.mapasSeleccionados[i]);
        }
    }
}


void ConfiguracionImagenesMapaDialog::seleccionarImagen()
{
    QString fichero = QFileDialog::getOpenFileName(this,
                                           Idioma::getInstancia()->getTexto("mapas.abrir.titulo"), ".",
                                           tr("Image files (*.jpg *.png *.tif *.tiff)"));
    if (fichero == "") {
        return;
    }
    ui->pathFichero->setText(fichero);
}

void ConfiguracionImagenesMapaDialog::alinear()
{
    for (int i = 0; i < ui->verticalLayout_2->count(); ++i) {
        QLayoutItem *layoutItem = ui->verticalLayout_2->itemAt(i);
        if (layoutItem->spacerItem()) {
            ui->verticalLayout_2->removeItem(layoutItem);
            return;
        }
    }
}

void ConfiguracionImagenesMapaDialog::crearWidget(ImagenMapa* mapa)
{    
    ImagenMapaWidget* widget = new ImagenMapaWidget(rutaHorizon, mapa, zona, this, tipoCoordenada);
    listaMapasWidgets.push_back(widget);
    alinear();
    ui->verticalLayout_2->addWidget(widget);
    connect(widget, SIGNAL(borrarImagen(ImagenMapa*)), this, SLOT(borrarImagen(ImagenMapa*)));
    connect(widget, SIGNAL(editarImagen(ImagenMapa*)), this, SLOT(editarImagen(ImagenMapa*)));
    ui->verticalLayout_2->addSpacerItem(espaciador);
}

void ConfiguracionImagenesMapaDialog::editarImagen(ImagenMapa* im)
{
    ui->pathFichero->setText(im->pathFichero);
    ui->lat1->setText(QString::number(im->latitud1, 'f', 6));
    ui->lat2->setText(QString::number(im->latitud2, 'f', 6));
    ui->lng1->setText(QString::number(im->longitud1, 'f', 6));
    ui->lng2->setText(QString::number(im->longitud2, 'f', 6));
    imagenAEditar = im;
    modoEditar = true;
}

void ConfiguracionImagenesMapaDialog::limpiarFormulario()
{
    ui->pathFichero->setText("");
    ui->lat1->setText("");
    ui->lat2->setText("");
    ui->lng1->setText("");
    ui->lng2->setText("");
    imagenAEditar = Q_NULLPTR;
}

void ConfiguracionImagenesMapaDialog::borrarImagen(ImagenMapa* im)
{    
    for (int i = 0; i < listaMapasWidgets.size(); i++) {
        if (listaMapasWidgets[i]->imagen->pathFichero == im->pathFichero) {
            ui->verticalLayout_2->removeWidget(listaMapasWidgets[i]);
            ui->verticalLayout_2->update();
            delete listaMapasWidgets[i];
            listaMapasWidgets.removeAt(i);
            eliminados.push_back(im);
            listaMapas.removeAt(i);
            return;
        }
    }
    update();
}

void ConfiguracionImagenesMapaDialog::aceptar()
{
    aceptado = true;
    if (tipoCoordenada == Enumerados::UTM) {
         for (int i = 0; i < listaMapas.size(); i++) {             
            QPointF punto1 = HerramientaConversorCoordenadas::getInstancia()->convertirUTMALatLon(listaMapas[i]->longitud1, listaMapas[i]->latitud1, zona);
            QPointF punto2 = HerramientaConversorCoordenadas::getInstancia()->convertirUTMALatLon(listaMapas[i]->longitud2, listaMapas[i]->latitud2, zona);
            listaMapas[i]->longitud1 = punto1.x();
            listaMapas[i]->longitud2 = punto2.x();
            listaMapas[i]->latitud1 = punto1.y();
            listaMapas[i]->latitud2 = punto2.y();
         }
    }
    hide();
}

void ConfiguracionImagenesMapaDialog::addImagen()
{
    if (ui->pathFichero->text().isNull() || ui->pathFichero->text().isEmpty()) {
        return;
    }

    if (ui->lat1->text().isNull() || ui->lat1->text().isEmpty() ||
            ui->lng1->text().isNull() || ui->lng1->text().isEmpty() ||
            ui->lat2->text().isNull() || ui->lat2->text().isEmpty() ||
            ui->lng1->text().isNull() || ui->lng1->text().isEmpty()) {
        return;
    }

    if (modoEditar) {
        //busca el correspondiente y lo modifica
        for (int i = 0; i < listaMapas.size(); i++) {
            if (listaMapas[i] == imagenAEditar) {
                listaMapas[i]->pathFichero = ui->pathFichero->text();                                
                listaMapas[i]->latitud1 = ui->lat1->text().toDouble();
                listaMapas[i]->latitud2 = ui->lat2->text().toDouble();
                listaMapas[i]->longitud1 = ui->lng1->text().toDouble();
                listaMapas[i]->longitud2 = ui->lng2->text().toDouble();
                listaMapas[i]->setImagen(ui->pathFichero->text());
                ui->verticalLayout_2->removeWidget(listaMapasWidgets[i]);
                ui->verticalLayout_2->update();
                delete listaMapasWidgets[i];
                listaMapasWidgets.removeAt(i);
                crearWidget(listaMapas[i]);
            }
        }
    } else {
        ImagenMapa* imagenMapa = new ImagenMapa();
        imagenMapa->pathFichero = ui->pathFichero->text();
        imagenMapa->latitud1 = ui->lat1->text().toDouble();
        imagenMapa->latitud2 = ui->lat2->text().toDouble();
        imagenMapa->longitud1 = ui->lng1->text().toDouble();
        imagenMapa->longitud2 = ui->lng2->text().toDouble();
        imagenMapa->setImagen(imagenMapa->pathFichero);
        listaMapas.push_back(imagenMapa);
        crearWidget(imagenMapa);
    }
    limpiarFormulario();
    modoEditar = false;
}

