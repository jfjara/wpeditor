#ifndef CONFIGURACIONVISORDIALOG_H
#define CONFIGURACIONVISORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include "idioma.h"

namespace Ui {
class ConfiguracionVisorDialog;
}

class ConfiguracionVisorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfiguracionVisorDialog(bool verNombres, bool verAlturas, bool distanciasReales, QWidget *parent = 0);
    ~ConfiguracionVisorDialog();

    bool establecerAlturas;
    int altura;
    bool verNombres;
    bool verAlturas;
    bool distanciasReales;
    bool rangoEstablecido = false;
    int inicioRango;
    int finRango;
    bool aceptado = false;

private:
    Ui::ConfiguracionVisorDialog *ui;

public slots:
    void aceptar();
    void checkAlturasPulsado();
};

#endif // CONFIGURACIONVISORDIALOG_H
