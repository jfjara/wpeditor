#ifndef ETIQUETAWIDGET_H
#define ETIQUETAWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class EtiquetaWidget : public QLabel
{
    Q_OBJECT
public:
    explicit EtiquetaWidget(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev);
signals:
    void Mouse_Pressed();

public slots:
};

#endif // ETIQUETAWIDGET_H
