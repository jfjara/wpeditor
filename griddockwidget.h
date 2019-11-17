#ifndef GRIDDOCKWIDGET_H
#define GRIDDOCKWIDGET_H

#include <QDockWidget>
#include <QPushButton>
#include "enumerados.h"
#include "grid.h"
#include "herramientaconversorcoordenadas.h"
#include "camara.h"
#include "idioma.h"
#include "doublevalidator.h"

namespace Ui {
class GridDockWidget;
}

class GridDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit GridDockWidget(Grid* grid, QList<Camara*> camaras, QWidget *parent = 0);
    ~GridDockWidget();

    Grid* grid;
    int angulo = 0;

    void traducir();

public slots:
    void mostrarPrevisualizacionCajaEnvolventeHAD();
    void mostrarPrevisualizacionCajaEnvolventeHAI();
    void mostrarPrevisualizacionCajaEnvolventeHABD();
    void mostrarPrevisualizacionCajaEnvolventeHABI();
    void mostrarPrevisualizacionCajaEnvolventeVAD();
    void mostrarPrevisualizacionCajaEnvolventeVAI();
    void mostrarPrevisualizacionCajaEnvolventeVABD();
    void mostrarPrevisualizacionCajaEnvolventeVABI();
    void mostrarCajaEnvolvente();
    void recargarDatosCamara(int indice);
    void cerrar();
    void aceptar();

    void girarRuta();
    void girarRutaDial();
    void mostrarPreviaRuta();
    void actualizarSolapeLateral();
    void actualizarSolapeDelantero();
    void actualizarAlturaVuelo();
    void actualizarGSD();
    void actualizarOvershot();
    void calcularRecorridoMasOptimo();

signals:
    void crearWayPointsPorGrid(Grid*);
    void girarCaja(int);

private:
    Ui::GridDockWidget *ui;

    QList<Camara*> camaras;

    void marcarBotonesCajasEnvolventes(bool marcar, QPushButton* botonPulsado);
    void crearValidadores();
};

#endif // GRIDDOCKWIDGET_H
