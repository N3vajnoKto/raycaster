
#ifndef POLYGON_H
#define POLYGON_H

#include <QWidget>
#include <vector>
#include "ray.h"

class Polygon
{
public:
    Polygon(const std::vector<QPointF>& vertices);
    Polygon() = default;
    void addVertex(const QPointF& vertex);
    void updateLastVertex(const QPointF& new_vertex);
    std::optional<QPointF> intersectRay(const Ray& ray) const;
    std::vector<QPointF> intersectPolygons(const Polygon& pol) const;
    std::optional<QPointF> intersectSegments(Ray first, Ray second) const;
    QPointF& operator[](int i);
    QPointF at(int i) const;
    int size() const;
    void pop();

private:
    std::vector<QPointF> vertices;
    double eps = 0.0000000001;

};

#endif // POLYGON_H
