#define _USE_MATH_DEFINES
#include "controller.h"
#include <iostream>
#include <QDebug>

Controller::Controller() {
   createNext();
}

Controller::Controller(const std::vector<Polygon>& polygons) {
    this->polygons = polygons;
    createNext();
}

void Controller::createNext() {

    double angle = 0;
    double delta = 2 * M_PI / (numOfLights - 1);

    for (int i = 0; i < numOfLights - 1; ++i) {
        next.push_back(QPointF(5 * cos(angle), 5 * sin(angle)));
        angle += delta;
    }
}

void Controller::addPolygon(const Polygon& pol) {
    polygons.push_back(pol);
    rebuildTargets();
}

void Controller::updateLastPolygon(const Polygon& new_pol) {
    polygons.back() = new_pol;
    rebuildTargets();
}


void Controller::addVertexToLastPolygon(const QPointF& new_vertex) {
    polygons.back().addVertex(new_vertex);
    rebuildTargets();
}

void Controller::updateLastPolygon(const QPointF& new_vertex) {
    polygons.back().updateLastVertex(new_vertex);
    rebuildTargets();
}

Polygon& Controller::operator[](int i) {
    if (i >= polygons.size() || i < 0) {
        throw std::invalid_argument("out_of_bounds");
    }

    return polygons[i];
}

int Controller::size() const {
    return polygons.size();
}

Polygon& Controller::back() {
    return polygons.back();
}

void Controller::pop() {
    polygons.pop_back();
    rebuildTargets();
}

void Controller::setSource(QPointF pnt) {
    source.clear();
    source.push_back(pnt);

    for (int i = 0; i < numOfLights - 1; ++i) {
        QPointF src = pnt + next[i];
        source.push_back(src);
    }
}
std::vector<QPointF>& Controller::getSource() {
    return source;
}

std::vector<Ray> Controller::castRays(QPointF src) {
    std::vector<Ray> res;

    for (auto pol : targets) {
        Ray ray = Ray(src, pol);
        ray.setEnd(pol);
        res.push_back(ray);
        res.push_back(ray.Rotate(eps));
        res.push_back(ray.Rotate(-eps));
    }

    return res;
}

void Controller::IntersectRays(std::vector<Ray>& rays) {
    for (auto& ray : rays) {
        long long rst = std::numeric_limits<long long>::max();

        for (auto pol : polygons) {
            auto tmp = pol.intersectRay(ray);
            if (tmp) {
                QPointF pnt = *tmp;
                long long dst = (pnt.x() - ray.begin().x()) * (pnt.x() - ray.begin().x())
                                + (pnt.y() - ray.begin().y()) * (pnt.y() - ray.begin().y());
                if ( dst < rst) {
                    rst = dst;
                    ray.setEnd(pnt);
                }
            }
        }
    }
}

double dist(QPointF a, QPointF b) {
    return sqrt( (a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
}

void Controller::RemoveAdjacentRays(std::vector<Ray>& rays) {
    std::vector<Ray> res;

    for (auto to : rays) {
        if (to.end() == QPointF(-1, -1)) {
            continue;
        }
        if (res.empty() || dist(res.back().end(), to.end()) >= 1.5) {
            res.push_back(to);
        }
    }

    rays = res;
}

Polygon Controller::CreateLightArea(std::vector<Ray>& rays){
    sort(rays.begin(), rays.end(), [](Ray& a, Ray& b) {
        return (a.angle() < b.angle());
    });
    Polygon pol;
    for (int i = 0; i < rays.size(); ++i) {
        pol.addVertex(rays[i].end());
    }

    return pol;
}

int Controller::numberOfLights(){
    return numOfLights;
}

void Controller::addStaticLight(QPointF src) {

    staticLights.push_back({});
    staticLights.back().push_back(src);

    for (int i = 0; i < numOfLights - 1; ++i) {
        QPointF pnt = src + next[i];
        staticLights.back().push_back(pnt);
    }
}

void Controller::popStaticLight() {
    staticLights.pop_back();
}

int Controller::sizeOfStaticLights() const {
    return staticLights.size();
}

std::vector<std::vector<QPointF> >& Controller::getStaticLights() {
    return staticLights;
}

void Controller::rebuildTargets() {
    targets.clear();

    for (int i = 0; i < polygons.size(); ++i) {
        for (int j = i; j < polygons.size(); ++j) {
            auto vec = polygons[i].intersectPolygons(polygons[j]);
            for (auto to : vec) {
                targets.push_back(to);
            }
            if (polygons[i].size() == 2) {
                targets.push_back(polygons[i][0]);
                targets.push_back(polygons[i][1]);
            }
        }
    }
}
