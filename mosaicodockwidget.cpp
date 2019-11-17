#include "mosaicodockwidget.h"
#include "ui_mosaicodockwidget.h"
#include <QDebug>

MosaicoDockWidget::MosaicoDockWidget(GestorTiles* gestorTiles, QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::MosaicoDockWidget)
{
    ui->setupUi(this);
    this->gestorTiles = gestorTiles;
    if (gestorTiles == Q_NULLPTR) {
        return;
    }

    for (int i = 0; i < gestorTiles->tiles.size(); i++) {
        QStringList cachos = gestorTiles->tiles[i]->pathFichero.split("\\");
        ui->listWidget->addItem(cachos[cachos.size() - 1]);
    }

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(marcarImagen(int)));
    connect(ui->eliminarBoton, SIGNAL(clicked(bool)), this, SLOT(eliminarImagen()));
    connect(ui->crearImagenBoton, SIGNAL(clicked(bool)), this, SLOT(exportarImagen()));
}

MosaicoDockWidget::~MosaicoDockWidget()
{
    delete ui;
}

void MosaicoDockWidget::marcarImagen(int i)
{
    gestorTiles->marcarImagen(i);
}

void MosaicoDockWidget::eliminarImagen()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    if (items.size() > 0) {
        QListWidgetItem* widgetItem =  ui->listWidget->takeItem(ui->listWidget->row(items[0]));
        gestorTiles->eliminarImagenPorNombre(widgetItem->text());
        delete widgetItem;
    }
}

void MosaicoDockWidget::exportarImagen()
{

}

void MosaicoDockWidget::closeEvent(QCloseEvent * event)
{
    gestorTiles->marcarImagen(-1);
    hide();
}
