#include "acercadedialog.h"
#include "ui_acercadedialog.h"

AcercaDeDialog::AcercaDeDialog(QString v, QString f, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AcercaDeDialog)
{
    ui->setupUi(this);
    this->fecha = f;
    this->version = v;


    setWindowTitle(Idioma::getInstancia()->getTexto("ventana.acercade.titulo"));
    ui->versionLabel->setText("WayPoint Editor V" + version);
    ui->desarrolladoLabel->setText(Idioma::getInstancia()->getTexto("ventana.acercade.desarrollo"));
}

AcercaDeDialog::~AcercaDeDialog()
{
    delete ui;
}
