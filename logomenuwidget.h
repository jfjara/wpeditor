#ifndef LOGOMENUWIDGET_H
#define LOGOMENUWIDGET_H

#include <QWidget>

namespace Ui {
class LogoMenuWidget;
}

class LogoMenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LogoMenuWidget(QWidget *parent = 0);
    ~LogoMenuWidget();

private:
    Ui::LogoMenuWidget *ui;
};

#endif // LOGOMENUWIDGET_H
