#ifndef __VEINS_LineLib_H_
#define __VEINS_LineLib_H_

using namespace std;

#include <veins/modules/application/misbehaviorDetection/supportClasses/rectIntLib/Vector.h>

class Line {
public:
    Line(Vector, Vector);
    double a;
    double b;
    double c;
    double call(Vector p);
    Vector intersection(Line other);
};

#endif
