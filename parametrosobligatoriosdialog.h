#ifndef PARAMETROSOBLIGATORIOSDIALOG_H
#define PARAMETROSOBLIGATORIOSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QValidator>
#include "idioma.h"
#include "formateador.h"

namespace Ui {
class ParametrosObligatoriosDialog;
}

class ParametrosObligatoriosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParametrosObligatoriosDialog(QWidget *parent = 0);
    ~ParametrosObligatoriosDialog();

    int alturaVisor;
    int alturaMaximaVuelo;
    int alturaPredefinida;
    QString nombre;
    bool aterrizajeEnCirculo;

    void traducir();

public slots:
    void aceptar();

private:
    Ui::ParametrosObligatoriosDialog *ui;

    bool validar();
    void crearValidadores();
};

#endif // PARAMETROSOBLIGATORIOSDIALOG_H
