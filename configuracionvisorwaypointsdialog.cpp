#include "configuracionvisorwaypointsdialog.h"
#include "ui_configuracionvisorwaypointsdialog.h"

ConfiguracionVisorWayPointsDialog::ConfiguracionVisorWayPointsDialog(bool verDistanciasEntrePuntos, bool verLineasDireccion, bool distanciaTotalRecorrido, Enumerados::Coordenadas tipoCoordenada, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfiguracionVisorWayPointsDialog)
{
    ui->setupUi(this);
    this->distanciaTotalRecorrido = distanciaTotalRecorrido;
    this->verDistanciasEntrePuntos = verDistanciasEntrePuntos;
    this->verLineasDireccion = verLineasDireccion;
    this->tipoCoordenada = tipoCoordenada;

    ui->verDistanciascheckBox->setChecked(verDistanciasEntrePuntos);
    ui->lineasCheckBox->setChecked(verLineasDireccion);
    ui->distanciaDeCadaTramocheckBox->setChecked(distanciaTotalRecorrido);    

    QPushButton* btn = ui->botonera->button(QDialogButtonBox::Ok);
    connect(btn, SIGNAL(clicked(bool)), this, SLOT(aceptar()));

    setWindowTitle(Idioma::getInstancia()->getTexto("visor.configuracion.titulo"));
    ui->distanciaDeCadaTramocheckBox->setText(Idioma::getInstancia()->getTexto("visor.configuracion.distanciametros"));
    ui->verDistanciascheckBox->setText(Idioma::getInstancia()->getTexto("visor.configuracion.distanciaswaypoints"));
    ui->lineasCheckBox->setText(Idioma::getInstancia()->getTexto("visor.configuracion.lineasdireccion"));
    ui->groupBox->setTitle(Idioma::getInstancia()->getTexto("visor.configuracion.coordenadas"));
    ui->groupBox->setTitle(Idioma::getInstancia()->getTexto("visor.configuracion.grupo"));
}

ConfiguracionVisorWayPointsDialog::~ConfiguracionVisorWayPointsDialog()
{
    delete ui;
}

void ConfiguracionVisorWayPointsDialog::aceptar()
{
    verDistanciasEntrePuntos = ui->verDistanciascheckBox->isChecked();
    verLineasDireccion = ui->lineasCheckBox->isChecked();
    distanciaTotalRecorrido = ui->distanciaDeCadaTramocheckBox->isChecked();   
    aceptado = true;
    hide();
}


