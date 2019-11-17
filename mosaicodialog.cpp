#include "mosaicodialog.h"
#include "ui_mosaicodialog.h"

MosaicoDialog::MosaicoDialog(QList<Camara*> camaras, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MosaicoDialog)
{
    this->camaras = camaras;
    ui->setupUi(this);

    QStringList listaCamarasCombo;
    for (int i = 0; i < camaras.size(); i++) {
        QString nombreCamara = camaras[i]->getNombreCamara();
        listaCamarasCombo.push_back(nombreCamara);
    }
    ui->camarasCombo->addItems(listaCamarasCombo);

    connect(ui->seleccionarCarpetaBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarCarpetaImagenes()));
    connect(ui->seleccionarFicheroMatchesBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarFicheroCoordenadas()));
    connect(ui->okBoton, SIGNAL(clicked(bool)), this, SLOT(aceptar()));
    connect(ui->cancelarBoton, SIGNAL(clicked(bool)), this, SLOT(cancelar()));
    connect(ui->camarasCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(seleccionarCamara(int)));

    //validar();
    traducir();

    ui->carpetaEditText->setText("c:\\proyectos\\fotos\\utrera\\100MSDCF");
    ui->ficheroEditText->setText("c:\\proyectos\\fotos\\utrera\\MATCH_FILE_26-04-2017_13-05-17.txt");
}

MosaicoDialog::~MosaicoDialog()
{
    delete ui;
}

void MosaicoDialog::traducir()
{
    this->setWindowTitle(Idioma::getInstancia()->getTexto("mosaico.ventana.proceso.titulo"));
    ui->camaraLabel->setText(Idioma::getInstancia()->getTexto("mosaico.camara"));
    ui->carpetalabel->setText(Idioma::getInstancia()->getTexto("mosaico.ruta.fotos"));
    ui->matchesLabel->setText(Idioma::getInstancia()->getTexto("mosaico.ruta.matches"));
    ui->okBoton->setText(Idioma::getInstancia()->getTexto("general.aceptar"));
    ui->cancelarBoton->setText(Idioma::getInstancia()->getTexto("general.cancelar"));
    ui->seleccionarCarpetaBoton->setText(Idioma::getInstancia()->getTexto("general.seleccionar"));
    ui->seleccionarFicheroMatchesBoton->setText(Idioma::getInstancia()->getTexto("general.seleccionar"));
}

void MosaicoDialog::cancelar()
{
    aceptado = false;
    hide();
}

void MosaicoDialog::aceptar()
{
    pathCarpeta = ui->carpetaEditText->text();
    pathFichero = ui->ficheroEditText->text();
    if (pathCarpeta.trimmed().isEmpty()) {
        return;
    }
    if (pathFichero.trimmed().isEmpty()) {
        return;
    }
    aceptado = true;

    hide();
}

void MosaicoDialog::seleccionarCarpetaImagenes()
{
    QString rutaCarpeta = QFileDialog::getExistingDirectory(this, Idioma::getInstancia()->getTexto("mosaico.ventana.seleccionarcarpeta"),
                                                "\\home",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    if (rutaCarpeta == "") {
        return;
    }
    ui->carpetaEditText->setText(rutaCarpeta);
    validar();
}

void MosaicoDialog::seleccionarFicheroCoordenadas()
{
    QString fichero = QFileDialog::getOpenFileName(this,
                                           Idioma::getInstancia()->getTexto("mosaico.ventana.seleccionarfichero"), "\\home",
                                           tr("TXT files (*.txt)"));
    if (fichero == "") {
        return;
    }
    ui->ficheroEditText->setText(fichero);
    validar();
}

void MosaicoDialog::validar()
{
    bool activarBotones = true;
    pathCarpeta = ui->carpetaEditText->text();
    pathFichero = ui->ficheroEditText->text();
    if (pathCarpeta.trimmed().isEmpty()) {
        activarBotones = false;
    }
    if (pathFichero.trimmed().isEmpty()) {
        activarBotones = false;
    }

    if (!activarBotones) {
        ui->okBoton->setEnabled(false);
    } else {
        ui->okBoton->setEnabled(true);
    }
}

void MosaicoDialog::seleccionarCamara(int indice)
{
    indiceCamara = indice;
}
