
#include "raycast.h"

#include <QGridLayout>
#include <QColorDialog>

RayCast::RayCast(QWidget *parent)
    : QMainWindow(parent), raycast(new RayCastGraph(this))
{
    resize(700, 600);
    setCentralWidget(raycast);

    modes = addToolBar("modes");
    modes->setFixedHeight(40);
    modes->setMovable(false);

    QPalette palTool = modes->palette();
//    palTool.setColor(modes->backgroundRole(), Qt::white);
    modes->setAutoFillBackground(true);
    modes->setPalette(palTool);

    modes->setIconSize(QSize(40, 40));

    light = new QAction(QIcon(":/icons/brightness.png"), "light", modes);
    light ->setShortcut( QKeySequence(Qt::Key_1));
    light->setCheckable(true);

    draw = new QAction(QIcon(":/icons/square.png"), "draw", modes);
    draw ->setShortcut( QKeySequence(Qt::Key_2));
    draw->setCheckable(true);

    staticLights = new QAction(QIcon(":/icons/candle-lamp.png"), "staticLights", modes);
    staticLights ->setShortcut( QKeySequence(Qt::Key_3));
    staticLights->setCheckable(true);

    modes->setContextMenuPolicy(Qt::PreventContextMenu);
    modes->layout()->setSpacing(5);

    colorbutton = new QPushButton(modes);
    colorbutton->setFixedSize(32, 32);
    colorbutton->setFlat(true);

    QPalette pal = colorbutton->palette();
    pal.setColor(QPalette::Button, color);
    colorbutton->setAutoFillBackground(true);
    colorbutton->setPalette(pal);

    lightColor = new QPushButton(modes);
    lightColor->setFixedSize(32, 32);
    lightColor->setFlat(true);

    pal = lightColor->palette();
    pal.setColor(QPalette::Button, color);
    lightColor->setAutoFillBackground(true);
    lightColor->setPalette(pal);

    modes->addAction(light);
    modes->addWidget(lightColor);
    modes->addAction(draw);
    modes->addAction(staticLights);
    modes->addWidget(colorbutton);


    QActionGroup* group = new QActionGroup(this);
    group->addAction(draw);
    group->addAction(light);
    group->addAction(staticLights);

    connect(light, &QAction::triggered, raycast, &RayCastGraph::setLight);
    connect(staticLights, &QAction::triggered, raycast, &RayCastGraph::setStaticLights);
    connect(draw, &QAction::triggered, raycast, &RayCastGraph::setDraw);
    connect(colorbutton, &QPushButton::clicked, this, &RayCast::changeColor);
    connect(lightColor, &QPushButton::clicked, this, &RayCast::changeLightColor);
}

RayCast::~RayCast()
{
}

void RayCast::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier) {
        raycast->back();
    }
}

void RayCast::changeColor() {
    color = QColorDialog::getColor();
    QPalette pal = colorbutton->palette();
    pal.setColor(QPalette::Button, color);
    colorbutton->setPalette(pal);
    raycast->setColor(color);
}

void RayCast::changeLightColor() {
    lColor = QColorDialog::getColor();
    QPalette pal = lightColor->palette();
    pal.setColor(QPalette::Button, lColor);
    lightColor->setPalette(pal);
    raycast->setLightColor(lColor);
}


