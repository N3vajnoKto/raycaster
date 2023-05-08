
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QPushButton>
#include <QKeyEvent>
#include <QActionGroup>
#include <QWidgetAction>
#include "colorbutton.h"
#include "raycastgraph.h"

class RayCast : public QMainWindow
{
    Q_OBJECT

public:
    RayCast(QWidget *parent = nullptr);
    ~RayCast();
    void keyPressEvent(QKeyEvent* event);
private:
    RayCastGraph* raycast;
    QToolBar* modes;
    QAction* light;
    QAction* draw;
    QAction* staticLights;
    QPushButton* colorbutton;
    QPushButton* lightColor;
    void changeColor();
    void changeLightColor();
    QColor color = Qt::white;
    QColor lColor;
};

#endif // MAINWINDOW_H
