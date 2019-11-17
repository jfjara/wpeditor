#ifndef IMPORTARFLYDIALOG_H
#define IMPORTARFLYDIALOG_H

#include <QDialog>
#include "idioma.h"
#include "imagenmapa.h"
#include "mapashorizondialog.h"
#include "constantes.h"

namespace Ui {
class ImportarFlyDialog;
}

class ImportarFlyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportarFlyDialog(QString rutaHorizon, QWidget *parent = 0);
    ~ImportarFlyDialog();

    bool aceptado = false;
    QString rutaFly;
    QString rutaMapa;
    ImagenMapa* imagenMapa;

    void traducir();

public slots:
    void seleccionarMapaHorizon();
    void seleccionarFicheroFly();
    void aceptar();
    void cancelar();

private:
    Ui::ImportarFlyDialog *ui;

    QString rutaHorizon;
};

#endif // IMPORTARFLYDIALOG_H
