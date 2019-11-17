#include "visoralturasdockwidget.h"
#include "ui_visoralturasdockwidget.h"

VisorAlturasDockWidget::VisorAlturasDockWidget(VisorAlturasWidget* visorAlturas, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::VisorAlturasDockWidget)
{
    ui->setupUi(this);
    this->visorAlturas = visorAlturas;

    ui->horizontalLayout->addWidget(visorAlturas);

    traducir();
}

void VisorAlturasDockWidget::traducir()
{
    this->setWindowTitle(Idioma::getInstancia()->getTexto("visoralturas.titulo"));    
}

VisorAlturasDockWidget::~VisorAlturasDockWidget()
{
    delete ui;
}


