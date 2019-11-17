#include "configuraciondialog.h"
#include "ui_configuraciondialog.h"

ConfiguracionDialog::ConfiguracionDialog(Enumerados::IdiomasAplicacion idioma, QString ruta, bool aterrizajeCirculo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfiguracionDialog)
{
    ui->setupUi(this);
    idiomaSeleccionado = idioma;
    rutaHorizon = ruta;
    this->aterrizajeCirculo = aterrizajeCirculo;

    if (idiomaSeleccionado == Enumerados::Castellano) {
        ui->spainBoton->setChecked(true);
        ui->ukBoton->setChecked(false);
    } else if (idiomaSeleccionado == Enumerados::English) {
        ui->spainBoton->setChecked(false);
        ui->ukBoton->setChecked(true);
    }

    ui->aterrizajeCheckBox->setChecked(aterrizajeCirculo);
    ui->rutaHorizonText->setText(ruta);

    traducir();

    connect(ui->botonera->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(aceptar()));
    connect(ui->spainBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarCastellano()));
    connect(ui->ukBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarIngles()));
    connect(ui->rutaHorizonBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarCarpetaHorizon()));


#ifdef MODO_PORTATIL

    ui->ukBoton->setMinimumHeight(48);
    ui->spainBoton->setMinimumHeight(48);
    ui->rutaHorizonBoton->setMinimumHeight(48);

#endif
}

ConfiguracionDialog::~ConfiguracionDialog()
{
    delete ui;
}

void ConfiguracionDialog::seleccionarCarpetaHorizon()
{
    QString rutaCarpeta = QFileDialog::getExistingDirectory(this, Idioma::getInstancia()->getTexto("general.seleccionar"),
                                                rutaHorizon,
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if (rutaCarpeta == "") {
        return;
    }
    ui->rutaHorizonText->setText(rutaCarpeta);
}


void ConfiguracionDialog::traducir()
{
    setWindowTitle(Idioma::getInstancia()->getTexto("opciones.titulo"));
    ui->idiomaLabel->setText(Idioma::getInstancia()->getTexto("opciones.idioma"));
    ui->spainBoton->setToolTip(Idioma::getInstancia()->getTexto("opciones.boton.spain"));
    ui->ukBoton->setToolTip(Idioma::getInstancia()->getTexto("opciones.boton.uk"));
    ui->rutaHorizonLabel->setText(Idioma::getInstancia()->getTexto("opciones.rutaCarpeta"));
    ui->rutaHorizonBoton->setText(Idioma::getInstancia()->getTexto("general.seleccionar"));
    ui->opcionesGroupBox->setTitle(Idioma::getInstancia()->getTexto("opciones.grupo.opciones"));
    ui->vueloGroupBox->setTitle(Idioma::getInstancia()->getTexto("opciones.grupo.vuelo"));
    ui->aterrizajeCheckBox->setText(Idioma::getInstancia()->getTexto("opciones.grupo.vuelo.aterrizaje"));

}

void ConfiguracionDialog::seleccionarCastellano()
{
    ui->spainBoton->setChecked(true);
    ui->ukBoton->setChecked(false);
}

void ConfiguracionDialog::seleccionarIngles()
{
    ui->spainBoton->setChecked(false);
    ui->ukBoton->setChecked(true);
}

void ConfiguracionDialog::aceptar()
{
    aceptado = true;

    if (ui->spainBoton->isChecked()) {
        idiomaSeleccionado = Enumerados::Castellano;
    } else if (ui->ukBoton->isChecked()) {
        idiomaSeleccionado = Enumerados::English;
    }
    rutaHorizon = ui->rutaHorizonText->text();
    aterrizajeCirculo = ui->aterrizajeCheckBox->isChecked();
    hide();
}
