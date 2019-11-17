#include "camarasdialog.h"
#include "ui_camarasdialog.h"

CamarasDialog::CamarasDialog(QList<Camara*> camaras, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CamarasDialog)
{
    ui->setupUi(this);    
    crearValidadores();
    for (int i = 0; i < camaras.size(); i++) {
        addCamara(camaras[i]);
    }

    mostrarBotonesEdicion(true);
    mostrarBotonesAceptacion(false);

    connect(ui->crearBoton, SIGNAL(clicked(bool)), this, SLOT(crearCamara()));
    connect(ui->editarBoton, SIGNAL(clicked(bool)), this, SLOT(modificarCamara()));
    connect(ui->eliminarBoton, SIGNAL(clicked(bool)), this, SLOT(eliminarCamara()));
    connect(ui->aceptarBoton, SIGNAL(clicked(bool)), this, SLOT(guardarCamara()));
    connect(ui->cancelarBoton, SIGNAL(clicked(bool)), this, SLOT(cancelarEdicion()));
    connect(ui->exportarBoton, SIGNAL(clicked(bool)), this, SLOT(exportar()));

    connect(ui->botonera->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(aceptar()));
    traducir();

#ifdef MODO_PORTATIL

    ui->crearBoton->setMinimumHeight(48);
    ui->editarBoton->setMinimumHeight(48);
    ui->eliminarBoton->setMinimumHeight(48);
    ui->aceptarBoton->setMinimumHeight(48);
    ui->cancelarBoton->setMinimumHeight(48);
    ui->exportarBoton->setMinimumHeight(48);

#endif

}

CamarasDialog::~CamarasDialog()
{
    delete ui;

    for (int i = 0; i < listaRadioCamara.size(); i++) {
        delete listaRadioCamara[i];
    }
}

void CamarasDialog::traducir()
{
    this->setWindowTitle(Idioma::getInstancia()->getTexto("camara.titulo"));
    ui->camarasDefinidasLabel->setText(Idioma::getInstancia()->getTexto("camara.lista.definidas"));
    ui->propiedadesGrupo->setTitle(Idioma::getInstancia()->getTexto("camara.propiedades.titulo"));
    ui->marcaLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.marca"));
    ui->modeloLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.modelo"));
    ui->notasLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.notas"));
    ui->parametrosGrupo->setTitle(Idioma::getInstancia()->getTexto("camara.propiedades.parametros.titulo"));
    ui->anchoPxLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.parametros.ancho"));
    ui->altoPxLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.parametros.alto"));
    ui->px1Label->setText(Idioma::getInstancia()->getTexto("general.px"));
    ui->px2Label->setText(Idioma::getInstancia()->getTexto("general.px"));
    ui->focalGrupo->setTitle(Idioma::getInstancia()->getTexto("camara.propiedades.focal.titulo"));
    ui->anchoMMLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.focal.ancho"));
    ui->altoMMLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.focal.alto"));
    ui->mm1Label->setText(Idioma::getInstancia()->getTexto("general.mm"));
    ui->mm2Label->setText(Idioma::getInstancia()->getTexto("general.mm"));
    ui->tamFocalLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.focal.tamanofocal"));
    ui->tamPixelLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.focal.tamanopixel"));
    ui->mm3Label->setText(Idioma::getInstancia()->getTexto("general.mm"));
    ui->mm4Label->setText(Idioma::getInstancia()->getTexto("general.mm"));
    ui->velocidadObtLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.focal.velocidadobturador"));
    ui->segundosLabel->setText(Idioma::getInstancia()->getTexto("general.segundos"));
    ui->paramImageGrupo->setTitle(Idioma::getInstancia()->getTexto("camara.propiedades.parametrosimagenes.titulo"));
    ui->velocidadSueloLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.parametrosimagenes.velocidadsuelo"));
    ui->metroSegundoLabel->setText(Idioma::getInstancia()->getTexto("general.metrossegundos"));
    ui->alturaVueloLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.parametrosimagenes.alturavuelo"));
    ui->metros1Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->gsdLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.parametrosimagenes.imagenesgsd"));
    //ui->metros2Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->tamMarcoGrupo->setTitle(Idioma::getInstancia()->getTexto("camara.propiedades.marco.titulo"));
    ui->anchoMetrosLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.marco.ancho"));
    ui->metros3Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->longitudLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.marco.longitud"));
    ui->metros4Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->solapeDelLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.marco.solapedelantero"));
    ui->solapeLatLabel->setText(Idioma::getInstancia()->getTexto("camara.propiedades.marco.solapelateral"));
    ui->porcentaje1Label->setText(Idioma::getInstancia()->getTexto("general.porcentaje"));
    ui->porcentaje2Label->setText(Idioma::getInstancia()->getTexto("general.porcentaje"));
    ui->crearBoton->setText(Idioma::getInstancia()->getTexto("general.crear"));
    ui->editarBoton->setText(Idioma::getInstancia()->getTexto("general.editar"));
    ui->eliminarBoton->setText(Idioma::getInstancia()->getTexto("general.eliminar"));
    ui->aceptarBoton->setText(Idioma::getInstancia()->getTexto("general.aceptar"));
    ui->cancelarBoton->setText(Idioma::getInstancia()->getTexto("general.cancelar"));
    ui->exportarBoton->setText(Idioma::getInstancia()->getTexto("general.exportar"));
}

void CamarasDialog::aceptar()
{
    aceptado = true;
    hide();
}

void CamarasDialog::crearValidadores()
{
    ui->anchoPx->setValidator(new QIntValidator(1, 99999, this));
    ui->altoPx->setValidator(new QIntValidator(1, 99999, this));
    ui->anchoMM->setValidator(new DoubleValidator(this));
    ui->altoMM->setValidator(new DoubleValidator(this));
    ui->focalLenteMM->setValidator(new DoubleValidator(this));
    ui->velocidadSueloMSec->setValidator(new DoubleValidator(this));
    ui->alturaVueloM->setValidator(new QIntValidator(1, 100000, this));
    ui->solapeDelantero->setValidator(new QIntValidator(1, 100, this));
    ui->solapeLateral->setValidator(new QIntValidator(1, 100, this));
}

void CamarasDialog::addCamara(Camara* camara)
{
    QRadioButton* radio = new QRadioButton(this);
    radio->setText(camara->marca + " / " + camara->modelo);
    listaRadioCamara.push_back(radio);
    camaras.push_back(camara);
    addRadio(radio);
}

void CamarasDialog::addRadio(QRadioButton* radio)
{
    ui->layoutDefinicionCamaras->addWidget(radio);
    connect(radio, SIGNAL(clicked(bool)), this, SLOT(cargarDatosCamara()));
}

void CamarasDialog::alinear()
{
    for (int i = 0; i < ui->layoutDefinicionCamaras->count(); ++i) {
        QLayoutItem *layoutItem = ui->layoutDefinicionCamaras->itemAt(i);
        if (layoutItem->spacerItem()) {
            ui->layoutDefinicionCamaras->removeItem(layoutItem);
            return;
        }
    }
}

int CamarasDialog::getIndiceCamaraSeleccionada()
{
    for (int i = 0; i < listaRadioCamara.size(); i++) {
        if (listaRadioCamara[i]->isChecked()) {
            return i;
        }
    }
    return -1;
}

Camara* CamarasDialog::getCamaraSeleccionada()
{
    for (int i = 0; i < listaRadioCamara.size(); i++) {
        if (listaRadioCamara[i]->isChecked()) {
            return camaras[i];
        }
    }
    return Q_NULLPTR;
}

void CamarasDialog::cargarDatosCamara()
{
    rellenarFormulario(getCamaraSeleccionada());
    ui->eliminarBoton->setEnabled(true);
    ui->editarBoton->setEnabled(true);
}

void CamarasDialog::rellenarFormulario(Camara* camara)
{
    if (camara != Q_NULLPTR) {
        ui->marca->setText(camara->marca);
        ui->modelo->setText(camara->modelo);
        ui->notas->setText(camara->notas);
        ui->anchoPx->setText(Formateador::formatearEntero(camara->widthInPixels));
        ui->altoPx->setText(Formateador::formatearEntero(camara->heightInPixels));
        ui->anchoMM->setText(Formateador::formatearDecimal(camara->widthInMM, 2));
        ui->altoMM->setText(Formateador::formatearDecimal(camara->heightInMM, 2));
        ui->tamanoPixelMM->setText(Formateador::formatearDecimal(camara->getPixelsInMM(), 6));
        ui->focalLenteMM->setText(Formateador::formatearDecimal(camara->lensFocalLengthInMM, 2));
        ui->velocidadObturador->setText(Formateador::formatearDecimal(camara->getCameraShutterSpeedInSeconds(), 5));
        ui->velocidadSueloMSec->setText(Formateador::formatearDecimal(camara->platformGroundSpeedMetersSec, 2));
        ui->alturaVueloM->setText(Formateador::formatearEntero(camara->fligthHeightInMeters));
        ui->imagenesGSD->setText(Formateador::formatearDecimal(camara->getImageryGSDInCm(), 3));
        ui->anchoMetros->setText(Formateador::formatearDecimal(camara->getWidthInMeters(), 2));
        ui->longitudMetros->setText(Formateador::formatearDecimal(camara->getLengthInMeters(), 2));
        ui->solapeDelantero->setText(Formateador::formatearEntero(camara->forwardOverlapInPercent));
        ui->solapeLateral->setText(Formateador::formatearEntero(camara->sideLapInPercent));
    } else {
        ui->marca->setText("");
        ui->modelo->setText("");
        ui->notas->setText("");
        ui->anchoPx->setText("");
        ui->altoPx->setText("");
        ui->anchoMM->setText("");
        ui->altoMM->setText("");
        ui->tamanoPixelMM->setText("");
        ui->focalLenteMM->setText("");
        ui->velocidadObturador->setText("");
        ui->velocidadSueloMSec->setText("");
        ui->alturaVueloM->setText("");
        ui->imagenesGSD->setText("");
        ui->anchoMetros->setText("");
        ui->longitudMetros->setText("");
        ui->solapeDelantero->setText("");
        ui->solapeLateral->setText("");
    }
}

void CamarasDialog::activarCampos(bool valor)
{
    ui->marca->setEnabled(valor);
    ui->modelo->setEnabled(valor);
    ui->notas->setEnabled(valor);
    ui->anchoPx->setEnabled(valor);
    ui->altoPx->setEnabled(valor);
    ui->anchoMM->setEnabled(valor);
    ui->altoMM->setEnabled(valor);
    ui->focalLenteMM->setEnabled(valor);
    ui->velocidadSueloMSec->setEnabled(valor);
    ui->alturaVueloM->setEnabled(valor);
    ui->solapeDelantero->setEnabled(valor);
    ui->solapeLateral->setEnabled(valor);
}

void CamarasDialog::crearCamara()
{
    mostrarBotonesAceptacion(true);
    mostrarBotonesEdicion(false);
    rellenarFormulario(Q_NULLPTR);
    activarCampos(true);
    esModificacion = false;
}


void CamarasDialog::cancelarEdicion()
{
    mostrarBotonesAceptacion(false);
    mostrarBotonesEdicion(true);
    cargarDatosCamara();
    activarCampos(false);
    esModificacion = false;
}

void CamarasDialog::guardarDatosCamara(Camara* camara)
{
    camara->widthInPixels = Formateador::formatearEntero(ui->anchoPx->text());
    camara->heightInPixels = Formateador::formatearEntero(ui->altoPx->text());
    camara->widthInMM = Formateador::formatearDecimal(ui->anchoMM->text(), 2);
    camara->heightInMM = Formateador::formatearDecimal(ui->altoMM->text(), 2);
    camara->lensFocalLengthInMM = Formateador::formatearDecimal(ui->focalLenteMM->text(), 2);
    camara->platformGroundSpeedMetersSec = Formateador::formatearDecimal(ui->velocidadSueloMSec->text(), 2);
    camara->fligthHeightInMeters = Formateador::formatearEntero(ui->alturaVueloM->text());
    camara->forwardOverlapInPercent = Formateador::formatearEntero(ui->solapeDelantero->text());
    camara->sideLapInPercent = Formateador::formatearEntero(ui->solapeLateral->text());
    camara->modelo = ui->modelo->text();
    camara->marca = ui->marca->text();
    camara->notas = ui->notas->toPlainText();
}

void CamarasDialog::guardarCamara()
{

    if (ui->marca->text().isEmpty() || ui->modelo->text().isEmpty()) {
        QMessageBox msg;
        msg.setWindowTitle(Idioma::getInstancia()->getTexto("camara.validation.titulo"));
        msg.setText(Idioma::getInstancia()->getTexto("camara.validation.mensaje"));
        msg.exec();
        return;
    }

    if (esModificacion) {
        Camara* camara = getCamaraSeleccionada();
        guardarDatosCamara(camara);
    } else {
        Camara* camara = new Camara();
        guardarDatosCamara(camara);
        addCamara(camara);
        listaRadioCamara[listaRadioCamara.size()-1]->setChecked(true);
    }
    cargarDatosCamara();
    activarCampos(false);
    esModificacion = false;

    mostrarBotonesAceptacion(false);
    mostrarBotonesEdicion(true);
}

void CamarasDialog::quitarCamara(int indice)
{
    if (indice == -1 || indice >= camaras.size()) {
        return;
    }
    camaras.removeAt(indice);
    ui->layoutDefinicionCamaras->removeWidget(listaRadioCamara[indice]);
    delete listaRadioCamara[indice];
    listaRadioCamara.removeAt(indice);
    update();
}

void CamarasDialog::eliminarCamara()
{
    QMessageBox msg;
    msg.setWindowTitle(Idioma::getInstancia()->getTexto("camara.eliminar.titulo"));
    msg.setText(Idioma::getInstancia()->getTexto("camara.eliminar.mensaje"));
    msg.setInformativeText(Idioma::getInstancia()->getTexto("camara.eliminar.descripcion"));
    msg.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Cancel);

    int opcion = msg.exec();
    if (opcion == QMessageBox::Cancel) {
        return;
    }
    int indice = getIndiceCamaraSeleccionada();
    if (indice == -1) {
        return;
    }
    quitarCamara(indice);
    mostrarBotonesAceptacion(false);
    mostrarBotonesEdicion(true);
    rellenarFormulario(Q_NULLPTR);
    activarCampos(false);
    esModificacion = false;
}

void CamarasDialog::modificarCamara()
{
    mostrarBotonesAceptacion(true);
    mostrarBotonesEdicion(false);
    activarCampos(true);
    esModificacion = true;
}

void CamarasDialog::mostrarBotonesAceptacion(bool valor)
{
    ui->aceptarBoton->setVisible(valor);
    ui->cancelarBoton->setVisible(valor);
}

void CamarasDialog::mostrarBotonesEdicion(bool valor)
{
    ui->crearBoton->setVisible(valor);
    ui->editarBoton->setVisible(valor);
    ui->eliminarBoton->setVisible(valor);
    ui->exportarBoton->setVisible(valor);

    if (camaras.size() == 0) {
        ui->eliminarBoton->setEnabled(false);
        ui->exportarBoton->setEnabled(false);
    } else {
        ui->eliminarBoton->setEnabled(true);
        ui->exportarBoton->setEnabled(true);
    }
    int indice = getIndiceCamaraSeleccionada();
    if (indice == -1) {
        ui->editarBoton->setEnabled(false);
        ui->eliminarBoton->setEnabled(false);
    } else {
        ui->editarBoton->setEnabled(true);
        ui->eliminarBoton->setEnabled(true);
    }
}

void CamarasDialog::exportar()
{
    if (camaras.size() <= 0) {
        return;
    }

    QString nombreFichero = QFileDialog::getSaveFileName(this,
                                       Idioma::getInstancia()->getTexto("camara.guardar.titulo"), ".",
                                       tr("CAM files (*.cam)"));

    if (nombreFichero == "") {
        return;
    }

    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QFile file(nombreFichero);
    if ( file.open(QIODevice::ReadWrite) )
    {
        try {
            QTextStream stream( &file );
            for (int i = 0; i < camaras.size(); i++) {
                QString propiedades = camaras[i]->getPropiedadesEnTexto();
                QString encriptado = crypto.encryptToString(propiedades);
                stream << encriptado << endl;
            }            
        } catch (...) {
            //mostrar error
            return;
        }
    }
    file.close();
}

