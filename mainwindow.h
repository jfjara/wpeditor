#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeySequence>
#include <QTreeWidgetItem>
#include <QTextCodec>
#include <QSignalMapper>
#include "visorwidget.h"
#include "visoralturasdockwidget.h"
#include "visoralturaswidget.h"
#include "waypoint.h"
#include "waypointdockwidget.h"
#include "datosproyecto.h"
#include "gestorxml.h"
#include "gestorfly.h"
#include "imagenmapa.h"
#include <QStringListModel>
#include <QFileDialog>
#include "gestorconfiguracion.h"
#include "gestorwaypoints.h"
#include "gestorgrids.h"
#include "gestorcamaras.h"
#include "gestoraviones.h"
#include "configuracionimagenesmapadialog.h"
#include "gestionaravionesdialog.h"
#include "logomenuwidget.h"
#include "acercadedialog.h"
#include "grid.h"
#include "griddockwidget.h"
#include "camarasdialog.h"
#include "camara.h"
#include "properties.h"
#include "idioma.h"
#include "configuraciondialog.h"
#include "configuracionvisorwaypointsdialog.h"
#include "gestorultimasmapas.h"
#include "importarflydialog.h"
#include "log.h"
#include "gestorhistoricooperaciones.h"
#include "constantes.h"
#include "gestorcomandosextra.h"
#include "formateador.h"
#include "doublevalidator.h"
#include "parametrosobligatoriosdialog.h"
#include "gestortiles.h"
#include "tile.h"
#include "mosaicodialog.h"
#include "barraprogresomosaicoform.h"
#include "ventanavaciadialog.h"
#include "mosaicodockwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *ev) override;

private:
    Ui::MainWindow *ui;
    VisorWidget* visor = Q_NULLPTR;
    VisorAlturasWidget* visorAlturasWidget = Q_NULLPTR;
    VisorAlturasDockWidget* visorAlturasDock = Q_NULLPTR;
    WayPointDockWidget* wpDock = Q_NULLPTR;
    GridDockWidget* gridDock = Q_NULLPTR;
    Enumerados::IdiomasAplicacion idioma = Enumerados::Castellano;
    QString rutaHorizon = "";
    DatosProyecto datosProyecto;
    QAction* accionCrearWayPoints;   
    QAction* accionCrearPoligonos;    
    QAction* accionGuardarProyecto;
    QAction* accionGuardarComoProyecto;
    QAction* accionAbrirProyecto;
    QAction* accionExportarFly;
    QAction* accionImportarFly;
    QAction* accionNuevaMision;
    QAction* accionBorrarWPSeleccionado;
    QAction* accionLimpiarMision;
    QAction* accionCamaras;
    QAction* accionConfiguracionAvion;
    QAction* accionDeshacerMenu;
    QList<QAction*> accionesMapas;

    QAction* accionGestionMapas;

    QAction* accionReglaAngulo;
    QAction* accionLeyenda;    
    QAction* accionMostrarVisorAlturas;
    QAction* accionMostrarDatosMision;
    QAction* accionOpcionesVisorWaypoints;
    QAction* accionSeleccionar;    
    GestorWayPoints* gestorWayPoints;
    GestorGrids* gestorGrids;
    GestorCamaras* gestorCamaras;
    GestorAviones* gestorAviones;
    GestorTiles* gestorTiles = Q_NULLPTR;
    QList<ImagenMapa*> mapas;
    QString version;
    QString fechaVersion;
    QAction* accionDeshacer;
    QAction* crearWPHotKey;
    QAction* crearWPAnteriorMarcadoHotKey;
    QAction* guardarProyectoHotKey;
    QAction* abrirProyectoHotKey;
    QAction* exportarProyectoHotKey;
    QAction* nuevaMisionHotKey;
    QAction* salirAplicacionHotKey;
    QAction* leyendaHotKey;
    QAction* crearGridHotKey;
    bool visualizacionPortatil = false;

    bool crearWP = false;
    bool leyendaActivada = true;
    bool crearGrid = false;
    bool medirDistancias = false;
    bool seleccionarWaypoints = false;

    MosaicoDockWidget* mosaicoDock = Q_NULLPTR;

    bool verMosaico = true;


    void crearBarraHerramientas();
    void conectarVisor();    
    void crearHotKeys();
    bool existeMapa(QString rutaMapa);
    bool establecerDatosProyecto(bool recoger);
    void actualizarWayPointsAlturas();
    QString calcularTiempoVuelo(double distancia, double velocidadCrucero);
    void reiniciarMision();
    void crearValidadores();
    void actualizarMapasMenu();
    void leerFicheroVersion();
    void activarBotones(bool valor);
    void traducir();
    Avion* getAvionSeleccionado();
    void activarBotonesEdicion(bool valor);

    void rellenarCombosCoordenadas();
    void establecerCoordenadaUTM(QString letra, QString numero);
    int getIndiceLetraAbecedario(QString letra);

public slots:
    void deshacer();
    void habilitarMedirDistanciasYAngulo();    
    void wpHotKeyPulsada();
    void gridHotKeyPulsada();
    void crearWPAnteriorMarcadoHotKeyPulsada();
    void cargarInformacionWayPoint(WayPoint*);
    void cargarInformacionGrid(Grid* grid);
    void quitarWpDock();
    void quitarGridDock();
    void quitarDockInformacion();
    void establecerLanding();
    void actualizarListaWayPointsDespuesLanding(int);
    void teclaPulsada(QKeyEvent *ev);
    void guardarProyecto();
    void abrirProyecto();
    void exportarFly();
    void importarFly();
    void aplicarCambiosConfiguracion();
    void cargarImagenMapa();
    void abrirOpciones();    
    void addWPLista(WayPoint*);
    void delWPLista(WayPoint*);
    void updateWPLista();
    void updateWPListaInformacion();
    void marcarWPVisor(const QItemSelection&);
    void marcarWPLista(int);
    void salir();
    void nuevaMision();
    void borrarWPSeleccionado();
    void limpiarMision();
    void mostrarVisorAlturas();
    void mostrarDatosMision();
    void mostrarVentanaOpcionesMenuWaypoints();
    void abrirVentanaAcercaDe();
    void abrirGestorCamaras();
    void abrirGestorAviones();
    void mostrarLeyenda();    
    void modificarAlturaMinima();
    void crearWayPointsPorGrid(Grid*);
    void modificarAterrizaje(bool derecha, int pitch);
    void recargarDatosAvion(int indice);
    void finalizarEdicion();
    void cargarMapaPrevio(QObject* mapaObjeto);
    void cargarMapaPrevio(int id);
    void actualizarTiempoDeVuelo();
    void habilitarSeleccionar();
    void modificarTipoCoordenada();
    void modificarLetraUTMCoordenada(int i);
    void modificarNumeroUTMCoordenada();
    void mostrarVentanaCrearMosaico();

    void mostrarMosaico();

};

#endif // MAINWINDOW_H
