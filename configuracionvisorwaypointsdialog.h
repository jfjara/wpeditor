#ifndef CONFIGURACIONVISORWAYPOINTSDIALOG_H
#define CONFIGURACIONVISORWAYPOINTSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include "enumerados.h"
#include "idioma.h"

namespace Ui {
class ConfiguracionVisorWayPointsDialog;
}

class ConfiguracionVisorWayPointsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfiguracionVisorWayPointsDialog(bool verDistanciasEntrePuntos, bool verLineasDireccion, bool distanciaTotalRecorrido, Enumerados::Coordenadas tipoCoordenada, QWidget *parent = 0);
    ~ConfiguracionVisorWayPointsDialog();

    bool verDistanciasEntrePuntos = true;
    bool distanciaTotalRecorrido = true;
    bool distanciaAcumuladaTrayecto = true;
    bool verLineasDireccion = true;
    bool aceptado = false;

    Enumerados::Coordenadas tipoCoordenada = Enumerados::Pixeles;

private:
    Ui::ConfiguracionVisorWayPointsDialog *ui;


public slots:
    void aceptar();
};

#endif // CONFIGURACIONVISORWAYPOINTSDIALOG_H
