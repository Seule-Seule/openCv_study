#ifndef FQIMAGEWIDGET_HPP
#define FQIMAGEWIDGET_HPP

#include <QWidget>
#include <QLabel>

namespace Ui {
class FqImageWidget;
}

class FqImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FqImageWidget(QWidget *parent = nullptr);
    ~FqImageWidget();

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    QLabel* label;

private:
    Ui::FqImageWidget *ui;
};

#endif // FQIMAGEWIDGET_HPP
