#include "FqImageWidget.hpp"
#include "ui_FqImageWidget.h"

FqImageWidget::FqImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FqImageWidget)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    label = ui->label;
}

FqImageWidget::~FqImageWidget()
{
    delete ui;
}

void FqImageWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    ui->label->resize(ui->widget->size());
}
