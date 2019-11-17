#ifndef MOSAICODOCKWIDGET_H
#define MOSAICODOCKWIDGET_H

#include <QDockWidget>
#include "gestortiles.h"

namespace Ui {
class MosaicoDockWidget;
}

class MosaicoDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit MosaicoDockWidget(GestorTiles* gestorTiles, QWidget *parent = 0);
    ~MosaicoDockWidget();

    void closeEvent(QCloseEvent * event) override;

public slots:
    void marcarImagen(int);
    void eliminarImagen();
    void exportarImagen();

private:
    Ui::MosaicoDockWidget *ui;
    GestorTiles* gestorTiles = Q_NULLPTR;
};

#endif // MOSAICODOCKWIDGET_H
