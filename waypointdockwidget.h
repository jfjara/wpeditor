#ifndef WAYPOINTWIDGET_H
#define WAYPOINTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QMessageBox>
#include <QCheckBox>
#include <QLineEdit>
#include "waypoint.h"
#include "herramientasvisor.h"
#include "herramientaconversorcoordenadas.h"
#include "idioma.h"
#include "constantes.h"
#include "gestorhistoricooperaciones.h"
#include "operacionrealizada.h"
#include "gestorcomandosextra.h"
#include "objetocomandoextraencapsulado.h"

namespace Ui {
class WayPointDockWidget;
}

class WayPointDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit WayPointDockWidget(Enumerados::Coordenadas tipo, WayPoint* wp, int numeroTotalWayPoints = 0, QString zona = "", QWidget *parent = 0);
    ~WayPointDockWidget();

    WayPoint* wp = Q_NULLPTR;
    int angulo;
    int numeroTotalWayPoints;
    QString zona;
    Enumerados::Coordenadas tipoCoordenada = Enumerados::LatLng;

    QList<ObjetoComandoExtraEncapsulado> listadoObjetoComandoExtra;
    void traducir();

private:
    Ui::WayPointDockWidget *ui;

    QMap<QString, Enumerados::TipoWayPoint> tipoWayPointMapa;
    QMap<QString, Enumerados::DireccionCirculo> direccionCirculoMapa;
    QMap<QString, Enumerados::ModoWayPoint> modoWayPointMapa;

    void rellenarDatosWayPoint();    
    void rellenarCombos();
    void rellenarComboTipoWP();
    void rellenarComboDireccion();
    void rellenarComboCuartosCirculo();
    void rellenarComboNivelado();
    void rellenarComboModoWP();
    void rellenarComboRepeticion();
    void actualizaPaneles(Enumerados::TipoWayPoint tipoWP);

    void habilitarCampos(bool valor);
    void crearOpcionesComandosExtra();
    void establecerComandosExtra();
    void rellenarComandosExtra();
    void salvarDatosComandosExtra();

signals:
    void landingEstablecido();
    void datosActualizados();
    void modificarAterrizaje(bool derecha, int pitch);

public slots:
    void cambiarTipoWayPoint();
    void cerrarPanel();
    void salvarDatosWayPoint();
    void establecerTipoAterrizajeUDerecha();
    void establecerTipoAterrizajeUIzquierda();
    void modificarHeadingAterrizaje(int v);
    void modificarRadioCirculo(int v);
    void girarRutaDial();
    void bloquearWaypoint();

};

#endif // WAYPOINTWIDGET_H
