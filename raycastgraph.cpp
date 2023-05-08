
#include "raycastgraph.h"
#include <QRadialGradient>

RayCastGraph::RayCastGraph(QWidget *parent)
    : QWidget(parent)
{
    QPalette pal;
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    Polygon pol;
    pol.addVertex(QPointF(-10, -10));
    pol.addVertex(QPointF(width() + 10, -10));
    pol.addVertex(QPointF(width() + 10, height() + 10));
    pol.addVertex(QPointF(-10, height() + 10));
    controller.addPolygon(pol);
}

void RayCastGraph::paintEvent(QPaintEvent *event) {
    QPainter paint(this);
    paint.setPen(QPen(Qt::white, 2));

    for (int i = 0; i < controller.size(); ++i) {
        if (controller[i].size() == 1) {
            paint.drawPoint(controller[i][0]);
        }
        QPainterPath pnt(controller[i][0]);
        for (int j = 1; j < controller[i].size(); ++j) {
            pnt.lineTo(controller[i][j]);
        }
        pnt.closeSubpath();
        paint.drawPath(pnt);
    }

    for (auto& src : controller.getStaticLights()) {
        paintLight(src, &paint, color);
    }

    if (mode == Mode::Light) {
        paintLight(controller.getSource(), &paint, lColor);
    }

    for (auto& src : controller.getStaticLights()) {
        QPixmap img(":/icons/candle-lamp.png");
        img = img.scaled(40, 40);
        QPainter PixPaint(&img);
        PixPaint.setCompositionMode(QPainter::CompositionMode_SourceIn);
        PixPaint.fillRect(img.rect(), Qt::black);
        paint.drawPixmap(src[0].x() - 20, src[0].y() - 20, 32, 32, img);
    }

//    paint.setPen(QPen(Qt::red, 4));
//    for (auto to : controller.targets) {
//        paint.drawPoint(to);
//    }

}

void RayCastGraph::paintLight(const std::vector<QPointF>& points, QPainter* paint, QColor lightColor) {
    paint->save();

    for (auto src : points) {
        auto rays = controller.castRays(src);
        controller.IntersectRays(rays);
        controller.RemoveAdjacentRays(rays);

        paint->setPen(Qt::NoPen);

        QRadialGradient grad(src, height());
        lightColor.setAlpha(255 / 2);
        grad.setStops({{0, lightColor}, {1, Qt::transparent}});
        paint->setBrush(grad);

        Polygon pol = controller.CreateLightArea(rays);

        QPainterPath pnt(pol[0]);

        for (int i = 1; i < pol.size(); ++i) {
            pnt.lineTo(pol[i]);
        }
        pnt.closeSubpath();
        paint->drawPath(pnt);

//        paint->setPen(QPen(Qt::red, 4));
//        for (auto to : rays) {
//            paint->drawPoint(to.end());
//        }
    }
    paint->restore();
}

void RayCastGraph::mousePressEvent(QMouseEvent* event)
{
    if (mode == Mode::Polygon) {
        if (event->button() == Qt::LeftButton) {
            if (firstClick) {
                Polygon pol;
                pol.addVertex(QPointF(event->x(), event->y()));
                controller.addPolygon(pol);
                firstClick = false;
            } else {
                controller.addVertexToLastPolygon(event->pos());
            }
        }
        if (event->button() == Qt::RightButton) {
            firstClick = true;
        }
    }
    if (mode == Mode::StaticLights){
        controller.addStaticLight(event->pos());
    }

    update();
}

void RayCastGraph::mouseReleaseEvent(QMouseEvent *event) {
    if (mode == Mode::Polygon) {
        if (event->button() == Qt::LeftButton) {
            controller.back().updateLastVertex(QPointF(event->x(), event->y()));
        }
    }
}

void RayCastGraph::mouseMoveEvent(QMouseEvent *event) {
    if (mode == Mode::Polygon) {
        controller.back().updateLastVertex(QPointF(event->x(), event->y()));
        controller.rebuildTargets();
    }
    if (mode == Mode::Light) {
        controller.setSource(QPointF(event->x(), event->y()));
    }

    update();

}

void RayCastGraph::setLight(bool checked) {
    if (checked) {
        mode = Mode::Light;
        firstClick = true;
        setMouseTracking(true);
        QPixmap cursor = QPixmap(":/icons/lantern.png");
        cursor = cursor.scaled(32, 32, Qt::KeepAspectRatio);
        QPainter painter(&cursor);
        painter.setBrush(Qt::red);
        setCursor(QCursor(cursor));
    }
    update();
}

void RayCastGraph::setDraw(bool checked) {
    if (checked) {
        mode = Mode::Polygon;
        setCursor(Qt::ArrowCursor);
        setMouseTracking(false);
    }
    update();
}

void RayCastGraph::setStaticLights(bool checked) {
    if (checked) {
        mode = Mode::StaticLights;
        setCursor(Qt::ArrowCursor);
        setMouseTracking(false);
    }
    update();
}

void RayCastGraph::resizeEvent(QResizeEvent *event) {
    controller[0][1].rx() = event->size().width() + 10;
    controller[0][2].rx() = event->size().width() + 10;
    controller[0][2].ry() = event->size().height() + 10;
    controller[0][3].ry() = event->size().height() + 10;
    controller.rebuildTargets();
}

void RayCastGraph::back() {
    if (mode == Mode::Polygon) {
        if (controller.size() > 1 && controller.back().size() > 1) {
            controller.back().pop();
        }
        else if (controller.size() > 1) {
            controller.pop();
            firstClick = 1;
        }
         controller.rebuildTargets();
    }
    if (mode == Mode::StaticLights) {
        if (controller.sizeOfStaticLights() > 0) controller.popStaticLight();
    }
    update();
}

void RayCastGraph::setColor(QColor color) {
    this->color = color;
    update();
}

void RayCastGraph::setLightColor(QColor color) {
    this->lColor = color;
    update();
}

