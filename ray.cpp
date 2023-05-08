
#include "ray.h"

Ray::Ray(const QPointF& beg, const QPointF& end) {
    begin_ = beg;
    direct_ = end - beg;
    end_ = end;
    angle_ = acos((end.x() - beg.x()) / length());
    if (direct_.y() < 0) angle_ = 2 * M_PI - angle_;

}

Ray::Ray(const QPointF& beg, double angle) {
    if (angle < 0) angle += 2 * M_PI;
    if (angle > 2 * M_PI) angle -= 2 * M_PI;
    direct_ = QPointF(cos(angle), sin(angle));
    begin_ = beg;
    angle_ = angle;
}

void Ray::normalize() {
    direct_.rx() /= length();
    direct_.ry() /= length();
}
double Ray::length() const {
    return sqrt( direct_.x() * direct_.x() + direct_.y() * direct_.y());
}

Ray Ray::Rotate(double ang) const {
    return Ray(begin_, ang + angle_);
}

QPointF& Ray::rbegin() {
    return begin_;
}

QPointF& Ray::rdirect() {
    return direct_;
}

QPointF Ray::begin() const {
    return begin_;
}

QPointF Ray::direct() const {
    return direct_;
}

void Ray::setEnd(QPointF pnt) {
    end_ = pnt;
}

QPointF Ray::end() const {
    return end_;
}

double Ray::angle() const {
    return angle_;
}


