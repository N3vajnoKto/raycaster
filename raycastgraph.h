
#ifndef RAYCAST_H
#define RAYCAST_H

#include <QWidget>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPainterPath>
#include "controller.h"
#include <vector>

enum class Mode{
    Light,
    Polygon,
    StaticLights
};

class RayCastGraph : public QWidget
{
    Q_OBJECT
public:
    explicit RayCastGraph(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    Controller controller;
    Mode mode = Mode::Polygon;
    bool firstClick = true;
    QPainter paint;
    QColor color = Qt::white;
    QColor lColor = Qt::white;
public slots:
    void setLight(bool checked);
    void setColor(QColor color);
    void setLightColor(QColor color);
    void setDraw(bool checked);
    void setStaticLights(bool checked);
    void paintLight(const std::vector<QPointF>& points, QPainter* paint, QColor lightColor = Qt::white);
    void back();
signals:

};

#endif // RAYCAST_H
