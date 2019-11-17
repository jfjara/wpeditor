#ifndef LINEASALTURADIALOG_H
#define LINEASALTURADIALOG_H

#include <QDialog>
#include <QList>
#include <QColorDialog>
#include <QSpacerItem>
#include "lineaaltura.h"
#include "lineaalturawidget.h"
#include "datosproyecto.h"
#include "idioma.h"

namespace Ui {
class LineasAlturaDialog;
}

class LineasAlturaDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LineasAlturaDialog(DatosProyecto* proyecto, QWidget *parent = 0);
    ~LineasAlturaDialog();

    QList<LineaAltura*> lineas;
    QList<LineaAlturaWidget*> widgets;
    DatosProyecto* proyecto;

private:
    Ui::LineasAlturaDialog *ui;
    QColor colorProvisional;
    bool modoEdicion = false;
    LineaAltura* lineaEditar;
    QSpacerItem* espaciador;

    void crearWidget(LineaAltura* linea);
    void alinear();

public slots:
    void aceptar();
    void crearLinea();
    void borrarLinea(LineaAltura* linea);
    void seleccionarColor();
    void editarLinea(LineaAltura* linea);

};

#endif // LINEASALTURADIALOG_H
