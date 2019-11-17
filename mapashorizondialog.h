#ifndef MAPASHORIZONDIALOG_H
#define MAPASHORIZONDIALOG_H

#include <QDialog>
#include <QMap>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QRadioButton>
#include <QImage>
#include "imagenmapa.h"
#include "idioma.h"
#include "constantes.h"

namespace Ui {
class MapasHorizonDialog;
}

class MapasHorizonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapasHorizonDialog(QString ruta, QWidget *parent = 0);
    ~MapasHorizonDialog();

    QString rutaHorizon = "/home";
    QList<ImagenMapa*> mapasSeleccionados;
    bool aceptado = false;

public slots:
    void aceptar();
    void seleccionarCarpetaImagenesHorizon();

private:
    Ui::MapasHorizonDialog *ui;

    QList<QMap<QString, QString>> informacionesMapas;
    QList<QRadioButton*> radios;
    QSpacerItem* espaciador;

    bool getInformacionFicheroMapa(QString rutaFichero, QMap<QString, QString>*);
    QString getValorEtiqueta(QString rutaFichero, QString seccion, QString etiqueta);
    void cargarListadoImagenes(QString ruta);
    void alinear();
};

#endif // MAPASHORIZONDIALOG_H
