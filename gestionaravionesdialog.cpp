#include "gestionaravionesdialog.h"
#include "ui_gestionaravionesdialog.h"

GestionarAvionesDialog::GestionarAvionesDialog(QString rutaHorizon, QList<Avion*> listaAviones, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GestionarAvionesDialog)
{
    ui->setupUi(this);    
    this->rutaHorizon = rutaHorizon;

    for (int i = 0; i < listaAviones.size(); i++) {
        addAvion(listaAviones[i]);
    }

    activarCampos(false);
    mostrarBotonesEdicion(true);
    mostrarBotonesAceptacion(false);
    crearValidadores();

    connect(ui->crearBoton, SIGNAL(clicked(bool)), this, SLOT(crearAvion()));
    connect(ui->editarBoton, SIGNAL(clicked(bool)), this, SLOT(modificarAvion()));
    connect(ui->eliminarBoton, SIGNAL(clicked(bool)), this, SLOT(eliminarAvion()));
    connect(ui->aceptarBoton, SIGNAL(clicked(bool)), this, SLOT(guardarAvion()));
    connect(ui->cancelarBoton, SIGNAL(clicked(bool)), this, SLOT(cancelarEdicion()));
    connect(ui->exportarBoton, SIGNAL(clicked(bool)), this, SLOT(exportar()));    
    connect(ui->botonera->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(aceptar()));
    connect(ui->headerBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarHeader()));
    connect(ui->footerBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarFooter()));

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

GestionarAvionesDialog::~GestionarAvionesDialog()
{
    delete ui;
    for (int i = 0; i < listaRadioAviones.size(); i++) {
        delete listaRadioAviones[i];
    }
}

void GestionarAvionesDialog::traducir()
{
    setWindowTitle(Idioma::getInstancia()->getTexto("aviones.titulo"));
    ui->avionesDefinidosLabel->setText(Idioma::getInstancia()->getTexto("aviones.lista.definidas"));
    ui->informacionGrupo->setTitle(Idioma::getInstancia()->getTexto("aviones.informacion.titulo"));
    ui->empresaLabel->setText(Idioma::getInstancia()->getTexto("aviones.informacion.empresa"));
    ui->modeloLabel->setText(Idioma::getInstancia()->getTexto("aviones.informacion.modelo"));
    ui->versionLabel->setText(Idioma::getInstancia()->getTexto("aviones.informacion.version"));
    ui->parametrosGrupo->setTitle(Idioma::getInstancia()->getTexto("aviones.parametros.titulo"));
    ui->maxPitchLabel->setText(Idioma::getInstancia()->getTexto("aviones.parametros.maxpitch"));
    ui->minPitchLabel->setText(Idioma::getInstancia()->getTexto("aviones.parametros.minpitch"));
    ui->grados1Label->setText(Idioma::getInstancia()->getTexto("general.grados"));
    ui->grados2Label->setText(Idioma::getInstancia()->getTexto("general.grados"));
    ui->velocidadLabel->setText(Idioma::getInstancia()->getTexto("aviones.parametros.velocidadcrucero"));
    ui->recorridoLabel->setText(Idioma::getInstancia()->getTexto("aviones.parametros.recorrido"));
    ui->baseLabel->setText(Idioma::getInstancia()->getTexto("aviones.parametros.base"));
    ui->msLabel->setText(Idioma::getInstancia()->getTexto("general.metrossegundos"));
    ui->metros1Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->metros2Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->footerFicheroLabel->setText(Idioma::getInstancia()->getTexto("aviones.parametros.footer"));
    ui->headerFicheroLabel->setText(Idioma::getInstancia()->getTexto("aviones.parametros.header"));
    ui->footerBoton->setText(Idioma::getInstancia()->getTexto("general.seleccionar"));
    ui->headerBoton->setText(Idioma::getInstancia()->getTexto("general.seleccionar"));
    ui->aceptarBoton->setText(Idioma::getInstancia()->getTexto("general.aceptar"));
    ui->cancelarBoton->setText(Idioma::getInstancia()->getTexto("general.cancelar"));
    ui->exportarBoton->setText(Idioma::getInstancia()->getTexto("general.exportar"));
    ui->editarBoton->setText(Idioma::getInstancia()->getTexto("general.editar"));
    ui->crearBoton->setText(Idioma::getInstancia()->getTexto("general.crear"));
    ui->eliminarBoton->setText(Idioma::getInstancia()->getTexto("general.eliminar"));
}

void GestionarAvionesDialog::seleccionarHeader()
{
    QString fichero = QFileDialog::getOpenFileName(this,
                                       Idioma::getInstancia()->getTexto("aviones.header.titulo"), rutaHorizon + "/fly/headers",
                                       tr("Fly files (*.fly)"));
    if (fichero == "") {
        return;
    }
    ui->headerPathText->setText(fichero);
}

void GestionarAvionesDialog::seleccionarFooter()
{
    QString fichero = QFileDialog::getOpenFileName(this,
                                       Idioma::getInstancia()->getTexto("aviones.footer.titulo"), rutaHorizon + "/fly/footers",
                                       tr("Fly files (*.fly)"));

    if (fichero == "") {
        return;
    }

    ui->footerPathText->setText(fichero);
}

void GestionarAvionesDialog::seleccionarImagen()
{
    QString fichero = QFileDialog::getOpenFileName(this,
                                       Idioma::getInstancia()->getTexto("aviones.seleccionarimagen"), ".",
                                       tr("Image files (*.jpg)"));
    if (fichero == "") {
        return;
    }
}

void GestionarAvionesDialog::aceptar()
{
    if (listaAviones.size() == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(Idioma::getInstancia()->getTexto("aviones.error.titulo"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(Idioma::getInstancia()->getTexto("aviones.error.mensaje"));
        msgBox.setStandardButtons(QMessageBox::Close);
        msgBox.setDefaultButton(QMessageBox::Close);
        msgBox.exec();
        return;
    }
    aceptado = true;
    hide();
}

void GestionarAvionesDialog::addAvion(Avion* avion)
{
    QRadioButton* radio = new QRadioButton(this);
    radio->setText(avion->modelo);
    listaRadioAviones.push_back(radio);
    listaAviones.push_back(avion);
    addRadio(radio);
}

void GestionarAvionesDialog::addRadio(QRadioButton* radio)
{
    ui->layoutAvionesDefinidos->addWidget(radio);
    connect(radio, SIGNAL(clicked(bool)), this, SLOT(cargarDatosAvion()));
}

void GestionarAvionesDialog::crearValidadores()
{
    ui->maxPitchtextEdit->setValidator(new QIntValidator(1, 100000, this));
    ui->minPitchTextEdit->setValidator(new QIntValidator(1, 100000, this));
    ui->velocidadTextEdit->setValidator(new DoubleValidator(this));
    ui->baseTextEdit->setValidator(new DoubleValidator(this));
    ui->recorridoTextEdit->setValidator(new DoubleValidator(this));
}

int GestionarAvionesDialog::getIndiceAvionSeleccionado()
{
    for (int i = 0; i < listaRadioAviones.size(); i++) {
        if (listaRadioAviones[i]->isChecked()) {
            return i;
        }
    }
    return -1;
}

Avion* GestionarAvionesDialog::getAvionSeleccionado()
{
    for (int i = 0; i < listaRadioAviones.size(); i++) {
        if (listaRadioAviones[i]->isChecked()) {
            return listaAviones[i];
        }
    }
    return Q_NULLPTR;
}

void GestionarAvionesDialog::rellenarFormulario(Avion* avion)
{
    if (avion != Q_NULLPTR) {
        ui->modeloTextEdit->setText(avion->modelo);
        ui->empresaTextEdit->setText(avion->empresa);
        ui->versionTextEdit->setText(avion->version);
        ui->maxPitchtextEdit->setText(Formateador::formatearEntero(avion->pitchMaximo));
        ui->minPitchTextEdit->setText(Formateador::formatearEntero(avion->pitchMinimo));
        ui->velocidadTextEdit->setText(Formateador::formatearEntero(avion->velocidadCrucero));
        ui->baseTextEdit->setText(Formateador::formatearDecimal(avion->baseCircuit, 2));
        ui->recorridoTextEdit->setText(Formateador::formatearDecimal(avion->recorridoCircuit, 2));
        ui->footerPathText->setText(avion->pathFooter);
        ui->headerPathText->setText(avion->pathHeader);
    } else {
        ui->modeloTextEdit->setText("");
        ui->empresaTextEdit->setText("");
        ui->versionTextEdit->setText("");
        ui->maxPitchtextEdit->setText("");
        ui->minPitchTextEdit->setText("");
        ui->velocidadTextEdit->setText("");
        ui->baseTextEdit->setText("");
        ui->recorridoTextEdit->setText("");
        ui->footerPathText->setText("");
        ui->headerPathText->setText("");
    }
}

void GestionarAvionesDialog::activarCampos(bool valor)
{
    ui->modeloTextEdit->setEnabled(valor);
    ui->empresaTextEdit->setEnabled(valor);
    ui->versionTextEdit->setEnabled(valor);
    ui->maxPitchtextEdit->setEnabled(valor);
    ui->minPitchTextEdit->setEnabled(valor);
    ui->velocidadTextEdit->setEnabled(valor);
    ui->baseTextEdit->setEnabled(valor);
    ui->recorridoTextEdit->setEnabled(valor);
}

void GestionarAvionesDialog::mostrarBotonesAceptacion(bool valor)
{
    ui->aceptarBoton->setVisible(valor);
    ui->cancelarBoton->setVisible(valor);
    ui->headerBoton->setVisible(valor);
    ui->footerBoton->setVisible(valor);
}

void GestionarAvionesDialog::mostrarBotonesEdicion(bool valor)
{
    ui->crearBoton->setVisible(valor);
    ui->editarBoton->setVisible(valor);
    ui->eliminarBoton->setVisible(valor);
    ui->exportarBoton->setVisible(valor);

    if (listaAviones.size() == 0) {
        ui->eliminarBoton->setEnabled(false);
        ui->exportarBoton->setEnabled(false);
    } else {
        ui->eliminarBoton->setEnabled(true);
        ui->exportarBoton->setEnabled(true);
    }
    int indice = getIndiceAvionSeleccionado();
    if (indice == -1) {
        ui->editarBoton->setEnabled(false);
        ui->eliminarBoton->setEnabled(false);
    } else {
        ui->editarBoton->setEnabled(true);
        ui->eliminarBoton->setEnabled(true);
    }
}

void GestionarAvionesDialog::cargarDatosAvion()
{
    rellenarFormulario(getAvionSeleccionado());
    ui->eliminarBoton->setEnabled(true);
    ui->editarBoton->setEnabled(true);
}

void GestionarAvionesDialog::crearAvion()
{
    mostrarBotonesAceptacion(true);
    mostrarBotonesEdicion(false);
    rellenarFormulario(Q_NULLPTR);
    activarCampos(true);
    esModificacion = false;
}


void GestionarAvionesDialog::cancelarEdicion()
{
    mostrarBotonesAceptacion(false);
    mostrarBotonesEdicion(true);
    cargarDatosAvion();
    activarCampos(false);
    esModificacion = false;
}

void GestionarAvionesDialog::guardarDatosAvion(Avion* avion)
{

    avion->modelo = ui->modeloTextEdit->text();
    avion->empresa = ui->empresaTextEdit->text();
    avion->version = ui->versionTextEdit->text();
    avion->pitchMaximo = Formateador::formatearDecimal(ui->maxPitchtextEdit->text(), 2);
    avion->pitchMinimo = Formateador::formatearDecimal(ui->minPitchTextEdit->text(), 2);
    avion->velocidadCrucero =Formateador::formatearDecimal( ui->velocidadTextEdit->text(), 2);
    avion->baseCircuit = Formateador::formatearDecimal(ui->baseTextEdit->text(), 2);
    avion->recorridoCircuit = Formateador::formatearDecimal(ui->recorridoTextEdit->text(), 2);
    avion->pathHeader = ui->headerPathText->text();
    avion->pathFooter = ui->footerPathText->text();
}

void GestionarAvionesDialog::guardarAvion()
{

    if (ui->modeloTextEdit->text().isEmpty() || ui->empresaTextEdit->text().isEmpty()) {
        QMessageBox msg;
        msg.setWindowTitle(Idioma::getInstancia()->getTexto("aviones.error.titulo"));
        msg.setText(Idioma::getInstancia()->getTexto("aviones.error.validation.mensaje"));
        msg.exec();
        return;
    }

    if (esModificacion) {
        Avion* avion = getAvionSeleccionado();
        guardarDatosAvion(avion);
    } else {
        Avion* avion = new Avion();
        guardarDatosAvion(avion);
        addAvion(avion);
        listaRadioAviones[listaRadioAviones.size()-1]->setChecked(true);
    }
    cargarDatosAvion();
    activarCampos(false);
    esModificacion = false;

    mostrarBotonesAceptacion(false);
    mostrarBotonesEdicion(true);
}

void GestionarAvionesDialog::quitarAvion(int indice)
{
    if (indice == -1 || indice >= listaAviones.size()) {
        return;
    }
    listaAviones.removeAt(indice);
    ui->layoutAvionesDefinidos->removeWidget(listaRadioAviones[indice]);
    delete listaRadioAviones[indice];
    listaRadioAviones.removeAt(indice);
    update();
}

void GestionarAvionesDialog::eliminarAvion()
{
    QMessageBox msg;
    msg.setWindowTitle(Idioma::getInstancia()->getTexto("aviones.eliminar.titulo"));
    msg.setText(Idioma::getInstancia()->getTexto("aviones.eliminar.mensaje"));
    msg.setInformativeText(Idioma::getInstancia()->getTexto("aviones.eliminar.informacion"));
    msg.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msg.setDefaultButton(QMessageBox::Cancel);

    int opcion = msg.exec();
    if (opcion == QMessageBox::Cancel) {
        return;
    }
    int indice = getIndiceAvionSeleccionado();
    if (indice == -1) {
        return;
    }
    quitarAvion(indice);
    mostrarBotonesAceptacion(false);
    mostrarBotonesEdicion(true);
    rellenarFormulario(Q_NULLPTR);
    activarCampos(false);
    esModificacion = false;
}

void GestionarAvionesDialog::modificarAvion()
{
    mostrarBotonesAceptacion(true);
    mostrarBotonesEdicion(false);
    activarCampos(true);
    esModificacion = true;
}

void GestionarAvionesDialog::exportar()
{
    if (listaAviones.size() <= 0) {
        return;
    }

    QString nombreFichero = QFileDialog::getSaveFileName(this,
                                       Idioma::getInstancia()->getTexto("aviones.exportar.titulo"), ".",
                                       tr("CFG files (*.cfg)"));

    if (nombreFichero == "") {
        return;
    }

    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QFile file(nombreFichero);
    if ( file.open(QIODevice::ReadWrite) )
    {
        try {
            QTextStream stream( &file );
            for (int i = 0; i < listaAviones.size(); i++) {
                QString propiedades = listaAviones[i]->getPropiedades();
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
