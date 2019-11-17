#include "barraprogresomosaicoform.h"
#include "ui_barraprogresomosaicoform.h"

BarraProgresoMosaicoForm::BarraProgresoMosaicoForm(GestorTiles* gestor, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BarraProgresoMosaicoForm)
{
    ui->setupUi(this);

    ui->cancelarBoton->setText(Idioma::getInstancia()->getTexto("general.cancelar"));

    gestorTiles = gestor;
    connect(&this->FutureWatcher, SIGNAL (finished()), this, SLOT (operacionFinalizada()));
    connect(ui->cancelarBoton, SIGNAL(clicked(bool)), this, SLOT(operacionCancelada()));
    connect(gestorTiles, SIGNAL(enviarTotalImagenes(int)), this, SLOT(establecerTotalImagenes(int)));
    connect(gestorTiles, SIGNAL(imagenProcesada()), this, SLOT(aumentarBarraProgreso()));

    QFuture<void> future = QtConcurrent::run(gestorTiles, &GestorTiles::leerFicheroReferencias);
    this->FutureWatcher.setFuture(future);
}

BarraProgresoMosaicoForm::~BarraProgresoMosaicoForm()
{
    delete ui;
}

void BarraProgresoMosaicoForm::operacionFinalizada()
{
    hide();
    emit cerrar();
}

void BarraProgresoMosaicoForm::operacionCancelada()
{
    FutureWatcher.cancel();
    gestorTiles->cancelar = true;
    delete gestorTiles;
    emit cerrar();
    hide();
}

void BarraProgresoMosaicoForm::establecerTotalImagenes(int total)
{
    totalImagenesAProcesar = total;
    pasoBarraProgeso =  100.0 / (double)total;
}

void BarraProgresoMosaicoForm::aumentarBarraProgreso()
{
    totalProcesado += pasoBarraProgeso;
    ui->progressBar->setValue((int)totalProcesado);
}


