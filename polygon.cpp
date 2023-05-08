
#include "polygon.h"
#include <limits.h>
#include <iostream>
#include <QDebug>

Polygon::Polygon(const std::vector<QPointF>& vertices) {
    this->vertices = vertices;
}

void Polygon::addVertex(const QPointF& vertex) {
    vertices.push_back(vertex);
}

void Polygon::updateLastVertex(const QPointF& new_vertex) {
    vertices.back() = new_vertex;
}

std::optional<QPointF> Polygon::intersectRay(const Ray& ray) const {
    long long res = std::numeric_limits<long long>::max();
    QPointF resPnt;
    bool found = false;
    for (int i = 0; i < vertices.size(); ++i) {
        int next = (i + 1) % vertices.size();
        QPointF norm = QPointF(-ray.direct().y(), ray.direct().x());
        QPointF dir = vertices[next] - vertices[i];
        if (abs(norm.x() * dir.x() + norm.y() * dir.y()) > eps) {
            double t = -norm.x() * vertices[i].x() - norm.y() * vertices[i].y()
                       + norm.x() * ray.begin().x() + norm.y() * ray.begin().y();
            t /= norm.x() * dir.x() + norm.y() * dir.y();

            if (t >= -eps && t < 1 + eps) {

                QPointF pnt(vertices[i].x() + dir.x() * t, vertices[i].y() + dir.y() * t);
                if (ray.direct().x() !=0 && (pnt.x() - ray.begin().x()) / ray.direct().x() < 0) continue;
                if (ray.direct().y() !=0 && (pnt.y() - ray.begin().y()) / ray.direct().y() < 0) continue;
                long long dst = (pnt.x() - ray.begin().x()) * (pnt.x() - ray.begin().x())
                             + (pnt.y() - ray.begin().y()) * (pnt.y() - ray.begin().y());

                if (dst < res) {
                    res = dst;
                    resPnt = pnt;
                    found = true;

                }
            }
        }
    }

    if (found) {
        return std::optional<QPointF>(resPnt);
    } else {
        return std::nullopt;
    }
}
std::optional<QPointF> Polygon::intersectSegments(Ray first, Ray second) const {
    QPointF norm = QPointF(-second.direct().y(), second.direct().x());
    QPointF dir = first.direct();
    if (abs(norm.x() * dir.x() + norm.y() * dir.y()) > eps) {
        double t = -norm.x() * first.begin().x() - norm.y() * first.begin().y()
                   + norm.x() * second.begin().x() + norm.y() * second.begin().y();
        t /= norm.x() * dir.x() + norm.y() * dir.y();

        if (t >= -eps && t < 1 + eps) {

            QPointF pnt(first.begin().x() + dir.x() * t, first.begin().y() + dir.y() * t);
            double t1;
            QPointF dr = pnt - second.begin();
            if (second.direct().x() != 0) t1 = dr.x() / second.direct().x();
            else t1 = dr.y() / second.direct().y();
            if (t1 > -eps && t1 < 1 + eps) {
                return std::optional<QPointF>(pnt);
            } else return std::nullopt;
        }
    }

    return std::nullopt;
}

std::vector<QPointF> Polygon::intersectPolygons(const Polygon &pol) const {
    std::vector<QPointF> res;
    for (int i = 0; i < vertices.size(); ++i) {
        int next = (i + 1) % vertices.size();
        for (int j = 0; j < pol.size(); ++j) {
            int next1 = (j + 1) % pol.size();
            auto pnt = intersectSegments(Ray(vertices[i], vertices[next]), Ray(pol.at(j), pol.at(next1)));
            if (pnt) {
                res.push_back(*pnt);
            }
        }
    }

    return res;
}

QPointF& Polygon::operator[](int i) {
    if (i >= vertices.size() || i < 0) {
        throw std::invalid_argument("out_of_bounds");
    }
    return vertices[i];
}

QPointF Polygon::at(int i) const {
    if (i >= vertices.size() || i < 0) {
        throw std::invalid_argument("out_of_bounds");
    }
    return vertices[i];
}

int Polygon::size() const {
    return vertices.size();
}

void Polygon::pop() {
    vertices.pop_back();
}
