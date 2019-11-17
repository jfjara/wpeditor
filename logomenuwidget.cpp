#include "logomenuwidget.h"
#include "ui_logomenuwidget.h"

LogoMenuWidget::LogoMenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogoMenuWidget)
{
    ui->setupUi(this);
}

LogoMenuWidget::~LogoMenuWidget()
{
    delete ui;
}
