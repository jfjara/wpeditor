#include "griddockwidget.h"
#include "ui_griddockwidget.h"
#include "formateador.h"

GridDockWidget::GridDockWidget(Grid* grid, QList<Camara*> camaras, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::GridDockWidget)
{
    ui->setupUi(this);
    this->grid = grid;
    this->camaras = camaras;

    //ui->areaGridTextEdit->setText(QString::number(grid->getArea()));

    ui->areaGridTextEdit->setText(Formateador::formatearDecimal(grid->getArea(), 2));

    QStringList listaCamarasCombo;
    for (int i = 0; i < camaras.size(); i++) {
        QString nombreCamara = camaras[i]->getNombreCamara();
        listaCamarasCombo.push_back(nombreCamara);
    }
    ui->comboBox->addItems(listaCamarasCombo);

    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(1);
    ui->horizontalLayout_13->setEnabled(false);

    if (grid->mostrarPrevia) {
        ui->visualizarRutaCheckBox->setChecked(grid->mostrarPrevia);
        ui->alturaVueloTextEdit->setText(Formateador::formatearEntero(grid->altura));
        ui->anguloSpinBox->setValue(grid->angulo);
        ui->comboBox->setCurrentIndex(grid->camaraSeleccionada);
        ui->gsdText->setText(Formateador::formatearDecimal(grid->gsd, 2));
        //ui->gsdText->setText(QString::number(grid->gsd, 'f', 3));
        ui->overshotTextEdit->setText(QString::number(grid->overshot));
        ui->overshotLatEditText->setText(QString::number(grid->overshotLateral));        
        ui->espacioLineasTextEdit->setText(Formateador::formatearEntero(grid->espacioLineas));

    } else {
        grid->camaraSeleccionada = 0;
        recargarDatosCamara(0);
    }

//    connect(ui->hadBoton, SIGNAL(clicked(bool)), this, SLOT(mostrarPrevisualizacionCajaEnvolventeHAD()));
//    connect(ui->haiBoton, SIGNAL(clicked(bool)), this, SLOT(mostrarPrevisualizacionCajaEnvolventeHAI()));
//    connect(ui->habdBoton, SIGNAL(clicked(bool)), this, SLOT(mostrarPrevisualizacionCajaEnvolventeHABD()));
//    connect(ui->habiBoton, SIGNAL(clicked(bool)), this, SLOT(mostrarPrevisualizacionCajaEnvolventeHABI()));
//    connect(ui->vadBoton, SIGNAL(clicked(bool)), this, SLOT(mostrarPrevisualizacionCajaEnvolventeVAD()));
//    connect(ui->vaiBoton, SIGNAL(clicked(bool)), this, SLOT(mostrarPrevisualizacionCajaEnvolventeVAI()));
//    connect(ui->vabdBoton, SIGNAL(clicked(bool)), this, SLOT(mostrarPrevisualizacionCajaEnvolventeVABD()));
//    connect(ui->vabiBoton, SIGNAL(clicked(bool)), this, SLOT(mostrarPrevisualizacionCajaEnvolventeVABI()));
    connect(ui->verCajaCheckBox, SIGNAL(clicked(bool)), this, SLOT(mostrarCajaEnvolvente()));
    connect(ui->cerrarBoton, SIGNAL(clicked(bool)), this, SLOT(cerrar()));
    connect(ui->aplicarBoton, SIGNAL(clicked(bool)), this, SLOT(aceptar()));    
    connect(ui->anguloSpinBox, SIGNAL(valueChanged(int)), this, SLOT(girarRuta()));
    connect(ui->anguloDial, SIGNAL(valueChanged(int)), this, SLOT(girarRutaDial()));
    connect(ui->visualizarRutaCheckBox, SIGNAL(clicked(bool)), this, SLOT(mostrarPreviaRuta()));
    connect(ui->solapeDelanteroSpinBox, SIGNAL(valueChanged(int)), this, SLOT(actualizarSolapeDelantero()));
    connect(ui->solapeLateralSpinBox, SIGNAL(valueChanged(int)), this, SLOT(actualizarSolapeLateral()));
    connect(ui->alturaVueloTextEdit, SIGNAL(editingFinished()), this, SLOT(actualizarAlturaVuelo()));
    connect(ui->gsdText, SIGNAL(editingFinished()), this, SLOT(actualizarGSD()));
    connect(ui->overshotTextEdit, SIGNAL(editingFinished()), this, SLOT(actualizarOvershot()));
    connect(ui->overshotLatEditText, SIGNAL(editingFinished()), this, SLOT(actualizarOvershot()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(recargarDatosCamara(int)));
    connect(ui->optimizarBoton, SIGNAL(clicked(bool)), this, SLOT(calcularRecorridoMasOptimo()));

   // ui->optimizarBoton->setVisible(false);

    crearValidadores();
    traducir();
}

void GridDockWidget::calcularRecorridoMasOptimo()
{
    angulo = grid->obtenerAnguloRecorridoMasOptimo();
    grid->angulo = angulo;
    disconnect(ui->anguloSpinBox, SIGNAL(valueChanged(int)));
    disconnect(ui->anguloDial, SIGNAL(valueChanged(int)));
    ui->anguloDial->setValue(angulo);
    ui->anguloSpinBox->setValue(angulo);
    grid->crearRecorrido(angulo);    
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
    connect(ui->anguloDial, SIGNAL(valueChanged(int)), this, SLOT(girarRutaDial()));
    connect(ui->anguloSpinBox, SIGNAL(valueChanged(int)), this, SLOT(girarRuta()));
}

void GridDockWidget::mostrarPreviaRuta()
{
    grid->overshot =  ui->overshotTextEdit->text().toInt();
    grid->solapeLateral =  ui->solapeLateralSpinBox->value();
    grid->espacioLineas =  ui->espacioLineasTextEdit->text().toInt();
    grid->gsd = Formateador::formatearDecimal(ui->gsdText->text(), 2);
    //grid->gsd = ui->gsdText->text().toDouble();
    grid->altura = ui->alturaVueloTextEdit->text().toInt();
   //grid->espacioLineas =  ui->espacioLineasTextEdit->text().toInt();
    grid->mostrarPrevia = ui->visualizarRutaCheckBox->isChecked();    
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
}

void GridDockWidget::girarRuta()
{
    angulo = ui->anguloSpinBox->value();
    ui->anguloDial->setValue(angulo);
    grid->angulo = angulo;
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
}

void GridDockWidget::girarRutaDial()
{
    angulo = ui->anguloDial->value();
    ui->anguloSpinBox->setValue(angulo);
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
}


void GridDockWidget::traducir()
{
    setWindowTitle(Idioma::getInstancia()->getTexto("grid.titulo"));
    ui->alturaLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.altura"));
    ui->aplicarBoton->setText(Idioma::getInstancia()->getTexto("general.aplicar"));    
    ui->areaLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.area"));
    ui->camaraLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.camara"));
    ui->cerrarBoton->setText(Idioma::getInstancia()->getTexto("general.cerrar"));
    ui->espacioLineasLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.espaciolineas"));
    ui->metros1Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->metros2Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->metros3Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->metros4Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->meters5Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->porcentaje1Label->setText(Idioma::getInstancia()->getTexto("general.porcentaje"));
    ui->porcentaje2Label->setText(Idioma::getInstancia()->getTexto("general.porcentaje"));
    ui->overshotLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.overshot"));
    ui->overshotlatLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.overshotlat"));
    ui->solapedelanteroLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.solapedelantero"));
    ui->solapelateralLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.solapelateral"));
    ui->verCajaCheckBox->setText(Idioma::getInstancia()->getTexto("grid.irregulares.vercaja"));
    ui->cajaRadioButton->setText(Idioma::getInstancia()->getTexto("grid.irregulares.caja"));
    ui->tabWidget->setTabText(0, Idioma::getInstancia()->getTexto("grid.irregulares.titulo"));
    ui->tabWidget->setTabText(1, Idioma::getInstancia()->getTexto("grid.basicos.titulo"));
    ui->tabWidget->setTabText(2, Idioma::getInstancia()->getTexto("grid.opciones.titulo"));
    ui->tabWidget->setTabText(3, Idioma::getInstancia()->getTexto("grid.opciones.camara"));

    ui->visualizarRutaCheckBox->setText(Idioma::getInstancia()->getTexto("grid.opciones.mostrarruta"));
    ui->anguloLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.angulo"));
    ui->distanciaTotalLabel->setText(Idioma::getInstancia()->getTexto("grid.opciones.distanciatotal"));
    ui->optimizarBoton->setText(Idioma::getInstancia()->getTexto("grid.opciones.optimizar"));
}

GridDockWidget::~GridDockWidget()
{
    delete ui;
}

void GridDockWidget::crearValidadores()
{
    ui->overshotLatEditText->setValidator(new QIntValidator(1, 1000, this));
    ui->overshotTextEdit->setValidator(new QIntValidator(1, 1000, this));
    ui->gsdText->setValidator(new DoubleValidator(this));
}

void GridDockWidget::recargarDatosCamara(int indice)
{
    if (indice >= camaras.size()) {
        return;
    }
    Camara* camara = camaras[indice];
    ui->alturaVueloTextEdit->setText(QString::number(camara->fligthHeightInMeters));
    ui->solapeDelanteroSpinBox->setValue(camara->forwardOverlapInPercent);
    ui->solapeLateralSpinBox->setValue(camara->sideLapInPercent);
    ui->gsdText->setText(Formateador::formatearDecimal(camara->getImageryGSDInCm(), 2));
    ui->espacioLineasTextEdit->setText(Formateador::formatearEntero(camara->getFlightLineSpacingInMeters()));
    grid->camaraSeleccionada = indice;
    grid->gsd = camara->getImageryGSDInCm();
    grid->altura = camara->fligthHeightInMeters;
    grid->espacioLineas =  ui->espacioLineasTextEdit->text().toInt();    
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));    
}

void GridDockWidget::actualizarSolapeLateral()
{
    int indice = ui->comboBox->currentIndex();
    Camara* camara = camaras[indice];
    camara->sideLapInPercent = ui->solapeLateralSpinBox->value();    
    ui->espacioLineasTextEdit->setText(Formateador::formatearEntero(camara->getFlightLineSpacingInMeters()));
    grid->solapeLateral =  ui->solapeLateralSpinBox->value();
    grid->espacioLineas =  ui->espacioLineasTextEdit->text().toInt();    
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
}

void GridDockWidget::actualizarSolapeDelantero()
{
    int indice = ui->comboBox->currentIndex();
    Camara* camara = camaras[indice];
    camara->forwardOverlapInPercent = ui->solapeDelanteroSpinBox->value();
    ui->espacioLineasTextEdit->setText(QString::number(camara->getFlightLineSpacingInMeters()));    
    grid->solapeDelantero = ui->solapeDelanteroSpinBox->value();
    grid->espacioLineas =  ui->espacioLineasTextEdit->text().toInt();    
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
}

void GridDockWidget::actualizarAlturaVuelo()
{
    int indice = ui->comboBox->currentIndex();
    Camara* camara = camaras[indice];
    camara->fligthHeightInMeters = ui->alturaVueloTextEdit->text().toInt();    
    double v = camara->getImageryGSDInCm();
    ui->gsdText->setText(Formateador::formatearDecimal(v, 2));
    ui->espacioLineasTextEdit->setText(Formateador::formatearEntero(camara->getFlightLineSpacingInMeters()));
    grid->altura = ui->alturaVueloTextEdit->text().toInt();
    grid->espacioLineas =  ui->espacioLineasTextEdit->text().toInt();    
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
    ui->alturaVueloTextEdit->setText(Formateador::formatearEntero(camara->fligthHeightInMeters));
}

void GridDockWidget::actualizarGSD()
{
    int indice = ui->comboBox->currentIndex();
    Camara* camara = camaras[indice];
    camara->gsd = Formateador::formatearDecimal(ui->gsdText->text(), 2);
    grid->gsd = camara->gsd;
    camara->fligthHeightInMeters = round((camara->gsd * camara->lensFocalLengthInMM * 100) / camara->getPixelsInMM() / 10000);    
    ui->alturaVueloTextEdit->setText(Formateador::formatearEntero(camara->fligthHeightInMeters));
    ui->espacioLineasTextEdit->setText(Formateador::formatearEntero(camara->getFlightLineSpacingInMeters()));
    grid->espacioLineas =  ui->espacioLineasTextEdit->text().toInt();    
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
    ui->gsdText->setText(Formateador::formatearDecimal(grid->gsd, 2));
}

void GridDockWidget::actualizarOvershot()
{
    grid->overshot =  ui->overshotTextEdit->text().toInt();
    grid->overshotLateral =  ui->overshotLatEditText->text().toInt();
    grid->espacioLineas =  ui->espacioLineasTextEdit->text().toInt();    
    ui->distanciaRecorridoText->setText(Formateador::formatearDecimal(grid->calcularDistanciaRecorrido(), 2));
}

void GridDockWidget::cerrar()
{
    this->hide();
    grid->puntosRecorridoCalculado.clear();
}

void GridDockWidget::aceptar()
{
    grid->altura = ui->alturaVueloTextEdit->text().toInt();
    emit crearWayPointsPorGrid(grid);
    cerrar();
}

void GridDockWidget::mostrarCajaEnvolvente()
{
    grid->mostrarCajaEnvolvente = !grid->mostrarCajaEnvolvente;
}

void GridDockWidget::marcarBotonesCajasEnvolventes(bool marcar, QPushButton* botonPulsado)
{
    if (botonPulsado != ui->hadBoton) {
        ui->hadBoton->setChecked(marcar);
    }
    if (botonPulsado != ui->haiBoton) {
        ui->haiBoton->setChecked(marcar);
    }
    if (botonPulsado != ui->habdBoton) {
        ui->habdBoton->setChecked(marcar);
    }
    if (botonPulsado != ui->habiBoton) {
        ui->habiBoton->setChecked(marcar);
    }
    if (botonPulsado != ui->vadBoton) {
        ui->vadBoton->setChecked(marcar);
    }
    if (botonPulsado != ui->vaiBoton) {
        ui->vaiBoton->setChecked(marcar);
    }
    if (botonPulsado != ui->vabdBoton) {
        ui->vabdBoton->setChecked(marcar);
    }
    if (botonPulsado != ui->vabiBoton) {
        ui->vabiBoton->setChecked(marcar);
    }
}

void GridDockWidget::mostrarPrevisualizacionCajaEnvolventeHAD()
{
     int overshot = ui->overshotTextEdit->text().toInt();
     int overshotLat = ui->overshotLatEditText->text().toInt();
     bool mostrarPrevia = true;
     if (!ui->hadBoton->isChecked()) {
         mostrarPrevia = false;
     } else {
         marcarBotonesCajasEnvolventes(false, ui->hadBoton);
     }
     int espacioLineas = ui->espacioLineasTextEdit->text().toInt();
     grid->crearRecorridoCajaEnvolvente(Enumerados::Horizontal, Enumerados::DDerecha, Enumerados::DArriba, HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLat));
     grid->dibujarRecorridoCalculado = mostrarPrevia;
}

void GridDockWidget::mostrarPrevisualizacionCajaEnvolventeHAI()
{
    int overshot = ui->overshotTextEdit->text().toInt();
    int overshotLat = ui->overshotLatEditText->text().toInt();
    bool mostrarPrevia = true;
     if (!ui->haiBoton->isChecked()) {
         mostrarPrevia = false;
     } else {
         marcarBotonesCajasEnvolventes(false, ui->haiBoton);
     }
     int espacioLineas = ui->espacioLineasTextEdit->text().toInt();
     grid->crearRecorridoCajaEnvolvente(Enumerados::Horizontal, Enumerados::DIzquierda, Enumerados::DArriba, HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLat));
     grid->dibujarRecorridoCalculado = mostrarPrevia;
}

void GridDockWidget::mostrarPrevisualizacionCajaEnvolventeHABD()
{
    int overshot = ui->overshotTextEdit->text().toInt();
    int overshotLat = ui->overshotLatEditText->text().toInt();
     bool mostrarPrevia = true;
     if (!ui->habdBoton->isChecked()) {
         mostrarPrevia = false;
     } else {
         marcarBotonesCajasEnvolventes(false, ui->habdBoton);
     }
     int espacioLineas = ui->espacioLineasTextEdit->text().toInt();
     grid->crearRecorridoCajaEnvolvente(Enumerados::Horizontal, Enumerados::DDerecha, Enumerados::DAbajo, HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLat));
     grid->dibujarRecorridoCalculado = mostrarPrevia;
}

void GridDockWidget::mostrarPrevisualizacionCajaEnvolventeHABI()
{
    int overshot = ui->overshotTextEdit->text().toInt();
    int overshotLat = ui->overshotLatEditText->text().toInt();
     bool mostrarPrevia = true;
     if (!ui->habiBoton->isChecked()) {
         mostrarPrevia = false;
     } else {
         marcarBotonesCajasEnvolventes(false, ui->habiBoton);
     }
     int espacioLineas = ui->espacioLineasTextEdit->text().toInt();
     grid->crearRecorridoCajaEnvolvente(Enumerados::Horizontal, Enumerados::DIzquierda, Enumerados::DAbajo, HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLat));
     grid->dibujarRecorridoCalculado = mostrarPrevia;
}

void GridDockWidget::mostrarPrevisualizacionCajaEnvolventeVAD()
{
    int overshot = ui->overshotTextEdit->text().toInt();
    int overshotLat = ui->overshotLatEditText->text().toInt();
     bool mostrarPrevia = true;
     if (!ui->vadBoton->isChecked()) {
         mostrarPrevia = false;
     } else {
         marcarBotonesCajasEnvolventes(false, ui->vadBoton);
     }
     int espacioLineas = ui->espacioLineasTextEdit->text().toInt();
     grid->crearRecorridoCajaEnvolvente(Enumerados::Vertical, Enumerados::DDerecha, Enumerados::DArriba, HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLat));
     grid->dibujarRecorridoCalculado = mostrarPrevia;
}

void GridDockWidget::mostrarPrevisualizacionCajaEnvolventeVAI()
{
    int overshot = ui->overshotTextEdit->text().toInt();
    int overshotLat = ui->overshotLatEditText->text().toInt();
     bool mostrarPrevia = true;
     if (!ui->vaiBoton->isChecked()) {
         mostrarPrevia = false;
     } else {
         marcarBotonesCajasEnvolventes(false, ui->vaiBoton);
     }
     int espacioLineas = ui->espacioLineasTextEdit->text().toInt();
     grid->crearRecorridoCajaEnvolvente(Enumerados::Vertical, Enumerados::DIzquierda, Enumerados::DArriba, HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLat));
     grid->dibujarRecorridoCalculado = mostrarPrevia;
}

void GridDockWidget::mostrarPrevisualizacionCajaEnvolventeVABD()
{
    int overshot = ui->overshotTextEdit->text().toInt();
    int overshotLat = ui->overshotLatEditText->text().toInt();
     bool mostrarPrevia = true;
     if (!ui->vabdBoton->isChecked()) {
         mostrarPrevia = false;
     } else {
         marcarBotonesCajasEnvolventes(false, ui->vabdBoton);
     }
     int espacioLineas = ui->espacioLineasTextEdit->text().toInt();
     grid->crearRecorridoCajaEnvolvente(Enumerados::Vertical, Enumerados::DDerecha, Enumerados::DAbajo, HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLat));
     grid->dibujarRecorridoCalculado = mostrarPrevia;
}

void GridDockWidget::mostrarPrevisualizacionCajaEnvolventeVABI()
{
    int overshot = ui->overshotTextEdit->text().toInt();
    int overshotLat = ui->overshotLatEditText->text().toInt();
     bool mostrarPrevia = true;
     if (!ui->vabiBoton->isChecked()) {
         mostrarPrevia = false;
     } else {
         marcarBotonesCajasEnvolventes(false, ui->vabiBoton);
     }
     int espacioLineas = ui->espacioLineasTextEdit->text().toInt();
     grid->crearRecorridoCajaEnvolvente(Enumerados::Vertical, Enumerados::DIzquierda, Enumerados::DAbajo, HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, espacioLineas), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshot), HerramientaConversorCoordenadas::getInstancia()->getPixeles(Enumerados::LatLng, overshotLat));
     grid->dibujarRecorridoCalculado = mostrarPrevia;
}
