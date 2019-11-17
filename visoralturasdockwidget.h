#ifndef VISORALTURASDOCKWIDGET_H
#define VISORALTURASDOCKWIDGET_H

#include <QDockWidget>
#include "visoralturaswidget.h"
#include "idioma.h"

namespace Ui {
class VisorAlturasDockWidget;
}

class VisorAlturasDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit VisorAlturasDockWidget(VisorAlturasWidget* visorAlturas, QWidget *parent = 0);
    ~VisorAlturasDockWidget();

    VisorAlturasWidget* visorAlturas = Q_NULLPTR;

    void traducir();

private:
    Ui::VisorAlturasDockWidget *ui;
};

#endif // VISORALTURASDOCKWIDGET_H
