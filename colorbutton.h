#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QFrame>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QGridLayout>
#include <QPainter>

class ColorButton : public QFrame
{
    Q_OBJECT
public:
    explicit ColorButton(QColor, QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent*) override;
    QWidget* color;
signals:
    void clicked();
};

#endif // COLORBUTTON_H
