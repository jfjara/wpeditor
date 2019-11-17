#ifndef CONFIGURACIONDIALOG_H
#define CONFIGURACIONDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include "idioma.h"

namespace Ui {
class ConfiguracionDialog;
}

class ConfiguracionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfiguracionDialog(Enumerados::IdiomasAplicacion idioma, QString ruta, bool aterrizajeCirculo, QWidget *parent = 0);
    ~ConfiguracionDialog();

    bool aceptado = false;
    Enumerados::IdiomasAplicacion idiomaSeleccionado = Enumerados::Castellano;
    QString rutaHorizon;
    bool aterrizajeCirculo;

    void traducir();

public slots:
    void aceptar();
    void seleccionarCastellano();
    void seleccionarIngles();
    void seleccionarCarpetaHorizon();

private:
    Ui::ConfiguracionDialog *ui;
};

#endif // CONFIGURACIONDIALOG_H
