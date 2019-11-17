#include "configuracionvisordialog.h"
#include "ui_configuracionvisordialog.h"

ConfiguracionVisorDialog::ConfiguracionVisorDialog(bool verNombres, bool verAlturas, bool distanciasReales, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfiguracionVisorDialog)
{
    ui->setupUi(this);

    this->verNombres = verNombres;
    this->verAlturas = verAlturas;
    this->distanciasReales = distanciasReales;

    ui->alturaWayPointCheckBox->setChecked(verAlturas);
    ui->nombresLineascheckBox->setChecked(verNombres);
    ui->distanciasCheckBox->setChecked(distanciasReales);

    QPushButton* btn = ui->botonera->button(QDialogButtonBox::Ok);    
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(aceptar()));
    connect(ui->establecerAlturascheckBox, SIGNAL(stateChanged(int)), this, SLOT(checkAlturasPulsado()));

    setWindowTitle(Idioma::getInstancia()->getTexto("visoralturas.configuracion.titulo"));
    ui->alturaWayPointCheckBox->setText(Idioma::getInstancia()->getTexto("visoralturas.configuracion.veraltura"));
    ui->distanciasCheckBox->setText(Idioma::getInstancia()->getTexto("visoralturas.configuracion.establecerdistanciareal"));
    ui->establecerAlturascheckBox->setText(Idioma::getInstancia()->getTexto("visoralturas.configuracion.estableceralturas"));
    ui->nombresLineascheckBox->setText(Idioma::getInstancia()->getTexto("visoralturas.configuracion.nombreslineas"));
    ui->labelAl->setText(Idioma::getInstancia()->getTexto("visoralturas.configuracion.al"));
    ui->metrosLabe->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->rangoCheckBox->setText(Idioma::getInstancia()->getTexto("visoralturas.configuracion.desde"));
    ui->opcionesGroupBox->setTitle(Idioma::getInstancia()->getTexto("visoralturas.configuracion.grupo.opciones"));
    ui->alturasGroupBox->setTitle(Idioma::getInstancia()->getTexto("visoralturas.configuracion.grupo.altura"));
}

ConfiguracionVisorDialog::~ConfiguracionVisorDialog()
{
    delete ui;
}

void ConfiguracionVisorDialog::checkAlturasPulsado()
{
    bool valor = ui->establecerAlturascheckBox->isChecked();
    ui->rangoCheckBox->setEnabled(valor);
    ui->wpFinEdit->setEnabled(valor);
    ui->wpInicioEfit->setEnabled(valor);
    ui->labelAl->setEnabled(valor);
}

void ConfiguracionVisorDialog::aceptar()
{
    establecerAlturas = ui->establecerAlturascheckBox->isChecked();
    if (establecerAlturas) {
        altura = ui->alturaEdit->text().toInt();
    }
    verAlturas = ui->alturaWayPointCheckBox->isChecked();
    verNombres = ui->nombresLineascheckBox->isChecked();
    distanciasReales = ui->distanciasCheckBox->isChecked();
    rangoEstablecido = ui->rangoCheckBox->isChecked();
    inicioRango = ui->wpInicioEfit->text().toInt();
    finRango = ui->wpFinEdit->text().toInt();
    aceptado = true;
    hide();
}

