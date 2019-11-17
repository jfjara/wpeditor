#include "etiquetawidget.h"

EtiquetaWidget::EtiquetaWidget(QWidget *parent) : QLabel(parent)
{

}

void EtiquetaWidget::mousePressEvent(QMouseEvent *)
{
    emit Mouse_Pressed();
}
