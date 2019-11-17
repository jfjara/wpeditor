#include "waypointdockwidget.h"
#include "ui_waypointwidget.h"
#include <QDebug>

WayPointDockWidget::WayPointDockWidget(Enumerados::Coordenadas tipo, WayPoint* wp, int numeroTotalWayPoints, QString zona, QWidget *parent)
    : QDockWidget(parent),
    ui(new Ui::WayPointDockWidget)
{
     ui->setupUi(this);
     this->wp = wp;
     this->numeroTotalWayPoints = numeroTotalWayPoints;
     this->tipoCoordenada = tipo;
     this->zona = zona;

     if (wp->tipo == Enumerados::Circuit) {
         if (wp->uDerecha) {
            ui->aterrizajeUDerechaBoton->setChecked(true);
            ui->aterrizajeUIzquierdaBoton->setChecked(false);
            establecerTipoAterrizajeUDerecha();
         } else {
            ui->aterrizajeUDerechaBoton->setChecked(false);
            ui->aterrizajeUIzquierdaBoton->setChecked(true);
            establecerTipoAterrizajeUIzquierda();
         }
     }

     rellenarCombos();
     rellenarDatosWayPoint();
     crearOpcionesComandosExtra();
     rellenarComandosExtra();

     if (wp->bloqueado) {
         ui->lockPushButton->setIcon(QIcon(":iconos/images/lock24.png"));
     } else {
         ui->lockPushButton->setIcon(QIcon(":iconos/images/unlock24.png"));
     }

     habilitarCampos(!wp->bloqueado);
     if (wp->tipo == Enumerados::Circulo) {
        ui->circuloHorizontalSlider->setValue(wp->diametro / 2);
        ui->radioSpinBox->setValue(wp->diametro / 2);
     }

     ui->lockPushButton->setStyleSheet("border:0px solid #ff0000;");

     connect(ui->tipoWPCombo , SIGNAL(currentIndexChanged(int)), this, SLOT(cambiarTipoWayPoint()));
     connect(ui->cerrarBoton, SIGNAL(clicked(bool)), this, SLOT(cerrarPanel()));
     connect(ui->aplicarBoton, SIGNAL(clicked(bool)), this, SLOT(salvarDatosWayPoint()));
     connect(ui->aterrizajeUDerechaBoton, SIGNAL(clicked(bool)), this, SLOT(establecerTipoAterrizajeUDerecha()));
     connect(ui->aterrizajeUIzquierdaBoton, SIGNAL(clicked(bool)), this, SLOT(establecerTipoAterrizajeUIzquierda()));
     connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(modificarHeadingAterrizaje(int)));
     connect(ui->circuloHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(modificarRadioCirculo(int)));
     connect(ui->radioSpinBox, SIGNAL(valueChanged(int)), this, SLOT(modificarRadioCirculo(int)));
     connect(ui->aterrizajeDial, SIGNAL(valueChanged(int)), this, SLOT(girarRutaDial()));
     connect(ui->lockPushButton, SIGNAL(clicked(bool)), this, SLOT(bloquearWaypoint()));

     traducir();

#ifdef MODO_PORTATIL
     ui->lockPushButton->setMaximumHeight(48);
     ui->lockPushButton->setMinimumHeight(48);
     ui->lockPushButton->setMinimumWidth(48);
     ui->lockPushButton->setIconSize(QSize(48,48));
     ui->aplicarBoton->setMinimumHeight(48);
     ui->cerrarBoton->setMinimumHeight(48);
     ui->aterrizajeDial->setMinimumHeight(48);
     ui->aterrizajeDial->setMinimumWidth(48);
#endif
}

WayPointDockWidget::~WayPointDockWidget()
{
}

void WayPointDockWidget::traducir()
{
    this->setWindowTitle(Idioma::getInstancia()->getTexto("waypoint.propiedades.titulo"));
    ui->waypointGrupo->setTitle(Idioma::getInstancia()->getTexto("waypoint.propiedades.waypoint"));
    ui->numeroWpLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.numero"));
    ui->latLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.latitud"));
    ui->lngLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.longitud"));
    ui->metrosLabel->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->alturaLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.altura"));
    ui->modoWpLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.modo"));
    ui->tipoWpLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.tipo"));
    ui->circuitGrupo->setTitle(Idioma::getInstancia()->getTexto("waypoint.propiedades.circuit.titulo"));
    ui->headingLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.circuit.heading"));
    ui->aterrizajeUDerechaBoton->setToolTip(Idioma::getInstancia()->getTexto("waypoint.propiedades.circuit.landRight.tooltip"));
    ui->aterrizajeUIzquierdaBoton->setToolTip(Idioma::getInstancia()->getTexto("waypoint.propiedades.circuit.landLeft.tooltip"));
    ui->gradosLabel->setText(Idioma::getInstancia()->getTexto("general.grados"));
    ui->propiedadesCirculoGroupBox->setTitle(Idioma::getInstancia()->getTexto("waypoint.propiedades.circulo.titulo"));
    ui->direccionLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.circulo.direccion"));
    ui->radioLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.circulo.radio"));
    ui->metros2Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->condicionesCirculoGroupBox->setTitle(Idioma::getInstancia()->getTexto("waypoint.propiedades.circulo.condiciones.titulo"));
    ui->niveladoLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.circulo.condiciones.nivelado"));
    ui->cuartosLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.circulo.condiciones.cuartos"));
    ui->headingCirculoLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.circulo.condiciones.heading"));
    ui->metros3Label->setText(Idioma::getInstancia()->getTexto("general.grados"));
    ui->cerrarBoton->setText(Idioma::getInstancia()->getTexto("general.cerrar"));
    ui->aplicarBoton->setText(Idioma::getInstancia()->getTexto("general.aceptar"));
    ui->repetirLabel->setText(Idioma::getInstancia()->getTexto("waypoint.propiedades.repetir"));
}

void WayPointDockWidget::establecerComandosExtra()
{
    wp->comandosExtras.clear();
    for (int i = 0; i < listadoObjetoComandoExtra.size(); i++) {
        if (listadoObjetoComandoExtra[i].grupo->isChecked()) {

        }
    }
}

void WayPointDockWidget::rellenarComandosExtra()
{
    for (int i = 0; i < wp->comandosExtras.size(); i++) {
        for (int j = 0; j < listadoObjetoComandoExtra.size(); j++) {
            if (listadoObjetoComandoExtra[j].comandoExtra.nombre == wp->comandosExtras[i]->nombre) {
                listadoObjetoComandoExtra[j].grupo->setChecked(true);
                listadoObjetoComandoExtra[j].registroText->setText(QString::number(wp->comandosExtras[i]->valor1));
                listadoObjetoComandoExtra[j].valorText->setText(QString::number(wp->comandosExtras[i]->valor2));
                break;
            }
        }
    }
}

void WayPointDockWidget::crearOpcionesComandosExtra()
{
    listadoObjetoComandoExtra.clear();
    if (GestorComandosExtra::getInstancia()->comandos.size() > 0) {
        for (int i = 0; i < GestorComandosExtra::getInstancia()->comandos.size(); i++) {
            QGroupBox* grupoComandosExtra = new QGroupBox();
            grupoComandosExtra->setCheckable(true);
            grupoComandosExtra->setChecked(false);
            grupoComandosExtra->setTitle(GestorComandosExtra::getInstancia()->comandos[i].nombre);
            grupoComandosExtra->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            grupoComandosExtra->setGeometry(grupoComandosExtra->geometry().x(), grupoComandosExtra->geometry().y(), 175, 134);
            grupoComandosExtra->setMinimumWidth(175);
            grupoComandosExtra->setMaximumWidth(175);
            QHBoxLayout* layoutComando = new QHBoxLayout();
            QLabel* registroLabel = new QLabel(Idioma::getInstancia()->getTexto("waypoint.extras.register"));
            registroLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            registroLabel->setGeometry(registroLabel->geometry().x(), registroLabel->geometry().y(), 40, 20);
            QLineEdit* registroText = new QLineEdit();
            registroText->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            registroText->setGeometry(registroText->geometry().x(), registroText->geometry().y(), 40, 20);
            if (GestorComandosExtra::getInstancia()->comandos[i].modo == 1 || GestorComandosExtra::getInstancia()->comandos[i].modo == 3) {
                registroText->setText(QString::number(GestorComandosExtra::getInstancia()->comandos[i].valor1));
            }
            QLabel* valorLabel = new QLabel(Idioma::getInstancia()->getTexto("waypoint.extras.value"));
            valorLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            valorLabel->setGeometry(valorLabel->geometry().x(), valorLabel->geometry().y(), 40, 20);
            QLineEdit* valorText = new QLineEdit();
            valorText->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            valorText->setGeometry(valorText->geometry().x(), valorText->geometry().y(), 40, 20);
            if (GestorComandosExtra::getInstancia()->comandos[i].modo != 1 || GestorComandosExtra::getInstancia()->comandos[i].modo != 2) {
                valorText->setText(QString::number(GestorComandosExtra::getInstancia()->comandos[i].valor2));
            }

            layoutComando->addWidget(registroLabel);
            layoutComando->addWidget(registroText);
            layoutComando->addWidget(valorLabel);
            layoutComando->addWidget(valorText);
            grupoComandosExtra->setLayout(layoutComando);
            ui->layoutComandosExtra->addWidget(grupoComandosExtra);

            ObjetoComandoExtraEncapsulado objetoEncapsulado;
            objetoEncapsulado.comandoExtra = GestorComandosExtra::getInstancia()->comandos[i];
            objetoEncapsulado.grupo = grupoComandosExtra;
            objetoEncapsulado.registroText = registroText;
            objetoEncapsulado.valorText = valorText;
            listadoObjetoComandoExtra.push_back(objetoEncapsulado);
        }

    }
}

void WayPointDockWidget::bloquearWaypoint()
{
    wp->bloqueado = !wp->bloqueado;
    if (wp->bloqueado) {
        ui->lockPushButton->setIcon(QIcon(":iconos/images/lock24.png"));
    } else {
        ui->lockPushButton->setIcon(QIcon(":iconos/images/unlock24.png"));
    }

#ifdef MODO_PORTATIL
    ui->lockPushButton->setMaximumHeight(48);
    ui->lockPushButton->setMinimumHeight(48);
    ui->lockPushButton->setMinimumWidth(48);
    ui->aplicarBoton->setMinimumHeight(48);
    ui->cerrarBoton->setMinimumHeight(48);

    ui->lockPushButton->setIconSize(QSize(48,48));
#endif

    habilitarCampos(!wp->bloqueado);
}

void WayPointDockWidget::girarRutaDial()
{
    angulo = ui->aterrizajeDial->value();
    ui->spinBox->setValue(angulo);
    modificarHeadingAterrizaje(angulo);
}

void WayPointDockWidget::modificarHeadingAterrizaje(int v)
{
    bool derecha = true;
    if (ui->aterrizajeUIzquierdaBoton->isChecked()) {
        derecha = false;
    }
    ui->aterrizajeDial->setValue(v);
    emit modificarAterrizaje(derecha, v);
}

void WayPointDockWidget::modificarRadioCirculo(int v)
{
    wp->diametro = v * 2;
    ui->spinBox->setValue(v);
    ui->radioSpinBox->setValue(v);
}

void WayPointDockWidget::establecerTipoAterrizajeUIzquierda()
{
    int pitch = ui->spinBox->value();
    ui->aterrizajeUDerechaBoton->setChecked(false);
    wp->uDerecha = false;
    emit modificarAterrizaje(false, pitch);
}

void WayPointDockWidget::establecerTipoAterrizajeUDerecha()
{
    int pitch = ui->spinBox->value();
    ui->aterrizajeUIzquierdaBoton->setChecked(false);
    wp->uDerecha = true;
    emit modificarAterrizaje(true, pitch);

}

void WayPointDockWidget::cerrarPanel()
{
    if (wp->tipo == Enumerados::Circuit) {
        emit modificarAterrizaje(wp->uDerecha, wp->headingCircuit);
    }
    this->hide();
}

void WayPointDockWidget::actualizaPaneles(Enumerados::TipoWayPoint tipoWP)
{
    switch (tipoWP) {   
    case Enumerados::Circulo:        
        ui->propiedadesCirculoGroupBox->setVisible(true);
        ui->condicionesCirculoGroupBox->setVisible(true);
        ui->circuitGrupo->setVisible(false);
        ui->modoWpLabel->setVisible(false);
        ui->modoWPComboBox->setVisible(false);
        break;
    case Enumerados::Circuit:
        ui->propiedadesCirculoGroupBox->setVisible(false);
        ui->condicionesCirculoGroupBox->setVisible(false);
        ui->circuitGrupo->setVisible(true);
        ui->modoWpLabel->setVisible(false);
        ui->modoWPComboBox->setVisible(false);
        break;
    case Enumerados::WayPoint:
        ui->circuitGrupo->setVisible(false);
        ui->propiedadesCirculoGroupBox->setVisible(false);
        ui->condicionesCirculoGroupBox->setVisible(false);
        ui->modoWpLabel->setVisible(true);
        ui->modoWPComboBox->setVisible(true);
        break;
    default:
        break;
    }
}

void WayPointDockWidget::cambiarTipoWayPoint()
{
    QString tipoWP = ui->tipoWPCombo->currentText();
    Enumerados::TipoWayPoint tipoSeleccionado =  tipoWayPointMapa.value(tipoWP);
    actualizaPaneles(tipoSeleccionado);   
}

void WayPointDockWidget::rellenarCombos()
{
    rellenarComboTipoWP();
    rellenarComboDireccion();
    rellenarComboNivelado();
    rellenarComboCuartosCirculo();
    rellenarComboModoWP();
    rellenarComboRepeticion();
}

void WayPointDockWidget::rellenarComboRepeticion()
{
    QList<QString> items;
    for (int i = 0; i <= numeroTotalWayPoints; i++) {
        items << QString::number(i);
    }
    ui->repetirComboBox->addItems(QStringList(items));
}

void WayPointDockWidget::rellenarComboTipoWP()
{
    int enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("TipoWayPoint");
    QMetaEnum meta = Enumerados::staticMetaObject.enumerator(enumIdx);
    for (int i = 0; i < meta.keyCount(); i++) {
        QString clave = QString::fromUtf8(meta.key(i));
        int valor = meta.keyToValue(clave.toUtf8().data());
        tipoWayPointMapa.insert(clave, static_cast<Enumerados::TipoWayPoint>(valor));
    }
    ui->tipoWPCombo->addItems(QStringList(tipoWayPointMapa.keys()));
}

void WayPointDockWidget::rellenarComboModoWP()
{
    int enumModoIdx = Enumerados::staticMetaObject.indexOfEnumerator("ModoWayPoint");
    QMetaEnum metaModoWP = Enumerados::staticMetaObject.enumerator(enumModoIdx);
    for (int i = 0; i < metaModoWP.keyCount(); i++) {
        QString clave = QString::fromUtf8(metaModoWP.key(i));
        int valor = metaModoWP.keyToValue(clave.toUtf8().data());
        modoWayPointMapa.insert(clave, static_cast<Enumerados::ModoWayPoint>(valor));
    }
    ui->modoWPComboBox->addItems(QStringList(modoWayPointMapa.keys()));
}

void WayPointDockWidget::rellenarComboDireccion()
{
    int enumDireccionIdx = Enumerados::staticMetaObject.indexOfEnumerator("DireccionCirculo");
    QMetaEnum metaCirculo = Enumerados::staticMetaObject.enumerator(enumDireccionIdx);
    for (int i = 0; i < metaCirculo.keyCount(); i++) {
        QString clave = QString::fromUtf8(metaCirculo.key(i));
        int valor = metaCirculo.keyToValue(clave.toUtf8().data());
        direccionCirculoMapa.insert(clave, static_cast<Enumerados::DireccionCirculo>(valor));
    }
    ui->direccionCirculoCombo->addItems(QStringList(direccionCirculoMapa.keys()));
}

void WayPointDockWidget::rellenarComboCuartosCirculo()
{
    QList<QString> items;
    items << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20" << "21" << "22" << "23" << "24";
    ui->cuartosCirculoCombo->addItems(QStringList(items));
}

void WayPointDockWidget::rellenarComboNivelado()
{
    QList<QString> items;
    items << "0" << "1";
    ui->niveladoCombo->addItems(QStringList(items));
}

void WayPointDockWidget::rellenarDatosWayPoint()
{


    QPointF coordenada = HerramientaConversorCoordenadas::getInstancia()->convertirCoordenada(tipoCoordenada, wp->punto.x() - HerramientasVisor::getInstancia()->getDesplazamientoX(), wp->punto.y() - HerramientasVisor::getInstancia()->getDesplazamientoY());
    double lat = coordenada.y();
    double lng = coordenada.x();

    ui->numeroWPEdit->setText(QString::number(wp->orden));
    if (tipoCoordenada == Enumerados::UTM) {
        ui->latitudEdit->setText(QString::number(lat, 'f', 0));
        ui->longitudEdit->setText(QString::number(lng, 'f', 0));
    } else {
        ui->latitudEdit->setText(QString::number(lat));
        ui->longitudEdit->setText(QString::number(lng));
    }
    ui->alturaSpinBox->setValue(wp->altura);

    int enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("TipoWayPoint");
    ui->tipoWPCombo->setCurrentText(QString::fromUtf8(Enumerados::staticMetaObject.enumerator(enumIdx).key(wp->tipo)));

    int enumModoIdx = Enumerados::staticMetaObject.indexOfEnumerator("ModoWayPoint");
    ui->modoWPComboBox->setCurrentText(QString::fromUtf8(Enumerados::staticMetaObject.enumerator(enumModoIdx).key(wp->modo)));

    ui->spinBox_2->setValue(wp->heading);
    enumIdx = Enumerados::staticMetaObject.indexOfEnumerator("DireccionCirculo");
    ui->direccionCirculoCombo->setCurrentText(QString::fromUtf8(Enumerados::staticMetaObject.enumerator(enumIdx).key(wp->direccionCirculo)));
    if (wp->cuartosCirculo == 1000000) {
        ui->cuartosCirculoCombo->setCurrentText("Infinito");
    } else {
        ui->cuartosCirculoCombo->setCurrentText(QString::number(wp->cuartosCirculo));
    }
    ui->niveladoCombo->setCurrentText(QString::number(wp->nivelado));
    ui->spinBox->setValue(wp->headingCircuit);

    ui->radioSpinBox->setValue(wp->diametro / 2);

    if (wp->indiceRepeticion > 0) {
        ui->repetirComboBox->setCurrentIndex(wp->indiceRepeticion);
    }

    actualizaPaneles(wp->tipo);
}

void WayPointDockWidget::habilitarCampos(bool valor)
{
    ui->alturaSpinBox->setEnabled(valor);
    ui->tipoWPCombo->setEnabled(valor);
    ui->modoWPComboBox->setEnabled(valor);
    ui->aterrizajeDial->setEnabled(valor);
    ui->spinBox->setEnabled(valor);
    ui->aterrizajeUDerechaBoton->setEnabled(valor);
    ui->aterrizajeUIzquierdaBoton->setEnabled(valor);
    ui->direccionCirculoCombo->setEnabled(valor);
    ui->radioSpinBox->setEnabled(valor);
    ui->niveladoCombo->setEnabled(valor);
    ui->cuartosCirculoCombo->setEnabled(valor);
    ui->spinBox_2->setEnabled(valor);
}

void WayPointDockWidget::salvarDatosComandosExtra()
{
    wp->comandosExtras.clear();
    for (int i = 0; i < listadoObjetoComandoExtra.size(); i++) {
        if (listadoObjetoComandoExtra[i].grupo->isChecked()) {
            ComandoExtra* comando = new ComandoExtra();
            comando->comando = listadoObjetoComandoExtra[i].comandoExtra.comando;
            comando->modo = listadoObjetoComandoExtra[i].comandoExtra.modo;
            comando->nombre = listadoObjetoComandoExtra[i].comandoExtra.nombre;
            comando->valor1 = listadoObjetoComandoExtra[i].registroText->text().toInt();
            comando->valor2 = listadoObjetoComandoExtra[i].valorText->text().toInt();
            wp->comandosExtras.push_back(comando);
        }
    }
}

void WayPointDockWidget::salvarDatosWayPoint()
{
    try {
        QString tipoWP = ui->tipoWPCombo->currentText();
        Enumerados::TipoWayPoint tipoSeleccionado =  tipoWayPointMapa.value(tipoWP);
        wp->tipo = tipoSeleccionado;

        QString modoWP = ui->modoWPComboBox->currentText();
        Enumerados::ModoWayPoint modoSeleccionado =  modoWayPointMapa.value(modoWP);
        wp->modo = modoSeleccionado;
        wp->altura = ui->alturaSpinBox->value();

        int indiceRepeticion = ui->repetirComboBox->currentIndex();
        //qDebug() << indiceRepeticion;
        if (indiceRepeticion > 0) {
            wp->indiceRepeticion = indiceRepeticion;
        }

        if (tipoCoordenada == Enumerados::UTM) {
            QPointF punto = HerramientaConversorCoordenadas::getInstancia()->convertirUTMALatLon(ui->longitudEdit->text().toDouble(), ui->latitudEdit->text().toDouble(), zona);
            wp->coordenadaX = punto.x();
            wp->coordenadaY = punto.y();
        } else if (tipoCoordenada == Enumerados::LatLng) {
            wp->coordenadaX = ui->longitudEdit->text().toDouble();
            wp->coordenadaY = ui->latitudEdit->text().toDouble();
        }
        wp->refrescarPosicion();

        salvarDatosComandosExtra();

        switch (tipoSeleccionado) {
            case Enumerados::Circulo:
                {
                    wp->headingCircuit = 0;
                    QString direccionCirculo = ui->direccionCirculoCombo->currentText();
                    Enumerados::DireccionCirculo direccionSeleccionada =  direccionCirculoMapa.value(direccionCirculo);
                    wp->direccionCirculo = direccionSeleccionada;
                    wp->heading = ui->spinBox_2->value();
                    if (ui->cuartosCirculoCombo->currentText() == "Infinito") {
                        wp->cuartosCirculo = 1000000;
                    } else {
                        wp->cuartosCirculo =  ui->cuartosCirculoCombo->currentText().toInt();
                    }
                    wp->nivelado = ui->niveladoCombo->currentText().toInt();                    
                    wp->diametro = ui->radioSpinBox->value() * 2;                    
                    break;
                }
            case Enumerados::WayPoint:
                wp->headingCircuit = 0;
                wp->heading = 0;
                wp->cuartosCirculo = 0;
                wp->nivelado = 0;
                wp->diametro = 0;                                          
                break;
            case Enumerados::Circuit:
                wp->headingCircuit = ui->spinBox->value();
                wp->heading = 0;
                wp->cuartosCirculo = 0;
                wp->nivelado = 0;
                wp->diametro = 0;
                wp->altura = 0;

                if (ui->aterrizajeUIzquierdaBoton->isChecked()) {
                    wp->uDerecha = false;
                }

                emit landingEstablecido();
                emit modificarHeadingAterrizaje(wp->headingCircuit);
                break;
            default:
                break;
        }

    } catch (...)
    {
        QMessageBox::critical(this, "Error de datos en el Way Point", "Datos introducidos incorrectos. Por favor, revise todos los campos.");
    }
//    WayPoint* wpClonado = gestorWayPoints->clonarWayPoint(wp);
//    OperacionRealizada operacionModificacionWP = new OperacionRealizada();
//    operacionModificacionWP = new OperacionRealizada();
//    operacionModificacionWP->accion = Enumerados::Modificar;
//    operacionModificacionWP->tipoObjeto = Enumerados::ObjetoWayPoint;
//    operacionModificacionWP->wp = wpClonado;
//    GestorHistoricoOperaciones::getInstancia()->apilar(operacionModificacionWP);
    emit datosActualizados();
}


