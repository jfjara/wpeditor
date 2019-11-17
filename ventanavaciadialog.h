#ifndef VENTANAVACIADIALOG_H
#define VENTANAVACIADIALOG_H

#include <QDialog>
#include "barraprogresomosaicoform.h"
#include "idioma.h"

namespace Ui {
class VentanaVaciaDialog;
}

class VentanaVaciaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VentanaVaciaDialog(QWidget *parent = 0);
    ~VentanaVaciaDialog();

    void setWidget(BarraProgresoMosaicoForm* widget);

    void closeEvent(QCloseEvent * e) override;

public slots:
    void cerrar();

private:
    Ui::VentanaVaciaDialog *ui;
    BarraProgresoMosaicoForm* form;
};

#endif // VENTANAVACIADIALOG_H
