#include "importarflydialog.h"
#include "ui_importarflydialog.h"

ImportarFlyDialog::ImportarFlyDialog(QString rutaHorizon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportarFlyDialog)
{
    ui->setupUi(this);

    this->rutaHorizon = rutaHorizon;
    connect(ui->MapaPushButton, SIGNAL(clicked(bool)), this, SLOT(seleccionarMapaHorizon()));
    connect(ui->flyPushButton, SIGNAL(clicked(bool)), this, SLOT(seleccionarFicheroFly()));
    connect(ui->okPushButton, SIGNAL(clicked(bool)), this, SLOT(aceptar()));
    connect(ui->cancelPushButton, SIGNAL(clicked(bool)), this, SLOT(cancelar()));

    ui->okPushButton->setEnabled(false);
    traducir();


#ifdef MODO_PORTATIL
    ui->MapaPushButton->setMinimumHeight(32);
    ui->flyPushButton->setMinimumHeight(32);
    ui->okPushButton->setMinimumHeight(48);
    ui->cancelPushButton->setMinimumHeight(48);

#endif
}

ImportarFlyDialog::~ImportarFlyDialog()
{
    delete ui;
}

void ImportarFlyDialog::traducir()
{
    setWindowTitle(Idioma::getInstancia()->getTexto("ventana.importar.titulo"));
    ui->flyLabel->setText(Idioma::getInstancia()->getTexto("ventana.importar.fly"));
    ui->mapaLabel->setText(Idioma::getInstancia()->getTexto("ventana.importar.mapa"));
    ui->flyPushButton->setText(Idioma::getInstancia()->getTexto("general.seleccionar"));
    ui->MapaPushButton->setText(Idioma::getInstancia()->getTexto("general.seleccionar"));
    ui->okPushButton->setText(Idioma::getInstancia()->getTexto("general.aceptar"));
    ui->cancelPushButton->setText(Idioma::getInstancia()->getTexto("general.cancelar"));
}

void ImportarFlyDialog::seleccionarFicheroFly()
{
    QString fichero = QFileDialog::getOpenFileName(this,
                                           Idioma::getInstancia()->getTexto("mapas.abrir.titulo"), rutaHorizon + "\\fly",
                                           tr("FLY files (*.fly)"));
    if (fichero == "") {
        return;
    }
    ui->flyLineEdit->setText(fichero);

    if (!ui->mapaLineEdit->text().isEmpty()) {
        ui->okPushButton->setEnabled(true);
    }
}

void ImportarFlyDialog::aceptar()
{
    aceptado = true;
    rutaFly = ui->flyLineEdit->text();
    rutaMapa = ui->MapaPushButton->text();
    hide();
}

void ImportarFlyDialog::cancelar()
{
    hide();
}

void ImportarFlyDialog::seleccionarMapaHorizon()
{
    MapasHorizonDialog dialogo(rutaHorizon + "/maps");
    //dialogo.rutaHorizon = rutaHorizon + "/maps";
    dialogo.setModal(true);
    dialogo.exec();

    if (dialogo.aceptado) {
        if (dialogo.mapasSeleccionados.size() > 0) {
            ui->mapaLineEdit->setText(dialogo.mapasSeleccionados[0]->pathFichero);
            imagenMapa = dialogo.mapasSeleccionados[0];
        }
    }
    if (!ui->flyLineEdit->text().isEmpty()) {
        ui->okPushButton->setEnabled(true);
    }
}
