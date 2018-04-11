#include <veins/modules/application/misbehaviorDetection/supportClasses/rectIntLib/Vector.h>

Vector::Vector() {
    this->x = 0;
    this->y = 0;
}

Vector::Vector(double x, double y) {
    this->x = x;
    this->y = y;
}

Vector Vector::add(Vector v) {
    return Vector(this->x + v.x, this->y + v.y);
}

Vector Vector::sub(Vector v) {
    return Vector(this->x - v.x, this->y - v.y);
}

double Vector::cross(Vector v) {
    return this->x * v.y - this->y * v.x;
}
