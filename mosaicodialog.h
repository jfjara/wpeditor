#ifndef MOSAICODIALOG_H
#define MOSAICODIALOG_H

#include <QDialog>
#include <QFileDialog>
#include "idioma.h"
#include "camara.h"

namespace Ui {
class MosaicoDialog;
}

class MosaicoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MosaicoDialog(QList<Camara*> camaras, QWidget *parent = 0);
    ~MosaicoDialog();

    bool aceptado = false;
    QString pathCarpeta;
    QString pathFichero;
    int indiceCamara = 0;

    void traducir();

public slots:
    void aceptar();
    void cancelar();
    void seleccionarCarpetaImagenes();
    void seleccionarFicheroCoordenadas();
    void seleccionarCamara(int indice);

private:
    Ui::MosaicoDialog *ui;

    void validar();

    QList<Camara*> camaras;
};

#endif // MOSAICODIALOG_H
