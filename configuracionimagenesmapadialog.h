#ifndef CONFIGURACIONIMAGENESMAPADIALOG_H
#define CONFIGURACIONIMAGENESMAPADIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSpacerItem>
#include "imagenmapa.h"
#include "imagenmapawidget.h"
#include "mapashorizondialog.h"
#include "idioma.h"
#include "constantes.h"
#include "herramientaconversorcoordenadas.h"

namespace Ui {
class ConfiguracionImagenesMapaDialog;
}

class ConfiguracionImagenesMapaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfiguracionImagenesMapaDialog(QString rutaHorizon, QList<ImagenMapa*> listaMapas, QString zona, QWidget *parent = 0, Enumerados::Coordenadas tipoCoordenada = Enumerados::LatLng);
    ~ConfiguracionImagenesMapaDialog();

    QString rutaHorizon;
    QList<ImagenMapa*> listaMapas;
    QList<ImagenMapaWidget*> listaMapasWidgets;
    QList<ImagenMapa*> eliminados;
    Enumerados::Coordenadas tipoCoordenada = Enumerados::LatLng;
    QString zona;

    bool aceptado = false;
    bool modoEditar = false;

    void crearWidget(ImagenMapa* mapa);

private:
    Ui::ConfiguracionImagenesMapaDialog *ui;
    ImagenMapa* imagenAEditar = Q_NULLPTR;

    QSpacerItem* espaciador;

    void alinear();
    void limpiarFormulario();

public slots:
    void addImagen();
    void borrarImagen(ImagenMapa*);
    void editarImagen(ImagenMapa*);
    void seleccionarImagen();
    void aceptar();
    void abrirVentanaMapasHorizon();
};

#endif // CONFIGURACIONIMAGENESMAPADIALOG_H
