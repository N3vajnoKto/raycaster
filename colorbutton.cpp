#include "colorbutton.h"

ColorButton::ColorButton(QColor cl, QWidget *parent)
    : QFrame{parent}, color(new QWidget(this))
{
    setFixedSize(40, 40);
    color->setFixedSize(height() * 0.6, height() * 0.6);
    QGridLayout* lay = new QGridLayout();

    lay->setSpacing(0);
    lay->setContentsMargins(QMargins(0,0,0,0));
    lay->setAlignment(Qt::AlignCenter);

    lay->addWidget(color);
    setLayout(lay);

    QPalette pal;
    pal.setColor(QPalette::Window, cl);
    color->setAutoFillBackground(true);
    color->setPalette(pal);
}

void ColorButton::mousePressEvent(QMouseEvent*) {
    emit clicked();
}

