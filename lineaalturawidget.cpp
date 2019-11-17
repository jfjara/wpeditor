#include "lineaalturawidget.h"
#include "ui_lineaalturawidget.h"

LineaAlturaWidget::LineaAlturaWidget(LineaAltura* linea, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineaAlturaWidget)
{
    ui->setupUi(this);
    this->linea = linea;

    recargar();

    connect(ui->editarLineaBoton, SIGNAL(clicked(bool)), this, SLOT(editarLinea()));
    connect(ui->eliminarBoton, SIGNAL(clicked(bool)), this, SLOT(eliminarLinea()));

    ui->nombreLabel->setText(Idioma::getInstancia()->getTexto("visoralturas.lineasaltura.nombre"));
    ui->alturaLabel->setText(Idioma::getInstancia()->getTexto("visoralturas.lineasaltura.altura"));
    ui->editarLineaBoton->setToolTip(Idioma::getInstancia()->getTexto("general.editar"));
    ui->eliminarBoton->setToolTip(Idioma::getInstancia()->getTexto("general.eliminar"));
    ui->metrosLabel->setText(Idioma::getInstancia()->getTexto("general.metros"));

#ifdef MODO_PORTATIL

    ui->editarLineaBoton->setMinimumHeight(48);
    ui->editarLineaBoton->setMinimumWidth(48);
    ui->eliminarBoton->setMinimumHeight(48);
    ui->eliminarBoton->setMinimumWidth(48);
#endif
}

LineaAlturaWidget::~LineaAlturaWidget()
{
    delete ui;
}

void LineaAlturaWidget::editarLinea()
{
    emit editar(linea);
}

void LineaAlturaWidget::eliminarLinea()
{
    emit eliminar(linea);
    hide();
}

void LineaAlturaWidget::recargar()
{
    ui->nombreEdit->setText(linea->nombre);
    ui->alturaEdit->setText(QString::number(linea->altura));
    ui->colorLabel->setStyleSheet("QLabel { background-color : " + linea->color.name() + "; }");
}
