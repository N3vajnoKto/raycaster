
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <vector>
#include "polygon.h"

class Controller
{
public:
    Controller(const std::vector<Polygon>& polygons);
    Controller();
    void addPolygon(const Polygon& pol);
    void updateLastPolygon(const Polygon& new_pol);
    void addVertexToLastPolygon(const QPointF& new_vertex);
    void updateLastPolygon(const QPointF& new_vertex);

    Polygon& operator[](int i);
    Polygon& back();
    int size() const;
    void pop();

    std::vector<Ray> castRays(QPointF src);
    void IntersectRays(std::vector<Ray>& rays);
    void RemoveAdjacentRays(std::vector<Ray>& rays);
    Polygon CreateLightArea(std::vector<Ray>& rays);
    void setSource(QPointF pnt);
    void createNext();
    void addStaticLight(QPointF src);
    void popStaticLight();
    std::vector<std::vector<QPointF> >& getStaticLights();
    int sizeOfStaticLights() const;
    std::vector<QPointF>& getSource();
    int numberOfLights();
    void rebuildTargets();
    std::vector<QPointF> targets;

private:
    std::vector<Polygon> polygons;
    std::vector<QPointF> source;
    std::vector<std::vector<QPointF> > staticLights;
    std::vector<QPointF> next;
    double eps = 0.0001;
    int numOfLights = 5;

};


#endif // CONTROLLER_H
