#ifndef ACERCADEDIALOG_H
#define ACERCADEDIALOG_H

#include <QDialog>
#include "idioma.h"

namespace Ui {
class AcercaDeDialog;
}

class AcercaDeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AcercaDeDialog(QString v, QString fecha, QWidget *parent = 0);
    ~AcercaDeDialog();

    QString version;
    QString fecha;

private:
    Ui::AcercaDeDialog *ui;
};

#endif // ACERCADEDIALOG_H
