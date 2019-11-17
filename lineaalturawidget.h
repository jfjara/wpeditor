#ifndef LINEAALTURAWIDGET_H
#define LINEAALTURAWIDGET_H

#include <QWidget>
#include "lineaaltura.h"
#include "idioma.h"
#include "constantes.h"

namespace Ui {
class LineaAlturaWidget;
}

class LineaAlturaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LineaAlturaWidget(LineaAltura* linea, QWidget *parent = 0);
    ~LineaAlturaWidget();

    LineaAltura* linea = Q_NULLPTR;

    void recargar();

private:
    Ui::LineaAlturaWidget *ui;

public slots:
    void editarLinea();
    void eliminarLinea();

signals:
    void editar(LineaAltura*);
    void eliminar(LineaAltura*);
};

#endif // LINEAALTURAWIDGET_H
