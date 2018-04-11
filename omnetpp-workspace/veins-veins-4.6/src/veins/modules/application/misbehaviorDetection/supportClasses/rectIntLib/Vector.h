#ifndef __VEINS_VectorLib_H_
#define __VEINS_VectorLib_H_

using namespace std;

class Vector {
public:
    Vector();
    Vector(double, double);
    double x;
    double y;
    Vector add(Vector v);
    Vector sub(Vector v);
    double cross(Vector v);
};

#endif
