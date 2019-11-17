#include "ventanavaciadialog.h"
#include "ui_ventanavaciadialog.h"

VentanaVaciaDialog::VentanaVaciaDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VentanaVaciaDialog)
{
    ui->setupUi(this);

    this->setWindowTitle(Idioma::getInstancia()->getTexto("mosaico.ventana.proceso.titulo"));
}

VentanaVaciaDialog::~VentanaVaciaDialog()
{
    delete ui;
}

void VentanaVaciaDialog::cerrar()
{
    hide();
}

void VentanaVaciaDialog::setWidget(BarraProgresoMosaicoForm* widget)
{
    ui->verticalLayout->addWidget(widget);
    form = widget;

    connect(form, SIGNAL(cerrar()), this, SLOT(cerrar()));
}

void VentanaVaciaDialog::closeEvent(QCloseEvent * e)
{
    form->operacionCancelada();
    hide();
}
