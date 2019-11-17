#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    gestorWayPoints = new GestorWayPoints(&datosProyecto);
    //GestorFly::leer("C:\\Users\\jfjara\\Desktop\\der.fly", gestorWayPoints->wayPoints);
    gestorGrids = new GestorGrids();
    gestorCamaras = new GestorCamaras();
    gestorAviones = new GestorAviones();    
    GestorUltimasMapas::getInstancia();
    GestorConfiguracion::getInstancia();
    GestorComandosExtra::getInstancia();
    idioma = GestorConfiguracion::getInstancia()->idioma;
    rutaHorizon = GestorConfiguracion::getInstancia()->rutaHorizon;
    Idioma::getInstancia()->setIdioma(idioma);
    leerFicheroVersion();

    crearBarraHerramientas();
    crearHotKeys();

    if (gestorAviones->listaAviones.size() == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(Idioma::getInstancia()->getTexto("general.cfg.avion.title"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(Idioma::getInstancia()->getTexto("general.cfg.avion.mensaje"));
        msgBox.setInformativeText(Idioma::getInstancia()->getTexto("general.cfg.avion.informacion"));
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.setDefaultButton(QMessageBox::Close);
        msgBox.exec();
        exit(0);
    }

    visor = new VisorWidget(&datosProyecto, gestorWayPoints, gestorGrids, this);
    visor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->horizontalLayout->addWidget(visor);

    QStringList listaAvionesCombo;
    for (int i = 0; i < gestorAviones->listaAviones.size(); i++) {
        QString modeloAvion = gestorAviones->listaAviones[i]->modelo;
        listaAvionesCombo.push_back(modeloAvion);
    }
    ui->comboAviones->addItems(listaAvionesCombo);
    connect(ui->comboAviones, SIGNAL(currentIndexChanged(int)), this, SLOT(recargarDatosAvion(int)));
    recargarDatosAvion(0);

    visorAlturasWidget = new VisorAlturasWidget(&datosProyecto, gestorWayPoints, this);
    visorAlturasDock = new VisorAlturasDockWidget(visorAlturasWidget, this);

#ifdef MODO_PORTATIL

    visorAlturasDock->hide();
    ui->tabWidget->hide();
    accionMostrarDatosMision->setChecked(false);
    accionMostrarVisorAlturas->setChecked(false);

#else
    addDockWidget(Qt::BottomDockWidgetArea, visorAlturasDock);
    visorAlturasDock->show();
    ui->tabWidget->show();

#endif


    ui->alturaWaypointsText->setText(Formateador::formatearEntero(datosProyecto.alturaWaypoints));
    showMaximized();

    activarBotones(false);
    ui->tabWidget->removeTab(0);        
    visor->setFocus();

    crearValidadores();
    actualizarMapasMenu();
    conectarVisor();
    ui->utmGroupBox->setVisible(false);
    rellenarCombosCoordenadas();

    connect(ui->maxAlturaVisorEdit, SIGNAL(editingFinished()), this, SLOT(aplicarCambiosConfiguracion()));
    connect(ui->maxAlturaVueloEdit, SIGNAL(editingFinished()), this, SLOT(aplicarCambiosConfiguracion()));
    connect(ui->alturaWaypointsText, SIGNAL(editingFinished()), this, SLOT(modificarAlturaMinima()));
    connect(ui->coordenadasComboBox, SIGNAL(currentIndexChanged(int)), SLOT(modificarTipoCoordenada()));
    connect(ui->letraZonacomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modificarLetraUTMCoordenada(int)));
    connect(ui->numeroZonacomboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(modificarNumeroUTMCoordenada()));

    connect(ui->actionCrear_mos_ico, SIGNAL(triggered(bool)), this, SLOT(mostrarVentanaCrearMosaico()));


    traducir();

    ParametrosObligatoriosDialog dialogo;
    dialogo.setModal(true);
    dialogo.exec();

    ui->nombreProyectoEdit->setText(dialogo.nombre);
    ui->alturaWaypointsText->setText(QString::number(dialogo.alturaPredefinida));
    ui->maxAlturaVisorEdit->setText(QString::number(dialogo.alturaVisor));
    ui->maxAlturaVueloEdit->setText(QString::number(dialogo.alturaMaximaVuelo));
    datosProyecto.aterrizajeCirculo = dialogo.aterrizajeEnCirculo;
    modificarAlturaMinima();

}

void MainWindow::mostrarVentanaCrearMosaico()
{

    if (datosProyecto.imagenesMapa.size() == 0) {
        QMessageBox::critical(this, Idioma::getInstancia()->getTexto("mosaico.ventana.error.titulo"), Idioma::getInstancia()->getTexto("mosaico.ventana.error.mensaje"));
        return;
    }

    MosaicoDialog dialogo(gestorCamaras->camaras, this);
    dialogo.setModal(true);
    dialogo.exec();

    if (dialogo.aceptado) {
        QString pathCarpetaImagenes = dialogo.pathCarpeta; //"C:\\Proyectos\\fotos\\mosaico";
        QString pathFicheroMatches = dialogo.pathFichero;//"C:\\Proyectos\\fotos\\mosaico\\mosaico.txt";
        if (gestorTiles != Q_NULLPTR) {
            delete gestorTiles;
        }

        gestorTiles = new GestorTiles(pathCarpetaImagenes, pathFicheroMatches, gestorCamaras->camaras[dialogo.indiceCamara]);
        visor->gestorTiles = gestorTiles;

        //BarraProgresoMosaicoForm ventanaProgreso(gestorTiles, this);
        VentanaVaciaDialog dialogo(this);
        dialogo.setWidget(new BarraProgresoMosaicoForm(gestorTiles, this));
        dialogo.exec();

        if (mosaicoDock != Q_NULLPTR) {
            mosaicoDock->hide();
            delete mosaicoDock;
        }
        mosaicoDock = new MosaicoDockWidget(gestorTiles, this);
        mosaicoDock->show();
        gestorTiles->obtenerMosaico(datosProyecto.imagenesMapa[0]->imagen->width(), datosProyecto.imagenesMapa[0]->imagen->height());
    }
}

void MainWindow::modificarLetraUTMCoordenada(int i)
{
    QString letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    datosProyecto.letraZona = QString(letras.at(i));
}

void MainWindow::modificarNumeroUTMCoordenada()
{
    datosProyecto.numeroZona = QString::number(ui->numeroZonacomboBox->currentIndex() + 1);
}

void MainWindow::modificarTipoCoordenada()
{
    int indiceCoordenada = ui->coordenadasComboBox->currentIndex();
    if (indiceCoordenada == 0) {
        ui->utmGroupBox->setVisible(false);
        datosProyecto.tipoCoordenada = Enumerados::LatLng;
    } else {
        ui->utmGroupBox->setVisible(true);
        datosProyecto.tipoCoordenada = Enumerados::UTM;
        establecerCoordenadaUTM(datosProyecto.letraZona, datosProyecto.numeroZona);
    }
}

int MainWindow::getIndiceLetraAbecedario(QString letra)
{
    QString letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < letras.length(); i++) {
        if (QString(letras.at(i)) == letra) {
            return i;
        }
    }
    return 0;
}

void MainWindow::establecerCoordenadaUTM(QString letra, QString numero)
{
    int indice = numero.toInt() - 1;
    ui->numeroZonacomboBox->setCurrentIndex(indice);
    ui->letraZonacomboBox->setCurrentIndex(getIndiceLetraAbecedario(letra));
}

void MainWindow::rellenarCombosCoordenadas()
{
    QList<QString> itemsNumeroZona;
    QList<QString> itemsLetrasZona;
    for (int i = 1; i <= 60; i++) {
        itemsNumeroZona << QString("%1").arg(i, 2, 10, QChar('0'));
    }
    ui->numeroZonacomboBox->addItems(QStringList(itemsNumeroZona));

    QString letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < letras.length(); i++) {
        QChar letra = letras.at(i);
        itemsLetrasZona << QString(letra);
    }
    ui->letraZonacomboBox->addItems(itemsLetrasZona);

    QList<QString> itemsCoordenadas;
    itemsCoordenadas << "Lat/Long" << "UTM";
    ui->coordenadasComboBox->addItems(itemsCoordenadas);

}

void MainWindow::modificarAlturaMinima()
{
    datosProyecto.alturaWaypoints = Formateador::formatearEntero(ui->alturaWaypointsText->text());
    gestorWayPoints->alturaWaypoints = datosProyecto.alturaWaypoints;
    ui->alturaWaypointsText->setText(Formateador::formatearEntero(datosProyecto.alturaWaypoints));
}

void MainWindow::traducir()
{
    ui->menuArchivo->setTitle(Idioma::getInstancia()->getTexto("menu.archivo"));
    ui->menuWaypoints->setTitle(Idioma::getInstancia()->getTexto("menu.waypoints"));
    ui->menuGrids->setTitle(Idioma::getInstancia()->getTexto("menu.grids"));
    ui->menuConfiguraci_n->setTitle(Idioma::getInstancia()->getTexto("menu.configuracion"));
    ui->menuMapas->setTitle(Idioma::getInstancia()->getTexto("menu.mapas"));
    ui->menuAyuda->setTitle(Idioma::getInstancia()->getTexto("menu.ayuda"));
    ui->actionNueva_mision->setText(Idioma::getInstancia()->getTexto("menu.archivo.nuevo"));
    ui->actionAbrir_proyecto->setText(Idioma::getInstancia()->getTexto("menu.archivo.abrir"));
    ui->actionGuardar_proyecto->setText(Idioma::getInstancia()->getTexto("menu.archivo.guardar"));
    ui->actionGuardar_mision_como->setText(Idioma::getInstancia()->getTexto("menu.archivo.guardarcomo"));
    ui->actionExportar_a_fichero_FLY->setText(Idioma::getInstancia()->getTexto("menu.archivo.exportar"));
    ui->actionImportar_fly_a_misi_n->setText(Idioma::getInstancia()->getTexto("menu.archivo.importar"));
    ui->actionSalir->setText(Idioma::getInstancia()->getTexto("menu.archivo.salir"));
    ui->actionA_adir_waypoints->setText(Idioma::getInstancia()->getTexto("menu.waypoints.nuevo"));
    ui->actionCrear_waypoint_anterior_a_marcado->setText(Idioma::getInstancia()->getTexto("menu.waypoints.nuevoantes"));
    ui->actionBorrar_waypoint_seleccionado->setText(Idioma::getInstancia()->getTexto("menu.waypoints.borrar"));
    ui->actionBorrar_todos_los_waypoints->setText(Idioma::getInstancia()->getTexto("menu.waypoints.borrartodos"));
    ui->actionCrear_grids_Ctrl_p->setText(Idioma::getInstancia()->getTexto("menu.grids.nuevo"));
    ui->actionAviones->setText(Idioma::getInstancia()->getTexto("menu.configuracion.aviones"));
    ui->actionC_maras->setText(Idioma::getInstancia()->getTexto("menu.configuracion.camaras"));
    ui->actionAcerca_de->setText(Idioma::getInstancia()->getTexto("menu.ayuda.acercade"));
    ui->actionA_dir_mapa->setText(Idioma::getInstancia()->getTexto("menu.maps.add"));
    ui->actionOpciones->setText(Idioma::getInstancia()->getTexto("menu.configuracion.opciones"));
    ui->actionOpciones_visor->setText(Idioma::getInstancia()->getTexto("menu.configuracion.visor"));
    accionNuevaMision->setStatusTip(Idioma::getInstancia()->getTexto("menu.archivo.nuevo.descripcion"));
    accionNuevaMision->setToolTip(Idioma::getInstancia()->getTexto("menu.archivo.nuevo.descripcion"));
    accionAbrirProyecto->setStatusTip(Idioma::getInstancia()->getTexto("menu.archivo.abrir.descripcion"));
    accionAbrirProyecto->setToolTip(Idioma::getInstancia()->getTexto("menu.archivo.abrir.descripcion"));
    accionGuardarProyecto->setStatusTip(Idioma::getInstancia()->getTexto("menu.archivo.guardar.descripcion"));
    accionGuardarProyecto->setToolTip(Idioma::getInstancia()->getTexto("menu.archivo.guardar.descripcion"));
    accionGuardarComoProyecto->setStatusTip(Idioma::getInstancia()->getTexto("menu.archivo.guardarcomo.descripcion"));
    accionGuardarComoProyecto->setToolTip(Idioma::getInstancia()->getTexto("menu.archivo.guardarcomo.descripcion"));
    accionImportarFly->setToolTip(Idioma::getInstancia()->getTexto("menu.archivo.importar.descripcion"));
    accionImportarFly->setStatusTip(Idioma::getInstancia()->getTexto("menu.archivo.importar"));
    accionExportarFly->setStatusTip(Idioma::getInstancia()->getTexto("menu.archivo.exportar.descripcion"));
    accionExportarFly->setToolTip(Idioma::getInstancia()->getTexto("menu.archivo.exportar.descripcion"));
    accionDeshacerMenu->setStatusTip(Idioma::getInstancia()->getTexto("menu.deshacer.descripcion"));
    accionDeshacerMenu->setToolTip(Idioma::getInstancia()->getTexto("menu.deshacer"));
    accionGestionMapas->setStatusTip(Idioma::getInstancia()->getTexto("menu.mapas.descripcion"));
    accionGestionMapas->setToolTip(Idioma::getInstancia()->getTexto("menu.mapas"));
    accionSeleccionar->setText(Idioma::getInstancia()->getTexto("menu.seleccion"));
    accionSeleccionar->setStatusTip(Idioma::getInstancia()->getTexto("menu.Seleccion.descripcion"));
    accionCrearWayPoints->setStatusTip(Idioma::getInstancia()->getTexto("menu.waypoints.nuevo.descripcion"));
    accionCrearWayPoints->setToolTip(Idioma::getInstancia()->getTexto("menu.waypoints.nuevo.descripcion"));
    accionCrearPoligonos->setStatusTip(Idioma::getInstancia()->getTexto("menu.grids.nuevo.descripcion"));    
    accionCrearPoligonos->setToolTip(Idioma::getInstancia()->getTexto("menu.grids.nuevo.descripcion"));
    accionBorrarWPSeleccionado->setStatusTip(Idioma::getInstancia()->getTexto("menu.waypoints.borrar.descripcion"));
    accionBorrarWPSeleccionado->setToolTip(Idioma::getInstancia()->getTexto("menu.waypoints.borrar.descripcion"));
    accionLimpiarMision->setStatusTip(Idioma::getInstancia()->getTexto("menu.waypoints.borrartodos.descripcion"));
    accionLimpiarMision->setToolTip(Idioma::getInstancia()->getTexto("menu.waypoints.borrartodos.descripcion"));
    accionConfiguracionAvion->setStatusTip(Idioma::getInstancia()->getTexto("menu.configuracion.aviones.descripcion"));
    accionConfiguracionAvion->setToolTip(Idioma::getInstancia()->getTexto("menu.configuracion.aviones.descripcion"));
    accionCamaras->setStatusTip(Idioma::getInstancia()->getTexto("menu.configuracion.camaras.descripcion"));
    accionCamaras->setToolTip(Idioma::getInstancia()->getTexto("menu.configuracion.camaras.descripcion"));
    accionOpcionesVisorWaypoints->setStatusTip(Idioma::getInstancia()->getTexto("menu.configuracion.visor.descripcion"));
    accionOpcionesVisorWaypoints->setToolTip(Idioma::getInstancia()->getTexto("menu.configuracion.visor.descripcion"));
    accionReglaAngulo->setStatusTip(Idioma::getInstancia()->getTexto("menu.medidor"));
    accionReglaAngulo->setToolTip(Idioma::getInstancia()->getTexto("menu.medidor"));
    accionLeyenda->setStatusTip(Idioma::getInstancia()->getTexto("menu.leyenda.descripcion"));
    accionLeyenda->setToolTip(Idioma::getInstancia()->getTexto("menu.leyenda.descripcion"));
    accionMostrarVisorAlturas->setStatusTip(Idioma::getInstancia()->getTexto("menu.visoralturas.descripcion"));
    accionMostrarVisorAlturas->setToolTip(Idioma::getInstancia()->getTexto("menu.visoralturas.descripcion"));
    accionMostrarDatosMision->setStatusTip(Idioma::getInstancia()->getTexto("menu.datosmision.descripcion"));
    accionMostrarDatosMision->setToolTip(Idioma::getInstancia()->getTexto("menu.datosmision.descripcion"));    
    ui->tabWidget->setTabText(0, Idioma::getInstancia()->getTexto("principal.planvuelo.titulo"));
    ui->tabWidget->setTabText(1, Idioma::getInstancia()->getTexto("principal.mision.titulo"));
    ui->tabWidget->setTabText(2, Idioma::getInstancia()->getTexto("principal.configuracionvuelo.titulo"));
    ui->nombreMisionLabel->setText(Idioma::getInstancia()->getTexto("principal.mision.nombre"));
    ui->localizacionLabel->setText(Idioma::getInstancia()->getTexto("principal.mision.localizacion"));
    ui->descripcionMisionLabel->setText(Idioma::getInstancia()->getTexto("principal.mision.descripcion"));
    ui->maxAlturaVisorLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.alturavisor"));
    ui->maxAlturaVueloLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.alturavuelo"));
    ui->baseLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.baseaterrizaje"));
    ui->recorridoLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.recorridoaterrizaje"));
    ui->metros4Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->metros5Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->avionLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.avion"));
    ui->maxInclinacionAscLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.inclinacionascendente"));
    ui->maxDeclinacionLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.inclinaciondescendente"));
    ui->metros1Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->metros2Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->grados1Label->setText(Idioma::getInstancia()->getTexto("general.grados"));
    ui->grados2Label->setText(Idioma::getInstancia()->getTexto("general.grados"));
    ui->velocidadLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.velocidad"));
    ui->tiempoVueloLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.tiempovuelo"));
    ui->alturaWaypointsLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.alturawaypoints"));
    ui->metros3Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->coordenadasLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.sistemacoordenadas"));
    ui->utmGroupBox->setTitle(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.configuracionutm"));
    ui->zonaLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.zona"));


    if (accionesMapas.size() > 0) {
        accionesMapas[0]->setText(Idioma::getInstancia()->getTexto("menu.maps.add"));
    }
    if (accionesMapas.size() > 1) {
        accionesMapas[accionesMapas.size() - 1]->setText(Idioma::getInstancia()->getTexto("menu.maps.clean"));
    }

    visorAlturasDock->traducir();
    visorAlturasWidget->traducir();

    if (wpDock != Q_NULLPTR) {
        wpDock->traducir();
    }
}

void MainWindow::conectarVisor()
{
    connect(visor, SIGNAL(mostrarInformacionWP(WayPoint*)), this, SLOT(cargarInformacionWayPoint(WayPoint*)));
    connect(visor, SIGNAL(mostrarInformacionGrid(Grid*)), this, SLOT(cargarInformacionGrid(Grid*)));
    connect(visor, SIGNAL(waypointAdd(WayPoint*)), this, SLOT(addWPLista(WayPoint*)));
    connect(visor, SIGNAL(waypointDel(WayPoint*)), this, SLOT(delWPLista(WayPoint*)));
    connect(visor, SIGNAL(waypointUpdate()), this, SLOT(updateWPLista()));
    connect(visor, SIGNAL(actualizarListaDespuesDeLanding(int)), this, SLOT(actualizarListaWayPointsDespuesLanding(int)));
    connect(visor, SIGNAL(waypointVisorMarcado(int)), this, SLOT(marcarWPLista(int)));
    connect(visorAlturasWidget, SIGNAL(wapypointSeleccionado(int)), this, SLOT(marcarWPLista(int)));
    connect(visor, SIGNAL(ocultarInformacion()), this, SLOT(quitarDockInformacion()));
    connect(visor, SIGNAL(teclaPulsada(QKeyEvent*)), this, SLOT(teclaPulsada(QKeyEvent*)));
    connect(visor, SIGNAL(finalizarEdicion()), this, SLOT(finalizarEdicion()));
}


MainWindow::~MainWindow()
{
    delete ui;
    delete visor;
    delete accionDeshacer;
    delete accionDeshacerMenu;
    delete crearWPHotKey;
    delete crearGridHotKey;
    delete crearWPAnteriorMarcadoHotKey;    
    delete visorAlturasWidget;
    delete visorAlturasDock;
    delete accionCrearWayPoints;
    delete accionCrearPoligonos;
    delete accionGuardarComoProyecto;
    delete accionGuardarProyecto;    
    delete accionAbrirProyecto;
    delete accionExportarFly;
    delete accionImportarFly;
    delete accionGestionMapas;
    delete accionNuevaMision;
    delete accionBorrarWPSeleccionado;
    delete accionLimpiarMision;
    delete accionCamaras;
    delete accionConfiguracionAvion;
    delete accionReglaAngulo;
    delete leyendaHotKey;
    delete accionLeyenda;
    delete accionMostrarVisorAlturas;
    delete accionMostrarDatosMision;
    delete accionOpcionesVisorWaypoints;    
    delete accionSeleccionar;
    quitarWpDock();
    quitarGridDock();
    delete guardarProyectoHotKey;
    delete abrirProyectoHotKey;
    delete exportarProyectoHotKey;
    delete nuevaMisionHotKey;
    delete salirAplicacionHotKey;
    delete gestorWayPoints;
    delete gestorAviones;
}

void MainWindow::leerFicheroVersion()
{
    Properties propiedadesVersiones("./version.cfg");
    version = propiedadesVersiones.getValor("version");
    fechaVersion = propiedadesVersiones.getValor("fecha");
}

void MainWindow::recargarDatosAvion(int indice)
{
    Avion* avion = gestorAviones->listaAviones[indice];

    ui->maxAnguloDeclinacionEdit->setText(Formateador::formatearDecimal(avion->pitchMinimo, 2));
    ui->maxAnguloInclinacionEdit->setText(Formateador::formatearDecimal(avion->pitchMaximo, 2));
    ui->baseLineEdit->setText(Formateador::formatearDecimal(avion->baseCircuit, 2));
    ui->recorridoLineEdit->setText(Formateador::formatearDecimal(avion->recorridoCircuit, 2));
    ui->velocidadTextEdit->setText(Formateador::formatearDecimal(avion->velocidadCrucero, 2));
    datosProyecto.maximaInclinacion = avion->pitchMaximo;
    datosProyecto.maximaDeclinacion = avion->pitchMinimo;
    WayPoint* circuit =  gestorWayPoints->getCircuit();
    if (circuit != Q_NULLPTR && circuit->aterrizaje != Q_NULLPTR) {
         circuit->aterrizaje->base = avion->baseCircuit;
         circuit->aterrizaje->recorrido = avion->recorridoCircuit;
         circuit->recorridoAterrizaje =  circuit->aterrizaje->recorrido * 2 +  circuit->aterrizaje->base;

        if (datosProyecto.aterrizajeCirculo && gestorWayPoints->wayPoints.size() >= 3) {
            gestorWayPoints->wayPoints[gestorWayPoints->wayPoints.size() - 2]->diametro = circuit->aterrizaje->base * 2;
        }

    }
    datosProyecto.numeroAvion = indice;
    actualizarTiempoDeVuelo();
}

void MainWindow::actualizarTiempoDeVuelo()
{
    QString tiempo = calcularTiempoVuelo(gestorWayPoints->getDistanciaTotalVuelo(), Formateador::formatearDecimal(ui->velocidadTextEdit->text(), 2));
    ui->tiempoVueloTextEdit->setText(tiempo);
}

QString MainWindow::calcularTiempoVuelo(double distancia, double velocidadCrucero)
{
    if (velocidadCrucero <= 0 || distancia <= 0) {
        return "-";
    }
    WayPoint* circuit = gestorWayPoints->getCircuit();
    if (circuit != Q_NULLPTR && circuit->aterrizaje != Q_NULLPTR) {
        distancia += circuit->aterrizaje->base + (circuit->aterrizaje->recorrido * 2);
    }

    double segundos = ((distancia / 1000) * 3600) / velocidadCrucero;    
    QDateTime fecha(QDate(1970,1,1), QTime(0,0,0));
    fecha = fecha.addSecs(segundos);
    return fecha.toString("hh:mm:ss");
}

void MainWindow::activarBotones(bool valor)
{

    Avion* avion = getAvionSeleccionado();
    if (avion != Q_NULLPTR) {
        if (!avion->pathHeader.isEmpty() && !avion->pathHeader.isEmpty()) {
            accionExportarFly->setEnabled(valor);
        }
    }

    accionBorrarWPSeleccionado->setEnabled(valor);
    accionLimpiarMision->setEnabled(valor);
    accionReglaAngulo->setEnabled(valor);

    if (gestorWayPoints->getCircuit() == Q_NULLPTR) {
        accionCrearWayPoints->setEnabled(valor);
        accionCrearPoligonos->setEnabled(valor);

    }

}


#ifndef MODO_PORTATIL
void MainWindow::crearBarraHerramientas()
{
    accionNuevaMision = new QAction();
    accionNuevaMision->setCheckable(false);
    accionNuevaMision->setStatusTip(tr("Nueva misión", "Action"));
    accionNuevaMision->setText("Nueva misión");
    accionNuevaMision->setIcon(QIcon(":iconos/images/new.png"));
    connect(accionNuevaMision, SIGNAL(triggered()), this, SLOT(nuevaMision()));
    ui->mainToolBar->addAction(accionNuevaMision);

    accionAbrirProyecto = new QAction();
    accionAbrirProyecto->setCheckable(false);
    accionAbrirProyecto->setStatusTip(tr("Abrir misión", "Action"));
    accionAbrirProyecto->setText("Abrir misión");
    accionAbrirProyecto->setIcon(QIcon(":iconos/images/open.png"));
    connect(accionAbrirProyecto, SIGNAL(triggered()), this, SLOT(abrirProyecto()));
    ui->mainToolBar->addAction(accionAbrirProyecto);

    accionGuardarProyecto = new QAction();
    accionGuardarProyecto->setCheckable(false);
    accionGuardarProyecto->setStatusTip(tr("Guardar misión", "Action"));
    accionGuardarProyecto->setText("Guardar misión");
    accionGuardarProyecto->setIcon(QIcon(":iconos/images/save.png"));
    connect(accionGuardarProyecto, SIGNAL(triggered()), this, SLOT(guardarProyecto()));
    ui->mainToolBar->addAction(accionGuardarProyecto);

    accionGuardarComoProyecto = new QAction();
    accionGuardarComoProyecto->setCheckable(false);
    accionGuardarComoProyecto->setStatusTip(tr("Guardar misión como...", "Action"));
    accionGuardarComoProyecto->setText("Guardar misión como...");
    accionGuardarComoProyecto->setIcon(QIcon(":iconos/images/saveas.png"));
    connect(accionGuardarComoProyecto, SIGNAL(triggered()), this, SLOT(guardarProyecto()));
    connect(ui->actionGuardar_mision_como, SIGNAL(triggered()), this, SLOT(guardarProyecto()));
    ui->mainToolBar->addAction(accionGuardarComoProyecto);

    accionExportarFly = new QAction();
    accionExportarFly->setCheckable(false);
    accionExportarFly->setStatusTip(tr("Exportar proyecto a fichero fly", "Action"));
    accionExportarFly->setText("Exportar a fly");
    accionExportarFly->setIcon(QIcon(":iconos/images/export.png"));
    connect(accionExportarFly, SIGNAL(triggered()), this, SLOT(exportarFly()));
    ui->mainToolBar->addAction(accionExportarFly);

    accionImportarFly = new QAction();
    accionImportarFly->setCheckable(false);
    accionImportarFly->setStatusTip(tr("Importar fichero fly", "Action"));
    accionImportarFly->setText("Importar fichero FLY");
    accionImportarFly->setIcon(QIcon(":iconos/images/import.png"));
    connect(accionImportarFly, SIGNAL(triggered()), this, SLOT(importarFly()));
    connect(ui->actionImportar_fly_a_misi_n, SIGNAL(triggered(bool)), this, SLOT(importarFly()));
    ui->mainToolBar->addAction(accionImportarFly);

    ui->mainToolBar->addSeparator();
    //boton lineas divisorias

    accionDeshacerMenu = new QAction();
    accionDeshacerMenu->setCheckable(false);
    accionDeshacerMenu->setStatusTip(tr("Deshacer accion", "Action"));
    accionDeshacerMenu->setText("Deshacer accion");
    accionDeshacerMenu->setIcon(QIcon(":iconos/images/undo.png"));
    connect(accionDeshacerMenu, SIGNAL(triggered()), this, SLOT(deshacer()));
    ui->mainToolBar->addAction(accionDeshacerMenu);


    accionGestionMapas = new QAction();
    accionGestionMapas->setCheckable(false);
    accionGestionMapas->setStatusTip(tr("Herramienta para gestionar mapas", "Action"));
    accionGestionMapas->setText("Mapas");
    accionGestionMapas->setIcon(QIcon(":iconos/images/maps.png"));
    connect(accionGestionMapas, SIGNAL(triggered()), this, SLOT(cargarImagenMapa()));
    ui->mainToolBar->addAction(accionGestionMapas);


    accionCrearWayPoints = new QAction();
    accionCrearWayPoints->setCheckable(true);
    accionCrearWayPoints->setStatusTip(tr("Herramienta para crear WayPoints", "Action"));
    accionCrearWayPoints->setText("WayPoints");
    accionCrearWayPoints->setIcon(QIcon(":iconos/images/waypoints.png"));
    connect(accionCrearWayPoints, SIGNAL(triggered()), this, SLOT(wpHotKeyPulsada()));
    ui->mainToolBar->addAction(accionCrearWayPoints);

    //boton lineas divisorias
    accionCrearPoligonos = new QAction();
    accionCrearPoligonos->setCheckable(true);
    accionCrearPoligonos->setStatusTip(tr("Herramienta para crear grids", "Action"));
    accionCrearPoligonos->setText("Grid");
    accionCrearPoligonos->setIcon(QIcon(":iconos/images/grid.png"));
    connect(accionCrearPoligonos, SIGNAL(triggered()), this, SLOT(gridHotKeyPulsada()));
    ui->mainToolBar->addAction(accionCrearPoligonos);

    accionBorrarWPSeleccionado = new QAction();
    accionBorrarWPSeleccionado->setCheckable(false);
    accionBorrarWPSeleccionado->setStatusTip(tr("Eliminar waypoint seleccionado", "Action"));
    accionBorrarWPSeleccionado->setText("Eliminar waypoint seleccionado");
    accionBorrarWPSeleccionado->setIcon(QIcon(":iconos/images/rubber.png"));
    connect(accionBorrarWPSeleccionado, SIGNAL(triggered()), this, SLOT(borrarWPSeleccionado()));
    connect(ui->actionBorrar_waypoint_seleccionado, SIGNAL(triggered(bool)), this, SLOT(borrarWPSeleccionado()));
    ui->mainToolBar->addAction(accionBorrarWPSeleccionado);

    accionLimpiarMision = new QAction();
    accionLimpiarMision->setCheckable(false);
    accionLimpiarMision->setStatusTip(tr("Eliminar elementos de la misión", "Action"));
    accionLimpiarMision->setText("Eliminar elementos de la misión");
    accionLimpiarMision->setIcon(QIcon(":iconos/images/clean.png"));
    connect(accionLimpiarMision, SIGNAL(triggered()), this, SLOT(limpiarMision()));
    connect(ui->actionBorrar_todos_los_waypoints, SIGNAL(triggered(bool)), this, SLOT(limpiarMision()));
    ui->mainToolBar->addAction(accionLimpiarMision);

    accionReglaAngulo = new QAction();
    accionReglaAngulo->setCheckable(true);
    accionReglaAngulo->setText(tr("Regla y ángulo", "Action"));
    accionReglaAngulo->setStatusTip("Herramienta para medir distancias y ángulos");
    accionReglaAngulo->setIcon(QIcon(":iconos/images/ruler.png"));
    connect(accionReglaAngulo, SIGNAL(triggered()), this, SLOT(habilitarMedirDistanciasYAngulo()));
    ui->mainToolBar->addAction(accionReglaAngulo);

    connect(ui->actionAcerca_de, SIGNAL(triggered(bool)), this, SLOT(abrirVentanaAcercaDe()));

    accionSeleccionar = new QAction();
    accionSeleccionar->setCheckable(true);
    accionSeleccionar->setText(tr("Seleccionar", "Action"));
    accionSeleccionar->setStatusTip("Herramienta para seleccionar waypoints");
    accionSeleccionar->setIcon(QIcon(":iconos/images/select.png"));
    connect(accionSeleccionar, SIGNAL(triggered()), this, SLOT(habilitarSeleccionar()));
    ui->mainToolBar->addAction(accionSeleccionar);

    ui->mainToolBar->addSeparator();

    accionCamaras = new QAction();
    accionCamaras->setCheckable(false);
    accionCamaras->setStatusTip(tr("Gestionar cámaras", "Action"));
    accionCamaras->setText("Gestionar cámaras");
    accionCamaras->setIcon(QIcon(":iconos/images/camera.png"));
    connect(accionCamaras, SIGNAL(triggered()), this, SLOT(abrirGestorCamaras()));
    connect(ui->actionC_maras, SIGNAL(triggered()), this, SLOT(abrirGestorCamaras()));
    ui->mainToolBar->addAction(accionCamaras);

    accionConfiguracionAvion = new QAction();
    accionConfiguracionAvion->setCheckable(false);
    accionConfiguracionAvion->setStatusTip(tr("Gestionar aviones", "Action"));
    accionConfiguracionAvion->setText("Gestionar aviones");
    accionConfiguracionAvion->setIcon(QIcon(":iconos/images/plane.png"));
    connect(accionConfiguracionAvion, SIGNAL(triggered()), this, SLOT(abrirGestorAviones()));
    connect(ui->actionAviones, SIGNAL(triggered(bool)), this, SLOT(abrirGestorAviones()));
    ui->mainToolBar->addAction(accionConfiguracionAvion);       

    accionOpcionesVisorWaypoints = new QAction();
    accionOpcionesVisorWaypoints->setCheckable(false);
    accionOpcionesVisorWaypoints->setStatusTip(tr("Opciones visor waypoints", "Action"));
    accionOpcionesVisorWaypoints->setText("Mostrar opciones del visor principal");
    accionOpcionesVisorWaypoints->setIcon(QIcon(":iconos/images/configviewer.png"));
    connect(accionOpcionesVisorWaypoints, SIGNAL(triggered()), this, SLOT(mostrarVentanaOpcionesMenuWaypoints()));
    connect(ui->actionOpciones_visor, SIGNAL(triggered(bool)), this, SLOT(mostrarVentanaOpcionesMenuWaypoints()));
    ui->mainToolBar->addAction(accionOpcionesVisorWaypoints);

    ui->mainToolBar->addSeparator();

    accionLeyenda = new QAction();
    accionLeyenda->setCheckable(true);
    accionLeyenda->setChecked(true);
    accionLeyenda->setText(tr("Leyenda", "Action"));
    accionLeyenda->setStatusTip("Muestra la leyenda en el visor");
    accionLeyenda->setIcon(QIcon(":iconos/images/leyend.png"));
    connect(accionLeyenda, SIGNAL(triggered()), this, SLOT(mostrarLeyenda()));
    ui->mainToolBar->addAction(accionLeyenda);

    accionMostrarVisorAlturas = new QAction();
    accionMostrarVisorAlturas->setCheckable(true);
    accionMostrarVisorAlturas->setChecked(true);
    accionMostrarVisorAlturas->setText(tr("Mostrar visor alturas", "Action"));
    accionMostrarVisorAlturas->setStatusTip("Muestra u oculta el visor de alturas");
    accionMostrarVisorAlturas->setIcon(QIcon(":iconos/images/mostrarVisorAlturas.png"));
    connect(accionMostrarVisorAlturas, SIGNAL(triggered()), this, SLOT(mostrarVisorAlturas()));
    ui->mainToolBar->addAction(accionMostrarVisorAlturas);

    accionMostrarDatosMision = new QAction();
    accionMostrarDatosMision->setCheckable(true);
    accionMostrarDatosMision->setChecked(true);
    accionMostrarDatosMision->setText(tr("Mostrar datos misión", "Action"));
    accionMostrarDatosMision->setStatusTip("Muestra u oculta los datos de la misión");
    accionMostrarDatosMision->setIcon(QIcon(":iconos/images/mostrarTab.png"));
    connect(accionMostrarDatosMision, SIGNAL(triggered()), this, SLOT(mostrarDatosMision()));
    ui->mainToolBar->addAction(accionMostrarDatosMision);

    connect(ui->actionOpciones, SIGNAL(triggered(bool)), this, SLOT(abrirOpciones()));

    connect(ui->actionVer_mosaico, SIGNAL(triggered(bool)), this, SLOT(mostrarMosaico()));

    ui->mainToolBar->addWidget(new LogoMenuWidget());
}

void MainWindow::mostrarMosaico()
{
    if (ui->actionVer_mosaico->isChecked()) {
        visor->verMosaico = true;
    } else {
        visor->verMosaico = false;
    }
}

#else
void MainWindow::crearBarraHerramientas()
{

    ui->menuBar->setVisible(false);
    ui->mainToolBar->setIconSize(QSize(48,48));

    accionNuevaMision = new QAction();
    accionNuevaMision->setCheckable(false);
    accionNuevaMision->setStatusTip(tr("Nueva misión", "Action"));
    accionNuevaMision->setText("Nueva misión");
    accionNuevaMision->setIcon(QIcon(":iconos/images/new.png"));
    connect(accionNuevaMision, SIGNAL(triggered()), this, SLOT(nuevaMision()));
    ui->mainToolBar->addAction(accionNuevaMision);

    accionAbrirProyecto = new QAction();
    accionAbrirProyecto->setCheckable(false);
    accionAbrirProyecto->setStatusTip(tr("Abrir misión", "Action"));
    accionAbrirProyecto->setText("Abrir misión");
    accionAbrirProyecto->setIcon(QIcon(":iconos/images/open.png"));
    connect(accionAbrirProyecto, SIGNAL(triggered()), this, SLOT(abrirProyecto()));
    ui->mainToolBar->addAction(accionAbrirProyecto);

    accionGuardarProyecto = new QAction();
    accionGuardarProyecto->setCheckable(false);
    accionGuardarProyecto->setStatusTip(tr("Guardar misión", "Action"));
    accionGuardarProyecto->setText("Guardar misión");
    accionGuardarProyecto->setIcon(QIcon(":iconos/images/save.png"));
    connect(accionGuardarProyecto, SIGNAL(triggered()), this, SLOT(guardarProyecto()));
    ui->mainToolBar->addAction(accionGuardarProyecto);

    accionGuardarComoProyecto = new QAction();
    accionGuardarComoProyecto->setCheckable(false);
    accionGuardarComoProyecto->setStatusTip(tr("Guardar misión como...", "Action"));
    accionGuardarComoProyecto->setText("Guardar misión como...");
    accionGuardarComoProyecto->setIcon(QIcon(":iconos/images/saveas.png"));
    connect(accionGuardarComoProyecto, SIGNAL(triggered()), this, SLOT(guardarProyecto()));
    connect(ui->actionGuardar_mision_como, SIGNAL(triggered()), this, SLOT(guardarProyecto()));
    ui->mainToolBar->addAction(accionGuardarComoProyecto);

    accionExportarFly = new QAction();
    accionExportarFly->setCheckable(false);
    accionExportarFly->setStatusTip(tr("Exportar proyecto a fichero fly", "Action"));
    accionExportarFly->setText("Exportar a fly");
    accionExportarFly->setIcon(QIcon(":iconos/images/export.png"));
    connect(accionExportarFly, SIGNAL(triggered()), this, SLOT(exportarFly()));
    ui->mainToolBar->addAction(accionExportarFly);

    accionImportarFly = new QAction();
    accionImportarFly->setCheckable(false);
    accionImportarFly->setStatusTip(tr("Importar fichero fly", "Action"));
    accionImportarFly->setText("Importar fichero FLY");
    accionImportarFly->setIcon(QIcon(":iconos/images/import.png"));
    connect(accionImportarFly, SIGNAL(triggered()), this, SLOT(importarFly()));
    connect(ui->actionImportar_fly_a_misi_n, SIGNAL(triggered(bool)), this, SLOT(importarFly()));
    ui->mainToolBar->addAction(accionImportarFly);

    ui->mainToolBar->addSeparator();

    accionDeshacerMenu = new QAction();
    accionDeshacerMenu->setCheckable(false);
    accionDeshacerMenu->setStatusTip(tr("Deshacer accion", "Action"));
    accionDeshacerMenu->setText("Deshacer accion");
    accionDeshacerMenu->setIcon(QIcon(":iconos/images/undo.png"));
    connect(accionDeshacerMenu, SIGNAL(triggered()), this, SLOT(deshacer()));
    ui->mainToolBar->addAction(accionDeshacerMenu);

    //boton lineas divisorias
    accionGestionMapas = new QAction();
    accionGestionMapas->setCheckable(false);
    accionGestionMapas->setStatusTip(tr("Herramienta para crear WayPoints", "Action"));
    accionGestionMapas->setText("WayPoints");
    accionGestionMapas->setIcon(QIcon(":iconos/images/maps.png"));
    connect(accionGestionMapas, SIGNAL(triggered()), this, SLOT(cargarImagenMapa()));
    ui->mainToolBar->addAction(accionGestionMapas);

    accionCrearWayPoints = new QAction();
    accionCrearWayPoints->setCheckable(true);
    accionCrearWayPoints->setStatusTip(tr("Herramienta para crear WayPoints", "Action"));
    accionCrearWayPoints->setText("WayPoints");
    accionCrearWayPoints->setIcon(QIcon(":iconos/images/waypoints.png"));
    connect(accionCrearWayPoints, SIGNAL(triggered()), this, SLOT(wpHotKeyPulsada()));
    ui->mainToolBar->addAction(accionCrearWayPoints);

    //boton lineas divisorias
    accionCrearPoligonos = new QAction();
    accionCrearPoligonos->setCheckable(true);
    accionCrearPoligonos->setStatusTip(tr("Herramienta para crear grids", "Action"));
    accionCrearPoligonos->setText("Grid");
    accionCrearPoligonos->setIcon(QIcon(":iconos/images/grid.png"));
    connect(accionCrearPoligonos, SIGNAL(triggered()), this, SLOT(gridHotKeyPulsada()));
    ui->mainToolBar->addAction(accionCrearPoligonos);

    accionBorrarWPSeleccionado = new QAction();
    accionBorrarWPSeleccionado->setCheckable(false);
    accionBorrarWPSeleccionado->setStatusTip(tr("Eliminar waypoint seleccionado", "Action"));
    accionBorrarWPSeleccionado->setText("Eliminar waypoint seleccionado");
    accionBorrarWPSeleccionado->setIcon(QIcon(":iconos/images/rubber.png"));
    connect(accionBorrarWPSeleccionado, SIGNAL(triggered()), this, SLOT(borrarWPSeleccionado()));
    connect(ui->actionBorrar_waypoint_seleccionado, SIGNAL(triggered(bool)), this, SLOT(borrarWPSeleccionado()));
    ui->mainToolBar->addAction(accionBorrarWPSeleccionado);

    accionLimpiarMision = new QAction();
    accionLimpiarMision->setCheckable(false);
    accionLimpiarMision->setStatusTip(tr("Eliminar elementos de la misión", "Action"));
    accionLimpiarMision->setText("Eliminar elementos de la misión");
    accionLimpiarMision->setIcon(QIcon(":iconos/images/clean.png"));
    connect(accionLimpiarMision, SIGNAL(triggered()), this, SLOT(limpiarMision()));
    connect(ui->actionBorrar_todos_los_waypoints, SIGNAL(triggered(bool)), this, SLOT(limpiarMision()));
    ui->mainToolBar->addAction(accionLimpiarMision);

    accionReglaAngulo = new QAction();
    accionReglaAngulo->setCheckable(true);
    accionReglaAngulo->setText(tr("Regla y ángulo", "Action"));
    accionReglaAngulo->setStatusTip("Herramienta para medir distancias y ángulos");
    accionReglaAngulo->setIcon(QIcon(":iconos/images/ruler.png"));
    connect(accionReglaAngulo, SIGNAL(triggered()), this, SLOT(habilitarMedirDistanciasYAngulo()));
    ui->mainToolBar->addAction(accionReglaAngulo);

    connect(ui->actionAcerca_de, SIGNAL(triggered(bool)), this, SLOT(abrirVentanaAcercaDe()));

    accionSeleccionar = new QAction();
    accionSeleccionar->setCheckable(true);
    accionSeleccionar->setText(tr("Seleccionar", "Action"));
    accionSeleccionar->setStatusTip("Herramienta para seleccionar waypoints");
    accionSeleccionar->setIcon(QIcon(":iconos/images/select.png"));
    connect(accionSeleccionar, SIGNAL(triggered()), this, SLOT(habilitarSeleccionar()));
    ui->mainToolBar->addAction(accionSeleccionar);

    ui->mainToolBar->addSeparator();

    accionCamaras = new QAction();
    accionCamaras->setCheckable(false);
    accionCamaras->setStatusTip(tr("Gestionar cámaras", "Action"));
    accionCamaras->setText("Gestionar cámaras");
    accionCamaras->setIcon(QIcon(":iconos/images/camera.png"));
    connect(accionCamaras, SIGNAL(triggered()), this, SLOT(abrirGestorCamaras()));
    connect(ui->actionC_maras, SIGNAL(triggered()), this, SLOT(abrirGestorCamaras()));
    ui->mainToolBar->addAction(accionCamaras);

    accionConfiguracionAvion = new QAction();
    accionConfiguracionAvion->setCheckable(false);
    accionConfiguracionAvion->setStatusTip(tr("Gestionar aviones", "Action"));
    accionConfiguracionAvion->setText("Gestionar aviones");
    accionConfiguracionAvion->setIcon(QIcon(":iconos/images/plane.png"));
    connect(accionConfiguracionAvion, SIGNAL(triggered()), this, SLOT(abrirGestorAviones()));
    connect(ui->actionAviones, SIGNAL(triggered(bool)), this, SLOT(abrirGestorAviones()));
    ui->mainToolBar->addAction(accionConfiguracionAvion);

    accionOpcionesVisorWaypoints = new QAction();
    accionOpcionesVisorWaypoints->setCheckable(false);
    accionOpcionesVisorWaypoints->setStatusTip(tr("Opciones visor waypoints", "Action"));
    accionOpcionesVisorWaypoints->setText("Mostrar opciones del visor principal");
    accionOpcionesVisorWaypoints->setIcon(QIcon(":iconos/images/configviewer.png"));
    connect(accionOpcionesVisorWaypoints, SIGNAL(triggered()), this, SLOT(mostrarVentanaOpcionesMenuWaypoints()));
    connect(ui->actionOpciones_visor, SIGNAL(triggered(bool)), this, SLOT(mostrarVentanaOpcionesMenuWaypoints()));
    ui->mainToolBar->addAction(accionOpcionesVisorWaypoints);

    ui->mainToolBar->addSeparator();

    accionLeyenda = new QAction();
    accionLeyenda->setCheckable(true);
    accionLeyenda->setChecked(true);
    accionLeyenda->setText(tr("Leyenda", "Action"));
    accionLeyenda->setStatusTip("Muestra la leyenda en el visor");
    accionLeyenda->setIcon(QIcon(":iconos/images/leyend.png"));
    connect(accionLeyenda, SIGNAL(triggered()), this, SLOT(mostrarLeyenda()));
    ui->mainToolBar->addAction(accionLeyenda);

    accionMostrarVisorAlturas = new QAction();
    accionMostrarVisorAlturas->setCheckable(true);
    accionMostrarVisorAlturas->setChecked(true);
    accionMostrarVisorAlturas->setText(tr("Mostrar visor alturas", "Action"));
    accionMostrarVisorAlturas->setStatusTip("Muestra u oculta el visor de alturas");
    accionMostrarVisorAlturas->setIcon(QIcon(":iconos/images/mostrarVisorAlturas.png"));
    connect(accionMostrarVisorAlturas, SIGNAL(triggered()), this, SLOT(mostrarVisorAlturas()));
    ui->mainToolBar->addAction(accionMostrarVisorAlturas);

    accionMostrarDatosMision = new QAction();
    accionMostrarDatosMision->setCheckable(true);
    accionMostrarDatosMision->setChecked(true);
    accionMostrarDatosMision->setText(tr("Mostrar datos misión", "Action"));
    accionMostrarDatosMision->setStatusTip("Muestra u oculta los datos de la misión");
    accionMostrarDatosMision->setIcon(QIcon(":iconos/images/mostrarTab.png"));
    connect(accionMostrarDatosMision, SIGNAL(triggered()), this, SLOT(mostrarDatosMision()));
    ui->mainToolBar->addAction(accionMostrarDatosMision);

    connect(ui->actionOpciones, SIGNAL(triggered(bool)), this, SLOT(abrirOpciones()));
}

#endif


void MainWindow::mostrarDatosMision()
{
    visor->setFocus();
    if (accionMostrarDatosMision->isChecked()) {
        ui->tabWidget->show();
    } else {
        ui->tabWidget->hide();
    }
}

void MainWindow::mostrarVentanaOpcionesMenuWaypoints()
{
    ConfiguracionVisorWayPointsDialog dialogo(visor->verDistanciasEntrePuntos, visor->verLineasDireccion, visor->distanciaTotalRecorrido, datosProyecto.tipoCoordenada);
    dialogo.setModal(true);
    dialogo.exec();

    if (dialogo.aceptado) {
        visor->verDistanciasEntrePuntos = dialogo.verDistanciasEntrePuntos;
        visor->verLineasDireccion = dialogo.verLineasDireccion;
        visor->distanciaTotalRecorrido = dialogo.distanciaTotalRecorrido;
        datosProyecto.tipoCoordenada = dialogo.tipoCoordenada;
    }
}

void MainWindow::mostrarVisorAlturas()
{    

    if (accionMostrarVisorAlturas->isChecked()) {
        visorAlturasDock->show();
        actualizarWayPointsAlturas();
    } else {
        visorAlturasDock->hide();
    }
}

void MainWindow::mostrarLeyenda()
{
    leyendaActivada = !leyendaActivada;
    accionLeyenda->setChecked(leyendaActivada);
    visor->mostrarLeyenda = leyendaActivada;
}

void MainWindow::abrirOpciones()
{
    ConfiguracionDialog ventana(idioma, rutaHorizon, datosProyecto.aterrizajeCirculo);
    ventana.setModal(true);
    ventana.exec();

    if (ventana.aceptado) {
        idioma = ventana.idiomaSeleccionado;
        Idioma::getInstancia()->setIdioma(idioma);
        rutaHorizon = ventana.rutaHorizon;
        datosProyecto.aterrizajeCirculo = ventana.aterrizajeCirculo;
        GestorConfiguracion::getInstancia()->rutaHorizon = rutaHorizon;
        GestorConfiguracion::getInstancia()->idioma = idioma;
        GestorConfiguracion::getInstancia()->guardar();
        traducir();
    }
}

void MainWindow::crearValidadores()
{
    ui->maxAlturaVisorEdit->setValidator(new QIntValidator(100, 1000, this));
    ui->maxAlturaVueloEdit->setValidator(new QIntValidator(1, 1000, this));
    ui->alturaWaypointsText->setValidator(new QIntValidator(1, 1000, this));
}

void MainWindow::borrarWPSeleccionado()
{
    visor->eliminarWPSeleccionados();
    gestorGrids->eliminarGridMarcados();
    visor->eliminarMarcadoresSeleccionados();
    quitarDockInformacion();
}

void MainWindow::abrirGestorCamaras()
{
   CamarasDialog* ventana = new CamarasDialog(gestorCamaras->camaras, this);
   ventana->setModal(true);
   ventana->exec();
   if (ventana->aceptado) {
       gestorCamaras->camaras = ventana->camaras;
   }
   delete ventana;
}

void MainWindow::activarBotonesEdicion(bool valor)
{
    accionCrearPoligonos->setEnabled(valor);
    accionCrearWayPoints->setEnabled(valor);
}

void MainWindow::abrirGestorAviones()
{
    GestionarAvionesDialog* ventana = new GestionarAvionesDialog(rutaHorizon, gestorAviones->listaAviones, this);
    ventana->setModal(true);
    ventana->exec();
    if (ventana->aceptado) {
        gestorAviones->listaAviones = ventana->listaAviones;        
        recargarDatosAvion(ui->comboAviones->currentIndex());
    }
    delete ventana;
}

void MainWindow::abrirVentanaAcercaDe()
{
    AcercaDeDialog ventana(version, fechaVersion, this);
    ventana.setModal(true);
    ventana.exec();
}

void MainWindow::aplicarCambiosConfiguracion()
{
    datosProyecto.alturaMaximaPermitida = Formateador::formatearEntero(ui->maxAlturaVueloEdit->text());
    datosProyecto.alturaMaximaVisor = Formateador::formatearEntero(ui->maxAlturaVisorEdit->text());
    ui->maxAlturaVisorEdit->setText(Formateador::formatearEntero(datosProyecto.alturaMaximaPermitida));
    ui->maxAlturaVisorEdit->setText(Formateador::formatearEntero(datosProyecto.alturaMaximaVisor));

    actualizarWayPointsAlturas();
}

void MainWindow::finalizarEdicion()
{
    if (crearGrid) {
        gridHotKeyPulsada();
    }
    crearWP = false;
    visor->activarCreacionWP(false);
    accionCrearWayPoints->setChecked(false);
    medirDistancias = false;
    accionReglaAngulo->setChecked(false);
    visor->activarMedirDistanciaYAngulo(false);
    if (seleccionarWaypoints) {
        habilitarSeleccionar();
    }
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void MainWindow::teclaPulsada(QKeyEvent *ev)
{
    if (ev->key() == Qt::Key_Escape) {
        finalizarEdicion();
    } else {
        visor->keyPressEvent(ev);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    teclaPulsada(ev);
}

void MainWindow::crearWayPointsPorGrid(Grid* grid)
{
    bool esFlyTo = false;
    int contador = 0;

    gestorWayPoints->modoCrear = true;    
    QList<QPointF> puntos = grid->puntosRecorridoCalculado;
    for (int j = 0; j < puntos.size(); j++) {

        QPointF punto = HerramientaConversorCoordenadas::getInstancia()->getPixel(Enumerados::LatLng, puntos[j].x(), puntos[j].y());

        punto.setX(HerramientasVisor::getInstancia()->getDesplazamientoX() + punto.x());
        punto.setY(HerramientasVisor::getInstancia()->getDesplazamientoY() + punto.y());
        gestorWayPoints->crearWayPoint(punto, grid->altura);
        WayPoint* wp = gestorWayPoints->getUltimoWayPoint();
        if (j == 0 || j == 1) {
            wp->modo = Enumerados::FromTo;
            if (j == 1) {
                esFlyTo = true;
            }
            addWPLista(wp);
            continue;
        }

        if (esFlyTo && contador < 2) {
            contador++;
            wp->modo = Enumerados::FlyTo;
            addWPLista(wp);
            if (contador == 2) {
                esFlyTo = false;
                contador = 0;
            }
            continue;
        }

        if (!esFlyTo && contador < 2) {
            wp->modo = Enumerados::FromTo;
            addWPLista(wp);
            esFlyTo = true;
            continue;
        }

    }
    gestorWayPoints->modoCrear = false;
    gestorGrids->eliminarGridMarcados();
}

void MainWindow::quitarWpDock()
{
    if (wpDock != Q_NULLPTR) {
        wpDock->hide();
        delete wpDock;
        wpDock = Q_NULLPTR;
    }
}

void MainWindow::quitarGridDock()
{
    if (gridDock != Q_NULLPTR) {
        gridDock->hide();
        delete gridDock;
        gridDock = Q_NULLPTR;
    }
}

void MainWindow::quitarDockInformacion()
{
    quitarWpDock();
    quitarGridDock();
}

void MainWindow::cargarInformacionGrid(Grid* grid)
{
    quitarDockInformacion();
    gridDock = new GridDockWidget(grid, gestorCamaras->camaras, this);
    connect(gridDock, SIGNAL(crearWayPointsPorGrid(Grid*)), this, SLOT(crearWayPointsPorGrid(Grid*)));
    addDockWidget(Qt::RightDockWidgetArea, gridDock);
    gridDock->show();
}

void MainWindow::cargarInformacionWayPoint(WayPoint* wp)
{
    quitarDockInformacion();
    wpDock = new WayPointDockWidget(datosProyecto.tipoCoordenada, wp, gestorWayPoints->wayPoints.size(), datosProyecto.numeroZona + datosProyecto.letraZona, this);
    connect(wpDock, SIGNAL(landingEstablecido()), this, SLOT(establecerLanding()));    
    connect(wpDock, SIGNAL(datosActualizados()), this, SLOT(updateWPListaInformacion()));
    connect(wpDock, SIGNAL(modificarAterrizaje(bool,int)), this, SLOT(modificarAterrizaje(bool, int)));
#ifndef MODO_PORTATIL
    addDockWidget(Qt::RightDockWidgetArea, wpDock);
#endif
    wpDock->show();
}

void MainWindow::modificarAterrizaje(bool derecha, int pitch)
{
    visor->modificaAterrizaje(derecha, pitch);
    actualizarTiempoDeVuelo();
}

void MainWindow::deshacer()
{
    if (GestorHistoricoOperaciones::getInstancia()->hayHistorico()) {
        OperacionRealizada* operacion = GestorHistoricoOperaciones::getInstancia()->desapilar();
        switch (operacion->tipoObjeto) {
        case Enumerados::ObjetoGrid:
            quitarGridDock();
            gestorGrids->realizarOperacionHistorica(operacion);
            break;
        case Enumerados::ObjetoWayPoint:
            quitarWpDock();
            gestorWayPoints->realizarOperacionHistorica(operacion);
            break;
        }
        actualizarWayPointsAlturas();
        if (datosProyecto.imagenesMapa.size() > 0) {
            activarBotones(true);
        } else {
            activarBotones(false);
        }
    }
}

void MainWindow::crearHotKeys()
{
    crearWPHotKey = new QAction(this);
    crearWPHotKey->setShortcut(QKeySequence(tr("Ctrl+w")));
    connect(crearWPHotKey, SIGNAL(triggered()), this, SLOT(wpHotKeyPulsada()));
    this->addAction(crearWPHotKey);
    connect(ui->actionA_adir_waypoints, SIGNAL(triggered()), this, SLOT(wpHotKeyPulsada()));


    accionDeshacer = new QAction(this);
    accionDeshacer->setShortcut(QKeySequence(tr("Ctrl+z")));
    connect(accionDeshacer, SIGNAL(triggered()), this, SLOT(deshacer()));
    this->addAction(accionDeshacer);

    crearGridHotKey = new QAction(this);
    crearGridHotKey->setShortcut(QKeySequence(tr("Ctrl+p")));
    connect(crearGridHotKey, SIGNAL(triggered()), this, SLOT(gridHotKeyPulsada()));
    this->addAction(crearGridHotKey);
    connect(ui->actionCrear_grids_Ctrl_p, SIGNAL(triggered()), this, SLOT(gridHotKeyPulsada()));

    crearWPAnteriorMarcadoHotKey = new QAction(this);
    crearWPAnteriorMarcadoHotKey->setShortcut(QKeySequence(tr("Ctrl+b")));
    connect(crearWPAnteriorMarcadoHotKey, SIGNAL(triggered()), this, SLOT(crearWPAnteriorMarcadoHotKeyPulsada()));
    this->addAction(crearWPAnteriorMarcadoHotKey);
    connect(ui->actionCrear_waypoint_anterior_a_marcado, SIGNAL(triggered()), this, SLOT(crearWPAnteriorMarcadoHotKeyPulsada()));

    guardarProyectoHotKey = new QAction(this);
    guardarProyectoHotKey->setShortcut(QKeySequence(tr("Ctrl+s")));
    connect(guardarProyectoHotKey, SIGNAL(triggered()), this, SLOT(guardarProyecto()));
    this->addAction(guardarProyectoHotKey);
    connect(ui->actionGuardar_proyecto, SIGNAL(triggered()), this, SLOT(guardarProyecto()));

    abrirProyectoHotKey = new QAction(this);
    abrirProyectoHotKey->setShortcut(QKeySequence(tr("Ctrl+o")));
    connect(abrirProyectoHotKey, SIGNAL(triggered()), this, SLOT(abrirProyecto()));
    this->addAction(abrirProyectoHotKey);
    connect(ui->actionAbrir_proyecto, SIGNAL(triggered()), this, SLOT(abrirProyecto()));

    exportarProyectoHotKey = new QAction(this);
    exportarProyectoHotKey->setShortcut(QKeySequence(tr("Ctrl+e")));
    connect(exportarProyectoHotKey, SIGNAL(triggered()), this, SLOT(exportarFly()));
    this->addAction(exportarProyectoHotKey);
    connect(ui->actionExportar_a_fichero_FLY, SIGNAL(triggered()), this, SLOT(exportarFly()));

    nuevaMisionHotKey = new QAction(this);
    nuevaMisionHotKey->setShortcut(QKeySequence(tr("Ctrl+n")));
    connect(nuevaMisionHotKey, SIGNAL(triggered()), this, SLOT(nuevaMision()));
    connect(ui->actionNueva_mision, SIGNAL(triggered(bool)), this, SLOT(nuevaMision()));
    this->addAction(nuevaMisionHotKey);

    salirAplicacionHotKey = new QAction(this);
    salirAplicacionHotKey->setShortcut(QKeySequence(tr("Ctrl+q")));
    connect(salirAplicacionHotKey, SIGNAL(triggered()), this, SLOT(salir()));
    connect(ui->actionSalir, SIGNAL(triggered(bool)), this, SLOT(salir()));
    this->addAction(salirAplicacionHotKey);

    leyendaHotKey = new QAction(this);
    leyendaHotKey->setShortcut(QKeySequence(tr("Ctrl+l")));
    connect(leyendaHotKey, SIGNAL(triggered()), this, SLOT(mostrarLeyenda()));
    this->addAction(leyendaHotKey);

}

void MainWindow::limpiarMision()
{
    if (gestorWayPoints->wayPoints.size() == 0) {
        return;
    }
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText(Idioma::getInstancia()->getTexto("ventana.limpiarmision.mensaje"));
    msgBox.setInformativeText(Idioma::getInstancia()->getTexto("ventana.limpiarmision.submensaje"));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int opcion = msgBox.exec();
    if (opcion == QMessageBox::Ok) {
        gestorWayPoints->marcarTodosWayPoint(true);
        gestorWayPoints->eliminarWayPointMarcados();
        WayPoint* circuit = gestorWayPoints->getCircuit();
        if (circuit != Q_NULLPTR && circuit->aterrizaje != Q_NULLPTR) {
            delete circuit->aterrizaje;
            circuit->aterrizaje = Q_NULLPTR;
        }
        visorAlturasWidget->listaWP.clear();
        visorAlturasWidget->listaWPAlturas.clear();
    }

}

void MainWindow::reiniciarMision()
{
    datosProyecto = DatosProyecto();
    mapas.clear();
    delete gestorWayPoints;
    gestorWayPoints = new GestorWayPoints(&datosProyecto);
    delete gestorGrids;
    gestorGrids = new GestorGrids();

    ui->horizontalLayout->removeWidget(visor);
    delete visor;
    visor = new VisorWidget(&datosProyecto, gestorWayPoints, gestorGrids, this);
    visor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->horizontalLayout->addWidget(visor);

    removeDockWidget(visorAlturasDock);
    delete visorAlturasWidget;
    delete visorAlturasDock;

    visorAlturasWidget = new VisorAlturasWidget(&datosProyecto, gestorWayPoints, this);
    visorAlturasDock = new VisorAlturasDockWidget(visorAlturasWidget, this);
    addDockWidget(Qt::BottomDockWidgetArea, visorAlturasDock);
    accionMostrarVisorAlturas->setChecked(true);
    accionMostrarDatosMision->setChecked(true);
    ui->tabWidget->show();
    visorAlturasDock->show();
    //mostrarVisorAlturas();


    establecerDatosProyecto(false);
    conectarVisor();

    ui->alturaWaypointsText->setText(Formateador::formatearEntero(datosProyecto.alturaWaypoints));
    ui->comboAviones->setCurrentIndex(0);
    recargarDatosAvion(0);

    HerramientaConversorCoordenadas::getInstancia()->establecerDatos(Enumerados::LatLng, 0,0,0,0,0,0);
    HerramientasVisor::getInstancia()->factorEscalado = 1;
    HerramientasVisor::getInstancia()->dx = 0;
    HerramientasVisor::getInstancia()->dy = 0;

    //accionLeyenda->setChecked(false);
    activarBotones(false);

    quitarWpDock();
    quitarGridDock();

#ifndef MODO_PORTATIL
    visorAlturasDock->show();
    ui->tabWidget->show();
#else
    visorAlturasDock->hide();
    ui->tabWidget->hide();
    accionMostrarDatosMision->setChecked(false);
    accionMostrarVisorAlturas->setChecked(false);
#endif
}

void MainWindow::nuevaMision()
{
    if (gestorWayPoints->wayPoints.size() > 0 || mapas.size() > 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(Idioma::getInstancia()->getTexto("ventana.nuevamision.titulo"));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText(Idioma::getInstancia()->getTexto("ventana.nuevamision.message"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int opcion = msgBox.exec();
        if (opcion == QMessageBox::Ok) {
            guardarProyecto();
        } else if (opcion == QMessageBox::Cancel){
            return;
        }
    }
    reiniciarMision();

    ParametrosObligatoriosDialog dialogo;
    dialogo.setModal(true);
    dialogo.exec();

    ui->nombreProyectoEdit->setText(dialogo.nombre);
    ui->alturaWaypointsText->setText(QString::number(dialogo.alturaPredefinida));
    ui->maxAlturaVisorEdit->setText(QString::number(dialogo.alturaVisor));
    ui->maxAlturaVueloEdit->setText(QString::number(dialogo.alturaMaximaVuelo));
    datosProyecto.aterrizajeCirculo = dialogo.aterrizajeEnCirculo;
}

void MainWindow::salir()
{    
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(Idioma::getInstancia()->getTexto("ventana.salir.titulo"));
    msgBox.setText(Idioma::getInstancia()->getTexto("ventana.salir.mensaje"));
    msgBox.setInformativeText(Idioma::getInstancia()->getTexto("ventana.salir.informacion"));
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    int opcion = msgBox.exec();
    if (opcion == QMessageBox::Yes) {
        exit(0);
    }
}

bool MainWindow::establecerDatosProyecto(bool recoger)
{    
    if (recoger) {
        datosProyecto.nombre = ui->nombreProyectoEdit->text();
        datosProyecto.localizacion = ui->localizacionEdit->text();
        datosProyecto.descripcion = ui->descripcionEdit->toPlainText();
        datosProyecto.waypointsLista = gestorWayPoints->wayPoints;
        datosProyecto.alturaWaypoints = Formateador::formatearEntero(ui->alturaWaypointsText->text());
    } else {
        ui->nombreProyectoEdit->setText(datosProyecto.nombre);
        ui->localizacionEdit->setText(datosProyecto.localizacion);
        ui->descripcionEdit->setPlainText(datosProyecto.descripcion);
        ui->alturaWaypointsText->setText(Formateador::formatearEntero(datosProyecto.alturaWaypoints));
        gestorWayPoints->wayPoints = datosProyecto.waypointsLista;
        visor->establecerAterrizajeDesdeFly();
        for (int i = 0; i < datosProyecto.imagenesMapa.size(); i++) {
            if (i == 0) {
                HerramientaConversorCoordenadas::getInstancia()->establecerDatos(Enumerados::LatLng, datosProyecto.imagenesMapa[i]->latitud1,
                                                                                                datosProyecto.imagenesMapa[i]->longitud1,
                                                                                                datosProyecto.imagenesMapa[i]->latitud2,
                                                                                                datosProyecto.imagenesMapa[i]->longitud2,
                                                                                                datosProyecto.imagenesMapa[i]->widthOriginal,
                                                                                                datosProyecto.imagenesMapa[i]->heigthOriginal);

//                HerramientaConversorCoordenadas::getInstancia()->establecerDatosLatitudLongitud(datosProyecto.imagenesMapa[i]->latitud1,
//                                                                                                datosProyecto.imagenesMapa[i]->longitud1,
//                                                                                                datosProyecto.imagenesMapa[i]->latitud2,
//                                                                                                datosProyecto.imagenesMapa[i]->longitud2,
//                                                                                                datosProyecto.imagenesMapa[i]->widthOriginal,
//                                                                                                datosProyecto.imagenesMapa[i]->heigthOriginal
//                            );
                QImage imagenEscalada = datosProyecto.imagenesMapa[0]->imagen->scaled(datosProyecto.imagenesMapa[0]->imagen->width() * HerramientasVisor::getInstancia()->factorEscalado,
                               datosProyecto.imagenesMapa[0]->imagen->height() * HerramientasVisor::getInstancia()->factorEscalado,
                               Qt::KeepAspectRatio);
                HerramientaConversorCoordenadas::getInstancia()->setWidth(imagenEscalada.width());
                HerramientaConversorCoordenadas::getInstancia()->setHeight(imagenEscalada.height());
                HerramientaConversorCoordenadas::getInstancia()->imagenReescalada = imagenEscalada;

            }
            datosProyecto.imagenesMapa[i]->desplazar(HerramientasVisor::getInstancia()->getDesplazamientoX(), HerramientasVisor::getInstancia()->getDesplazamientoY());
        }
        ui->comboAviones->setCurrentIndex(datosProyecto.numeroAvion);
        recargarDatosAvion(datosProyecto.numeroAvion);
    }
    return true;
}

Avion* MainWindow::getAvionSeleccionado()
{
    int indice = ui->comboAviones->currentIndex();
    if (indice < 0 || gestorAviones->listaAviones.size() <= 0) {
        return Q_NULLPTR;
    }
    return gestorAviones->listaAviones[indice];
}

void MainWindow::importarFly()
{
    ImportarFlyDialog dialogo(rutaHorizon, this);
    dialogo.setModal(true);
    dialogo.exec();

    if (dialogo.aceptado) {
        reiniciarMision();
        GestorFly::altura = Formateador::formatearEntero(ui->alturaWaypointsText->text());
        int resultado = GestorFly::leer(dialogo.rutaFly, gestorWayPoints->wayPoints);
        if (resultado == -1) {
            QMessageBox::critical(this, Idioma::getInstancia()->getTexto("ventana.importar.error.titulo"), Idioma::getInstancia()->getTexto("ventana.importar.error.mensaje"));
            return;
        }
        if (resultado == -2) {
            QMessageBox::critical(this, Idioma::getInstancia()->getTexto("ventana.importar.error.titulo"), Idioma::getInstancia()->getTexto("ventana.importar.error.imperial.mensaje"));
            return;
        }
        visor->establecerAterrizajeDesdeFly();
        mapas.clear();
        mapas.push_back(dialogo.imagenMapa);
        cargarMapaPrevio(dialogo.imagenMapa);
        GestorUltimasMapas::getInstancia()->add(mapas[0]);
        actualizarMapasMenu();
        actualizarWayPointsAlturas();
    }
}

void MainWindow::exportarFly()
{
    WayPoint* circuit = gestorWayPoints->getCircuit();
    if (circuit == Q_NULLPTR) {
        QMessageBox::critical(this, Idioma::getInstancia()->getTexto("ventana.exportar.circuit.error.titulo"), Idioma::getInstancia()->getTexto("ventana.exportar.circuit.error.mensaje"));
        return;
    }
    QString nombreFichero = QFileDialog::getSaveFileName(this, Idioma::getInstancia()->getTexto("ventana.exportar.titulo"), rutaHorizon + "\\fly", tr("Fly files (*.fly)"));
    if (nombreFichero == "") {
        return;
    }
    Avion* avion = getAvionSeleccionado();
    if (avion == Q_NULLPTR) {
        return;
    }
    if (!GestorFly::exportar(nombreFichero, datosProyecto.tipoCoordenada, gestorWayPoints->wayPoints, avion)) {
        QMessageBox::critical(this, Idioma::getInstancia()->getTexto("ventana.exportar.error.titulo"), Idioma::getInstancia()->getTexto("ventana.exportar.error.mensaje"));
        return;
    }
    QMessageBox::information(this, Idioma::getInstancia()->getTexto("ventana.exportar.finexportado.titulo"), Idioma::getInstancia()->getTexto("ventana.exportar.finexportado.mensaje"));
}

bool MainWindow::existeMapa(QString rutaMapa)
{
    for (int i = 0; i < mapas.size(); i++) {
        if (rutaMapa == mapas[i]->pathFichero) {
            return true;
        }
    }
    return false;
}

void MainWindow::cargarImagenMapa()
{
    ConfiguracionImagenesMapaDialog* dialogo = new ConfiguracionImagenesMapaDialog(rutaHorizon, datosProyecto.imagenesMapa, datosProyecto.numeroZona + datosProyecto.letraZona, this, datosProyecto.tipoCoordenada);
    dialogo->exec();

    if (dialogo->aceptado) {
        QList<ImagenMapa*> mapasAIncluir;
        for (int i = 0; i < dialogo->listaMapas.size(); i++) {
            if (!existeMapa(dialogo->listaMapas[i]->pathFichero)) {
                //Log::getInstancia()->info("Se añade el mapa " + dialogo->listaMapas[i]->pathFichero);
                mapasAIncluir.push_back(dialogo->listaMapas[i]);
            }
        }
        mapas.clear();
        mapas = dialogo->listaMapas;        
        datosProyecto.imagenesMapa = mapas;
        //datosProyecto.tipoCoordenada = Enumerados::LatLng;

        if (mapas.size() > 0) {

            HerramientaConversorCoordenadas::getInstancia()->establecerDatos(Enumerados::LatLng, mapas[0]->latitud1,
                                                                                                            mapas[0]->longitud1,
                                                                                                            mapas[0]->latitud2,
                                                                                                            mapas[0]->longitud2,
                                                                                                            mapas[0]->widthOriginal,
                                                                                                            mapas[0]->heigthOriginal);

            HerramientasVisor::getInstancia()->dx = 0;
            HerramientasVisor::getInstancia()->dy = 0;
            HerramientasVisor::getInstancia()->dzx = 0;
            HerramientasVisor::getInstancia()->dzy = 0;
            HerramientasVisor::getInstancia()->factorEscalado = 1;

            HerramientaConversorCoordenadas::getInstancia()->imagenReescalada = *mapas[0]->imagen;
            int dx = abs((mapas[0]->imagen->width() - visor->width()) / 2);
            int dy = abs((mapas[0]->imagen->height() - visor->height()) / 2);
            HerramientasVisor::getInstancia()->acumularDesplazamiento(-dx, -dy);

            visor->desplazarImagenesMapa(-dx, -dy);
            GestorUltimasMapas::getInstancia()->add(mapas[0]);
            actualizarMapasMenu();
        }
    }

    if (datosProyecto.imagenesMapa.size() > 0) {
        activarBotones(true);
    } else {
        activarBotones(false);
    }
}

void MainWindow::actualizarMapasMenu()
{
    ui->menuMapas->clear();
    QSignalMapper* signalMapper = new QSignalMapper(this);
    QSignalMapper* signalMapperClean = new QSignalMapper(this);
    accionesMapas.clear();

    QAction* accionAdd = new QAction(Idioma::getInstancia()->getTexto("menu.maps.add"));
    accionAdd->setIcon(QIcon(":iconos/images/maps.png"));
    connect(accionAdd, SIGNAL(triggered(bool)), this, SLOT(cargarImagenMapa()));
    accionesMapas.push_back(accionAdd);


    for (int i = 0; i < GestorUltimasMapas::getInstancia()->mapasPrevios.size(); i++) {
        QString texto = GestorUltimasMapas::getInstancia()->mapasPrevios[i]->pathFichero + ", " + QString::number(GestorUltimasMapas::getInstancia()->mapasPrevios[i]->latitud1, 'f', 6) + ", " + QString::number(GestorUltimasMapas::getInstancia()->mapasPrevios[i]->longitud1, 'f', 6) + ", " + QString::number(GestorUltimasMapas::getInstancia()->mapasPrevios[i]->latitud2, 'f', 6) + ", " + QString::number(GestorUltimasMapas::getInstancia()->mapasPrevios[i]->longitud2, 'f', 6);
        QAction* accion = new QAction(texto);
        signalMapper->setMapping(accion, GestorUltimasMapas::getInstancia()->mapasPrevios[i]);
        connect(accion, SIGNAL(triggered(bool)), signalMapper, SLOT(map()));
        accionesMapas.push_back(accion);
    }
    connect(signalMapper, SIGNAL(mapped(QObject*)), this, SLOT(cargarMapaPrevio(QObject*)));

    if (accionesMapas.size() > 1) {
        QAction* accion = new QAction(Idioma::getInstancia()->getTexto("menu.maps.clean"));
        signalMapperClean->setMapping(accion, 0);
        connect(accion, SIGNAL(triggered(bool)), signalMapperClean, SLOT(map()));
        accionesMapas.push_back(accion);
    }
    connect(signalMapperClean, SIGNAL(mapped(int)), this, SLOT(cargarMapaPrevio(int)));


    ui->menuMapas->addActions(accionesMapas);


}

void MainWindow::cargarMapaPrevio(int id)
{          
    if (id == 0) {

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(Idioma::getInstancia()->getTexto("ventana.mapas.borrar.titulo"));
        msgBox.setText(Idioma::getInstancia()->getTexto("ventana.mapas.borrar.mensaje"));
        msgBox.setInformativeText(Idioma::getInstancia()->getTexto("ventana.mapas.borrar.informacion"));
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        int opcion = msgBox.exec();
        if (opcion == QMessageBox::No) {
            return;
        }

        ui->menuMapas->clear();
        GestorUltimasMapas::getInstancia()->limpiar();


        QAction* accionAdd = new QAction(Idioma::getInstancia()->getTexto("menu.maps.add"));
        accionAdd->setIcon(QIcon(":iconos/images/maps.png"));
        connect(accionAdd, SIGNAL(triggered(bool)), this, SLOT(cargarImagenMapa()));
        ui->menuMapas->addAction(accionAdd);
        //acciones.push_back(accionAdd);

        return;
    }
}

void MainWindow::cargarMapaPrevio(QObject* mapaObjeto)
{
    if (datosProyecto.imagenesMapa.size() > 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(Idioma::getInstancia()->getTexto("ventana.mapas.cargar.titulo"));
        msgBox.setText(Idioma::getInstancia()->getTexto("ventana.mapas.cargar.mensaje"));
        msgBox.setInformativeText(Idioma::getInstancia()->getTexto("ventana.mapas.cargar.informacion"));
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        int opcion = msgBox.exec();
        if (opcion == QMessageBox::No) {
            return;
        }
    }

    ImagenMapa* mapa = dynamic_cast<ImagenMapa*>(mapaObjeto);
    mapa->punto = QPointF(0,0);
    mapas.clear();
    mapas.push_back(mapa);
    datosProyecto.imagenesMapa = mapas;
    //datosProyecto.tipoCoordenada = Enumerados::LatLng;

    HerramientaConversorCoordenadas::getInstancia()->establecerDatos(Enumerados::LatLng, mapas[0]->latitud1,
                                                                                                    mapas[0]->longitud1,
                                                                                                    mapas[0]->latitud2,
                                                                                                    mapas[0]->longitud2,
                                                                                                    mapas[0]->widthOriginal,
                                                                                                    mapas[0]->heigthOriginal);
    HerramientaConversorCoordenadas::getInstancia()->imagenReescalada = *mapas[0]->imagen;
    HerramientasVisor::getInstancia()->factorEscalado = 1;
    HerramientasVisor::getInstancia()->dx = 0;
    HerramientasVisor::getInstancia()->dy = 0;
    HerramientasVisor::getInstancia()->dzx = 0;
    HerramientasVisor::getInstancia()->dzy = 0;

    int dx = abs((mapas[0]->imagen->width() - visor->width()) / 2);
    int dy = abs((mapas[0]->imagen->height() - visor->height()) / 2);
    HerramientasVisor::getInstancia()->acumularDesplazamiento(-dx, -dy);
    visor->desplazarImagenesMapa(-dx, -dy);
    if (datosProyecto.imagenesMapa.size() > 0) {
        activarBotones(true);
    } else {
        activarBotones(false);
    }
    gestorWayPoints->refrescarWayPoints();
}

void MainWindow::abrirProyecto()
{
    if (gestorWayPoints->wayPoints.size() != 0 || mapas.size() != 0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setWindowTitle(Idioma::getInstancia()->getTexto("ventana.cambios.titulo"));
        msgBox.setText(Idioma::getInstancia()->getTexto("ventana.cambios.mensaje"));
        msgBox.setInformativeText(Idioma::getInstancia()->getTexto("ventana.cambios.informacion"));
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        int opcion = msgBox.exec();
        if (opcion == QMessageBox::No) {
            return;
        }
    }

    QString fichero = QFileDialog::getOpenFileName(this, Idioma::getInstancia()->getTexto("ventana.abrirmision.titulo"), ".",
                                       tr("Xml files (*.xml)"));
    if (fichero == "") {
        return;
    }
    datosProyecto.limpiar();
    reiniciarMision();
    if (!GestorXML::leerProyecto(fichero, &datosProyecto)) {
        QMessageBox::critical(this, Idioma::getInstancia()->getTexto("ventana.abrirmision.error.titulo"), Idioma::getInstancia()->getTexto("ventana.abrirmision.error.mensaje"));
        return;
    }

    establecerDatosProyecto(false);
    datosProyecto.guardado = true;
    datosProyecto.nombreFichero = fichero;

    mapas = datosProyecto.imagenesMapa;
    actualizarWayPointsAlturas();
    gestorWayPoints->refrescarWayPoints();
    QMessageBox::information(this, Idioma::getInstancia()->getTexto("ventana.abrirmision.titulo"), Idioma::getInstancia()->getTexto("ventana.abrirmision.fincarga.mensaje"));

    if (datosProyecto.imagenesMapa.size() > 0) {
        activarBotones(true);
    } else {
        activarBotones(false);
    }

    if (datosProyecto.tipoCoordenada == Enumerados::LatLng) {
        ui->coordenadasComboBox->setCurrentIndex(0);
        ui->utmGroupBox->setVisible(false);
    } else {
        ui->coordenadasComboBox->setCurrentIndex(1);
        ui->utmGroupBox->setVisible(true);
    }

#ifndef MODO_PORTATIL
    visorAlturasDock->show();
    ui->tabWidget->show();
#else
    visorAlturasDock->hide();
    ui->tabWidget->hide();
    accionMostrarDatosMision->setChecked(false);
    accionMostrarVisorAlturas->setChecked(false);
#endif
}


void MainWindow::guardarProyecto()
{
    if (datosProyecto.imagenesMapa.size() <= 0) {
        return;
    }

    QString nombreFichero = datosProyecto.nombreFichero;
    if (!datosProyecto.guardado) {
        nombreFichero = QFileDialog::getSaveFileName(this, Idioma::getInstancia()->getTexto("ventana.guardar.finguardado.titulo"), ".",
                                           tr("Xml files (*.xml)"));

    }
    if (nombreFichero == "") {
        return;
    }
    if (!establecerDatosProyecto(true)) {
        QMessageBox::critical(this, Idioma::getInstancia()->getTexto("ventana.guardar.error.titulo"), Idioma::getInstancia()->getTexto("ventana.guardar.error.mensaje1"));
        return;
    }

    if (!GestorXML::guardarProyecto(nombreFichero, datosProyecto)) {
        QMessageBox::critical(this, Idioma::getInstancia()->getTexto("ventana.guardar.error.titulo"), Idioma::getInstancia()->getTexto("ventana.guardar.error.mensaje2"));
        return;
    }
    datosProyecto.nombreFichero = nombreFichero;
    datosProyecto.guardado = true;
    QMessageBox::information(this, Idioma::getInstancia()->getTexto("ventana.guardar.finguardado.titulo"), Idioma::getInstancia()->getTexto("ventana.guardar.finguardado.mensaje"));
}

void MainWindow::crearWPAnteriorMarcadoHotKeyPulsada()
{
    visor->setFocus();
    if (datosProyecto.imagenesMapa.size() <= 0) {
        return;
    }
    if (gestorWayPoints->getCircuit() != Q_NULLPTR) {
        return;
    }
    visor->crearNuevoWPAntesDelMarcado();
    actualizarWayPointsAlturas();
}

void MainWindow::habilitarSeleccionar()
{
    seleccionarWaypoints = !seleccionarWaypoints;
    crearGrid = false;
    crearWP = false;
    medirDistancias = false;
    accionCrearWayPoints->setChecked(false);
    accionCrearPoligonos->setChecked(false);
    accionReglaAngulo->setChecked(false);
    accionSeleccionar->setChecked(seleccionarWaypoints);
    visor->activarMedirDistanciaYAngulo(medirDistancias);
    visor->activarCreacionGrid(crearGrid);
    visor->activarCreacionWP(crearWP);
    visor->seleccionarWaypoints = seleccionarWaypoints;
    if (seleccionarWaypoints) {
        QApplication::setOverrideCursor(Qt::CrossCursor);
    } else {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}

void MainWindow::habilitarMedirDistanciasYAngulo()
{
    visor->setFocus();
    if (datosProyecto.imagenesMapa.size() <= 0) {
        return;
    }

    //visor->medidores.clear();
    crearGrid = false;
    crearWP = false;
    seleccionarWaypoints = false;
    accionCrearWayPoints->setChecked(false);
    accionCrearPoligonos->setChecked(false);
    accionSeleccionar->setChecked(false);
    visor->activarCreacionGrid(crearGrid);
    visor->activarCreacionWP(crearWP);
    visor->seleccionarWaypoints = false;
    medirDistancias = !medirDistancias;
    accionReglaAngulo->setChecked(medirDistancias);
    visor->activarMedirDistanciaYAngulo(medirDistancias);

    if (medirDistancias) {
        QApplication::setOverrideCursor(Qt::CrossCursor);
    } else {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}

void MainWindow::gridHotKeyPulsada()
{
    visor->setFocus();
    if (datosProyecto.imagenesMapa.size() <= 0) {
        return;
    }
    if (gestorWayPoints->getCircuit() != Q_NULLPTR) {
        return;
    }
    crearGrid = !crearGrid;
    crearWP = false;
    medirDistancias = false;
    seleccionarWaypoints = false;
    accionCrearWayPoints->setChecked(false);
    accionReglaAngulo->setChecked(false);    
    accionSeleccionar->setChecked(false);
    visor->activarMedirDistanciaYAngulo(medirDistancias);
    visor->seleccionarWaypoints = false;
    gestorWayPoints->marcarTodosWayPoint(false);
    gestorGrids->activarCreacionGrid(crearGrid);
    if (!crearGrid) {
        if (gestorGrids->grids[gestorGrids->grids.size() - 1]->getNumeroPuntos() - 1 < 3) {
            gestorGrids->grids.pop_back();
        } else {
            gestorGrids->grids[gestorGrids->grids.size() - 1]->marcado = true;
            cargarInformacionGrid(gestorGrids->grids[gestorGrids->grids.size() - 1]);
            OperacionRealizada* operacionCrearGrid = new OperacionRealizada();
            operacionCrearGrid = new OperacionRealizada();
            operacionCrearGrid->accion = Enumerados::Crear;
            operacionCrearGrid->tipoObjeto = Enumerados::ObjetoGrid;
            operacionCrearGrid->grid = gestorGrids->getGridMarcado();
            GestorHistoricoOperaciones::getInstancia()->apilar(operacionCrearGrid);
        }
    }

    visor->activarCreacionWP(crearWP);
    accionCrearPoligonos->setChecked(crearGrid);

    if (crearGrid) {
        QApplication::setOverrideCursor(Qt::CrossCursor);
    } else {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}

void MainWindow::wpHotKeyPulsada()
{
    visor->setFocus();
    if (datosProyecto.imagenesMapa.size() <= 0) {
        return;
    }
    if (gestorWayPoints->getCircuit() != Q_NULLPTR) {
        return;
    }
    crearWP = !crearWP;
    crearGrid = false;
    medirDistancias = false;
    seleccionarWaypoints = false;
    accionSeleccionar->setChecked(false);
    accionCrearPoligonos->setChecked(false);
    accionReglaAngulo->setChecked(false);
    visor->seleccionarWaypoints = false;
    visor->activarMedirDistanciaYAngulo(medirDistancias);
    visor->activarCreacionWP(crearWP);
    visor->activarCreacionGrid(crearGrid);    
    accionCrearWayPoints->setChecked(crearWP);

    if (crearWP) {
        QApplication::setOverrideCursor(Qt::CrossCursor);
    } else {
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    }
}

void MainWindow::establecerLanding()
{
    visor->landingEstablecido();    
    gestorGrids->eliminarTodosGrids();
    actualizarTiempoDeVuelo();
}

void MainWindow::marcarWPVisor(const QItemSelection& seleccion)
{
//    if (seleccion.indexes().isEmpty()) {

//    } else {
//        int indice = seleccion.indexes().first().row();
//        visor->marcarWayPoint(wayPointsListView[indice]->orden);
//    }
}

void MainWindow::updateWPLista()
{        
    quitarWpDock();
    Avion* avion = getAvionSeleccionado();
    QString tiempo = calcularTiempoVuelo(gestorWayPoints->getDistanciaTotalVuelo(), avion->velocidadCrucero);
    ui->tiempoVueloTextEdit->setText(tiempo);
    actualizarTiempoDeVuelo();
}

void MainWindow::updateWPListaInformacion()
{    
    quitarWpDock();
    actualizarWayPointsAlturas();
    actualizarTiempoDeVuelo();

}

void MainWindow::actualizarWayPointsAlturas()
{
    visorAlturasWidget->limpiarWayPoints();
    for (int i = 0; i < gestorWayPoints->wayPoints.size(); i++) {
        visorAlturasWidget->addWayPoint(gestorWayPoints->wayPoints[i]);
    }
    visorAlturasWidget->update();
}

void MainWindow::actualizarListaWayPointsDespuesLanding(int posicion)
{   
    actualizarWayPointsAlturas();
}

void MainWindow::delWPLista(WayPoint* wp)
{   
    if (wp == Q_NULLPTR) return;

    if (wp->tipo == Enumerados::Circuit) {
        delete wp->aterrizaje;
        wp->aterrizaje = Q_NULLPTR;
         activarBotonesEdicion(true);
    }

    visorAlturasWidget->borrarWP(wp);

    if (gestorWayPoints->wayPoints.size() > 0) {
        gestorWayPoints->wayPoints[0]->distanciaWPAnterior = 0;
    }


    actualizarWayPointsAlturas();
    if (datosProyecto.imagenesMapa.size() > 0) {
        activarBotones(true);
    } else {
        activarBotones(false);
    }
}

void MainWindow::marcarWPLista(int posicion)
{
    cargarInformacionWayPoint(gestorWayPoints->wayPoints[posicion]);
}

void MainWindow::addWPLista(WayPoint* wp)
{
    if (wp == Q_NULLPTR) return;
    actualizarWayPointsAlturas();
    actualizarTiempoDeVuelo();
    if (wp->tipo == Enumerados::Circuit) {
        activarBotonesEdicion(false);
    }
}

