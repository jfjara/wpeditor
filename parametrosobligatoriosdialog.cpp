#include "parametrosobligatoriosdialog.h"
#include "ui_parametrosobligatoriosdialog.h"

ParametrosObligatoriosDialog::ParametrosObligatoriosDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametrosObligatoriosDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(Qt::WindowTitleHint);

    connect(ui->aceptarBoton, SIGNAL(clicked(bool)), this, SLOT(aceptar()));

    crearValidadores();
    traducir();
}

ParametrosObligatoriosDialog::~ParametrosObligatoriosDialog()
{
    delete ui;
}

void ParametrosObligatoriosDialog::traducir()
{
    ui->aceptarBoton->setText(Idioma::getInstancia()->getTexto("general.aceptar"));
    ui->metros1Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->metros2Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->metros3Label->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->nombreMisionLabel->setText(Idioma::getInstancia()->getTexto("principal.mision.nombre"));
    ui->alturaMaximaVueloLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.alturavuelo"));
    ui->alturaVisorLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.alturavisor"));
    ui->alturaVueloLabel->setText(Idioma::getInstancia()->getTexto("principal.configuracionvuelo.alturawaypoints"));
    ui->aterrizajeEnCirculocheckBox->setText(Idioma::getInstancia()->getTexto("opciones.grupo.vuelo.aterrizaje"));
    this->setWindowTitle(Idioma::getInstancia()->getTexto("proyecto.nuevo.ventana.titulo"));
    ui->parametrosGroupBox->setTitle(Idioma::getInstancia()->getTexto("proyecto.nuevo.parametros"));
}

void ParametrosObligatoriosDialog::crearValidadores()
{
    ui->alturaVisorText->setValidator(new QIntValidator(100, 300, this));
    ui->alturaMaximaVueloText->setValidator(new QIntValidator(10, 300, this));
    ui->alturaVueloText->setValidator(new QIntValidator(10, 300, this));
}

void ParametrosObligatoriosDialog::aceptar()
{
    if (!validar()) {
        QMessageBox::critical(this, Idioma::getInstancia()->getTexto("proyecto.nuevo.ventana.error.titulo"), Idioma::getInstancia()->getTexto("proyecto.nuevo.ventana.error.descripcion"));
        return;
    }
    alturaVisor = ui->alturaVisorText->text().toInt();
    alturaMaximaVuelo = ui->alturaMaximaVueloText->text().toInt();
    alturaPredefinida = ui->alturaVueloText->text().toInt();
    nombre = ui->nombreMisionText->text();
    aterrizajeEnCirculo = ui->aterrizajeEnCirculocheckBox->isChecked();
    hide();
}

bool ParametrosObligatoriosDialog::validar()
{
    if (ui->alturaVisorText->text().isEmpty() || ui->alturaVisorText->text().toInt() <= 100)
        return false;
    if (ui->alturaMaximaVueloText->text().isEmpty() || ui->alturaMaximaVueloText->text().toInt() >= ui->alturaVisorText->text().toInt() || ui->alturaMaximaVueloText->text().toInt() <= 0)
        return false;
    if (ui->alturaVueloText->text().isEmpty() || ui->alturaVueloText->text().toInt() >= ui->alturaVisorText->text().toInt() || ui->alturaVueloText->text().toInt() <= 0)
        return false;
    if (ui->nombreMisionText->text().isEmpty())
        return false;
    return true;
}

