#include "lineasalturadialog.h"
#include "ui_lineasalturadialog.h"

LineasAlturaDialog::LineasAlturaDialog(DatosProyecto* proyecto, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LineasAlturaDialog)
{
    ui->setupUi(this);
    this->proyecto = proyecto;

     espaciador = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

    ui->colorLabel->setStyleSheet("QLabel { background-color : red; }");    
    for (int i = 0; i < proyecto->lineasAltura.size(); i++) {
        lineas.push_back(proyecto->lineasAltura[i]);
        crearWidget(lineas[i]);
    }

    connect(ui->seleccionarColorBoton, SIGNAL(clicked(bool)), this, SLOT(seleccionarColor()));
    connect(ui->addBoton, SIGNAL(clicked(bool)), this, SLOT(crearLinea()));
    connect(ui->botonera->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(aceptar()));

    setWindowTitle(Idioma::getInstancia()->getTexto("visoralturas.lineasaltura.titulo"));
    ui->nombreLabel->setText(Idioma::getInstancia()->getTexto("visoralturas.lineasaltura.nombre"));
    ui->alturaLabel->setText(Idioma::getInstancia()->getTexto("visoralturas.lineasaltura.altura"));
    ui->metrosLabel->setText(Idioma::getInstancia()->getTexto("general.metros"));
    ui->colorLabel_2->setText(Idioma::getInstancia()->getTexto("visoralturas.lineasaltura.color"));
    ui->seleccionarColorBoton->setToolTip(Idioma::getInstancia()->getTexto("visoralturas.lineasaltura.seleccionarcolor.tooltip"));
    ui->addBoton->setText(Idioma::getInstancia()->getTexto("general.add"));
    ui->seleccionarColorBoton->setText(Idioma::getInstancia()->getTexto("general.seleccionar"));

}

LineasAlturaDialog::~LineasAlturaDialog()
{
    delete ui;
}

void LineasAlturaDialog::aceptar()
{
    proyecto->lineasAltura.clear();
    for (int i = 0; i < lineas.size(); i++) {
        proyecto->lineasAltura.push_back(lineas[i]);
    }
    modoEdicion = false;
    hide();
}

void LineasAlturaDialog::alinear()
{
    for (int i = 0; i < ui->widgetsVerticalLayout->count(); ++i) {
        QLayoutItem *layoutItem = ui->widgetsVerticalLayout->itemAt(i);
        if (layoutItem->spacerItem()) {
            ui->widgetsVerticalLayout->removeItem(layoutItem);
            return;
        }
    }
}

void LineasAlturaDialog::crearWidget(LineaAltura* linea)
{
    LineaAlturaWidget* widget = new LineaAlturaWidget(linea);
    widgets.push_back(widget);    
    alinear();
    connect(widget, SIGNAL(editar(LineaAltura*)), this, SLOT(editarLinea(LineaAltura*)));
    connect(widget, SIGNAL(eliminar(LineaAltura*)), this, SLOT(borrarLinea(LineaAltura*)));
    ui->widgetsVerticalLayout->addWidget(widget);    
    ui->widgetsVerticalLayout->addSpacerItem(espaciador);
}

void LineasAlturaDialog::crearLinea()
{
    if (!modoEdicion) {
        LineaAltura* linea = new LineaAltura();
        linea->id = lineas.size() + 1;
        linea->nombre = ui->nombreLineaEdit->text();
        linea->altura = ui->alturaEdit->text().toInt();
        linea->color = colorProvisional;
        lineas.push_back(linea);
        crearWidget(linea);
    } else {
        borrarLinea(lineaEditar);
        lineaEditar->nombre = ui->nombreLineaEdit->text();
        lineaEditar->altura = ui->alturaEdit->text().toInt();
        lineaEditar->color = colorProvisional;
        lineas.push_back(lineaEditar);
        crearWidget(lineaEditar);
        lineaEditar = Q_NULLPTR;
    }

    modoEdicion = false;
    ui->nombreLineaEdit->setText("");
    ui->alturaEdit->setText("");
    ui->colorLabel->setStyleSheet("QLabel { background-color : red; }");
}

void LineasAlturaDialog::borrarLinea(LineaAltura* linea)
{
    for (int i = 0; i < widgets.size(); i++) {
        if (widgets[i]->linea->id == linea->id) {
            ui->widgetsVerticalLayout->removeWidget(widgets[i]);
            ui->widgetsVerticalLayout->update();
            widgets.removeAt(i);
            break;
        }
    }
    for (int i = 0; i < lineas.size(); i++) {
        if (lineas[i]->id == linea->id) {
            lineas.removeAt(i);
            break;
        }
    }
    update();
}

void LineasAlturaDialog::seleccionarColor()
{
    QColor color = QColorDialog::getColor(Qt::yellow, this );
    if (color.isValid() )
    {
        ui->colorLabel->setStyleSheet("QLabel { background-color : " + color.name() + "; }");
        colorProvisional = color;
    }
}

void LineasAlturaDialog::editarLinea(LineaAltura* linea)
{
    modoEdicion = true;
    ui->nombreLineaEdit->setText(linea->nombre);
    ui->alturaEdit->setText(QString::number(linea->altura));
    ui->colorLabel->setStyleSheet("QLabel { background-color : "+linea->color.name()+"; }");
    lineaEditar = linea;
}
