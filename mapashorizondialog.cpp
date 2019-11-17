#include "mapashorizondialog.h"
#include "ui_mapashorizondialog.h"
#include <QDebug>
#include "log.h"

MapasHorizonDialog::MapasHorizonDialog(QString ruta, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapasHorizonDialog)
{
    ui->setupUi(this);
    this->rutaHorizon = ruta;
    espaciador = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

    connect(ui->botonera->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(aceptar()));
    connect(ui->seleccionarBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarCarpetaImagenesHorizon()));

    setWindowTitle(Idioma::getInstancia()->getTexto("ventana.mapas.horizon.titulo"));
    ui->pathLabel->setText(Idioma::getInstancia()->getTexto("ventana.mapas.horizon.path"));
    ui->seleccionarBoton->setText(Idioma::getInstancia()->getTexto("ventana.mapas.horizon.seleccionar"));

#ifdef MODO_PORTATIL
    ui->seleccionarBoton->setMinimumHeight(48);
#endif

    cargarListadoImagenes(rutaHorizon);
}

MapasHorizonDialog::~MapasHorizonDialog()
{
    delete ui;
}

void MapasHorizonDialog::aceptar()
{    
    for (int i = 0; i < radios.size(); i++) {
        if (radios[i]->isChecked()) {
            ImagenMapa* imagenMapa = new ImagenMapa();
            QMap<QString, QString> map = informacionesMapas[i];
            imagenMapa->pathFichero = informacionesMapas[i]["imagen"];
            imagenMapa->latitud1 = informacionesMapas[i]["north"].toDouble();
            imagenMapa->latitud2 = informacionesMapas[i]["south"].toDouble();
            imagenMapa->longitud1 = informacionesMapas[i]["west"].toDouble();
            imagenMapa->longitud2 = informacionesMapas[i]["east"].toDouble();
            QImage* im = new QImage(imagenMapa->pathFichero);
            imagenMapa->imagen = im;
            imagenMapa->widthOriginal = imagenMapa->imagen->width();
            imagenMapa->heigthOriginal = imagenMapa->imagen->height();
           // Log::getInstancia()->info("Tamaño del fichero : " + QString::number(im->byteCount()));
           // Log::getInstancia()->info("PATH: " + imagenMapa->pathFichero);
           // Log::getInstancia()->info("Tamaño de la imagen -> w: " + QString::number(im->width()) + ", h: " + QString::number(im->height()));
            mapasSeleccionados.push_back(imagenMapa);
        }
    }
    aceptado = true;
}

void MapasHorizonDialog::alinear()
{
    for (int i = 0; i < ui->layoutContenido->count(); ++i) {
        QLayoutItem *layoutItem = ui->layoutContenido->itemAt(i);
        if (layoutItem->spacerItem()) {
            ui->layoutContenido->removeItem(layoutItem);
            return;
        }
    }
}

void MapasHorizonDialog::cargarListadoImagenes(QString rutaCarpeta)
{
    QDir directorio(rutaCarpeta);
    QStringList filtroInfo;
    filtroInfo << "*.info";

    if (ui->layoutContenido->count() > 0) {
        QLayoutItem *child;
        while ((child = ui->layoutContenido->takeAt(0)) != 0) {
            delete child;
        }
    }

    radios.clear();
    informacionesMapas.clear();
    foreach (QString fichero, directorio.entryList(filtroInfo, QDir::Files)) {
        QMap<QString, QString> informacion;
        getInformacionFicheroMapa(rutaCarpeta + "/" + fichero, &informacion);
        informacionesMapas.push_back(informacion);
    }

    for (int i = 0; i < informacionesMapas.size() ; i++) {
        QMap<QString, QString> informacion = informacionesMapas[i];
        QRadioButton* radio = new QRadioButton(informacion["imagen"], this);
        radios.push_back(radio);
        ui->layoutContenido->addWidget(radio);
    }
 //   ui->layoutContenido->addSpacerItem(espaciador);
}

void MapasHorizonDialog::seleccionarCarpetaImagenesHorizon()
{
    QString rutaCarpeta = QFileDialog::getExistingDirectory(this, Idioma::getInstancia()->getTexto("ventana.mapas.horizon.seleccioncarpeta"),
                                                rutaHorizon,
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    if (rutaCarpeta == "") {
        return;
    }
    ui->pathCarpetaMapasEdit->setText(rutaCarpeta);
    cargarListadoImagenes(rutaCarpeta);

}

QString MapasHorizonDialog::getValorEtiqueta(QString rutaFichero, QString seccion, QString etiqueta)
{
    QString resultado;
    QFile file(rutaFichero);
    if(!file.open(QIODevice::ReadOnly)) {
        return QString();
    }

    QTextStream in(&file);
    bool seccionEncontrada = false;
    while(!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("[")) {
            line = line.replace("[", "").replace("]", "");
            if (line == seccion) {
                seccionEncontrada = true;
            } else {
                seccionEncontrada = false;
            }
            continue;
        }
        if (seccionEncontrada) {
            if (line.startsWith(etiqueta + QString("="))) {
                QStringList cadenaFraccionada = line.split("=");
                if (cadenaFraccionada.size() > 1) {
                    resultado = cadenaFraccionada[1];
                }
                break;
            }
        }
    }
    file.close();
    return resultado;
}

bool MapasHorizonDialog::getInformacionFicheroMapa(QString rutaFichero, QMap<QString, QString>* informacion)
{
    QString xsize = getValorEtiqueta(rutaFichero, "Map Data", "xsize");
    QString ysize = getValorEtiqueta(rutaFichero, "Map Data", "ysize");
    QString north = getValorEtiqueta(rutaFichero, "Map Data", "north");
    QString south = getValorEtiqueta(rutaFichero, "Map Data", "south");
    QString east = getValorEtiqueta(rutaFichero, "Map Data", "east");
    QString west = getValorEtiqueta(rutaFichero, "Map Data", "west");
    informacion->insert("xsize", xsize);
    informacion->insert("ysize", ysize);
    informacion->insert("north", north);
    informacion->insert("south", south);
    informacion->insert("east", east);
    informacion->insert("west", west);    
    QString rutaImagen = rutaFichero.replace(".info", ".jpg");
    informacion->insert("imagen", rutaImagen);
    return true;
}

