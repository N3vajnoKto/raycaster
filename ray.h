
#ifndef RAY_H
#define RAY_H

#define _USE_MATH_DEFINES

#include <QWidget>

class Ray
{
public:
    Ray(const QPointF& beg, const QPointF& end);
    Ray(const QPointF& beg, double angle);
    void normalize();
    double length() const;
    double angle() const;
    Ray Rotate(double angle) const;
    QPointF& rbegin();
    QPointF& rdirect();

    QPointF begin() const;
    QPointF direct() const;

    void setEnd(QPointF pnt);
    QPointF end() const;
private:
    QPointF begin_ = QPointF(0, 0);
    QPointF direct_ = QPointF(1, 0);
    QPointF end_ = QPointF(-1, -1);
    double angle_ = 0;
};

#endif // RAY_H
