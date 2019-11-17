#ifndef BARRAPROGRESOMOSAICOFORM_H
#define BARRAPROGRESOMOSAICOFORM_H

#include <QWidget>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include "gestortiles.h"
#include "idioma.h"


namespace Ui {
class BarraProgresoMosaicoForm;
}

class BarraProgresoMosaicoForm : public QWidget
{
    Q_OBJECT

public:
    explicit BarraProgresoMosaicoForm(GestorTiles* gestor, QWidget *parent = 0);
    ~BarraProgresoMosaicoForm();


public slots:
    void operacionFinalizada();
    void operacionCancelada();
    void establecerTotalImagenes(int total);
    void aumentarBarraProgreso();


private:
    Ui::BarraProgresoMosaicoForm *ui;   
    QFutureWatcher<void> FutureWatcher;
    GestorTiles* gestorTiles;

    int totalImagenesAProcesar = 0;
    double totalProcesado = 0.0;
    double pasoBarraProgeso = 0;

signals:
    void cerrar();
};

#endif // BARRAPROGRESOMOSAICOFORM_H
