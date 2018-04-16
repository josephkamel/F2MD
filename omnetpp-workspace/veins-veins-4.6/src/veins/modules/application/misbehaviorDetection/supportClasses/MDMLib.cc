/*******************************************************************************
 * @author  Joseph Kamel
 * @date    11/04/2014
 * @version 1.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/

#include "MDMLib.h"

using namespace std;

double MDMLib::calculateDistance(Coord pos1, Coord pos2) {
    return sqrt(pow(pos1.x - pos2.x, 2.0) + pow(pos1.y - pos2.y, 2.0));
    //   + pow(pos1.z - pos2.z, 2.0));
}

double MDMLib::calculateSpeed(Coord Speed) {
    return sqrt(pow(Speed.x, 2.0) + pow(Speed.y, 2.0) + pow(Speed.z, 2.0));
}

double MDMLib::calculateDeltaTime(BasicSafetyMessage bsm1,
        BasicSafetyMessage bsm2) {
    return fabs(bsm1.getArrivalTime().dbl() - bsm2.getArrivalTime().dbl());
}

double MDMLib::calculatePolynom(long double coof[], const int coofNum,
        double x) {
    double y = 0;
    for (int var = 0; var < coofNum; ++var) {
        y = y + coof[var] * pow(x, var);
    }
    return y;
}

double MDMLib::calculateCircleSegment(double radius, double intDistance) {
    double area = 0;

    if (radius <= 0) {
        return 0;
    }

    if (intDistance <= 0) {
        return 0;
    }

    if (intDistance > 2 * radius) {
        return PI * radius * radius;
    }

    if (radius > intDistance) {
        area = radius * radius * acos((radius - intDistance) / radius)
                - (radius - intDistance)
                        * sqrt(
                                2 * radius * intDistance
                                        - intDistance * intDistance);
    } else {
        double intDistanceTemp = 2 * radius - intDistance;
        area = radius * radius * acos((radius - intDistanceTemp) / radius)
                - (radius - intDistanceTemp)
                        * sqrt(
                                2 * radius * intDistanceTemp
                                        - intDistanceTemp * intDistanceTemp);
        area = PI * radius * radius - area;
    }

    return area;
}

double MDMLib::calculateCircleCircleIntersection(double r0, double r1,
        double d) {

    if (r0 <= 0 || r1 <= 0) {
        return 0;
    }

    double rr0 = r0 * r0;
    double rr1 = r1 * r1;

    // Circles do not overlap
    if (d > r1 + r0) {
        return 0;
    }
    // Circle1 is completely inside circle0
    else if (d <= fabs(r0 - r1) && r0 >= r1) {
        // Return area of circle1
        return PI * rr1;
    }

// Circle0 is completely inside circle1
    else if (d <= fabs(r0 - r1) && r0 < r1) {
        // Return area of circle0
        return PI * rr0;
    }

// Circles partially overlap
    else {
        double phi = (acos((rr0 + (d * d) - rr1) / (2 * r0 * d))) * 2;
        double theta = (acos((rr1 + (d * d) - rr0) / (2 * r1 * d))) * 2;
        double area1 = 0.5 * theta * rr1 - 0.5 * rr1 * sin(theta);
        double area2 = 0.5 * phi * rr0 - 0.5 * rr0 * sin(phi);

// Return area of intersection
        return area1 + area2;
    }
}

double MDMLib::calculateHeadingAngle(Coord heading) {
    double x2 = 1;
    double y2 = 0;

    double dot = heading.x * x2 + heading.y * y2; // dot product between [x1, y1] and [x2, y2]
    double det = heading.x * y2 - heading.y * x2;      // determinant
    double angle = atan2(det, dot) * 180 / PI; // atan2(y, x) or atan2(sin, cos);

    if (heading.x >= 0 && heading.y > 0) {
        angle = 360 + angle;
    } else if (heading.x <0 && heading.y >= 0) {
        angle = 360 + angle;
    }

    return angle;
}

double MDMLib::CircleCircleFactor(double d, double r1, double r2,
        double range) {

    double d1 = 0;
    double d2 = 0;
    if (d > 0) {
        d1 = (r1 * r1 + d * d - r2 * r2) / (2 * d);
        d2 = (r2 * r2 + d * d - r1 * r1) / (2 * d);

        if ((d1 + r1) < range / 2 && (d2 + r2) > range / 2) {
            d2 = d2 - (range / 2 - (d1 + r1));
            d1 = d1 + (range / 2 - (d1 + r1));
        }

        if ((d2 + r2) < range / 2 && (d1 + r1) > range / 2) {
            d1 = d1 - (range / 2 - (d2 + r2));
            d2 = d2 + (range / 2 - (d2 + r2));
        }
    }

    if (r1 <= 0 && r2 <= 0) {
        if (range >= d) {
            return 1;
        } else {
            return 0;
        }
    } else if (r1 <= 0) {
        if (range / 2 >= d1) {
            double area2 = calculateCircleCircleIntersection(r2, range / 2, d2);
            double factor = (area2) / (PI * r2 * r2);
            return factor;
        } else {
            return 0;
        }
    } else if (r2 <= 0) {
        if (range / 2 >= d2) {
            double area1 = calculateCircleCircleIntersection(r1, range / 2, d1);
            double factor = (area1) / (PI * r1 * r1);
            return factor;
        } else {
            return 0;
        }
    } else {
        double area1 = calculateCircleCircleIntersection(r1, range / 2, d1);
        double area2 = calculateCircleCircleIntersection(r2, range / 2, d2);

        double factor = (area1 + area2) / (PI * r1 * r1 + PI * r2 * r2);

        return factor;
    }
}

double MDMLib::SegmentSegmentFactor(double d, double r1, double r2,
        double range) {

    double d1 = 0;
    double d2 = 0;
    if (d > 0) {
        d1 = (r1 * r1 + d * d - r2 * r2) / (2 * d);
        d2 = (r2 * r2 + d * d - r1 * r1) / (2 * d);

        if ((d1 + r1) < range / 2 && (d2 + r2) > range / 2) {
            d2 = d2 - (range / 2 - (d1 + r1));
            d1 = d1 + (range / 2 - (d1 + r1));
        }

        if ((d2 + r2) < range / 2 && (d1 + r1) > range / 2) {
            d1 = d1 - (range / 2 - (d2 + r2));
            d2 = d2 + (range / 2 - (d2 + r2));
        }
    }

    double overlap1 = 0;
    double overlap2 = 0;

    double addon = 0;

    if ((d1 - range / 2) < r1) {
        if ((d1 - range / 2) > -r1) {
            addon = -(d1 - r1);
            if (addon < range / 2) {
                overlap1 = range / 2 + addon;
            } else {
                overlap1 = range;
            }
        } else {
            overlap1 = 2 * r1;
        }
    }

    if ((d2 - range / 2) < r2) {
        if ((d2 - range / 2) > -r2) {
            addon = -(d2 - r2);
            if (addon < range / 2) {
                overlap2 = range / 2 + addon;
            } else {
                overlap2 = range;
            }
        } else {
            overlap2 = 2 * r2;
        }
    }

    double factor = (overlap1 + overlap2) / (2 * r1 + 2 * r2);
    return factor;
}

double MDMLib::CircleSegmentFactor(double d, double r1, double r2,
        double range) {

    if (range > d + r1 + r2) {
        return 1;
    } else {
        if (range < d - r1 - r2) {
            return 0;
        } else {

            double d1 = 0;
            double d2 = 0;
            if (d > 0) {
                d1 = (r1 * r1 + d * d - r2 * r2) / (2 * d);
                d2 = (r2 * r2 + d * d - r1 * r1) / (2 * d);

                if ((d1 + r1) < range / 2 && (d2 + r2) > range / 2) {
                    d2 = d2 - (range / 2 - (d1 + r1));
                    d1 = d1 + (range / 2 - (d1 + r1));
                }

                if ((d2 + r2) < range / 2 && (d1 + r1) > range / 2) {
                    d1 = d1 - (range / 2 - (d2 + r2));
                    d2 = d2 + (range / 2 - (d2 + r2));
                }
            }

            if (r1 <= 0 && r2 <= 0) {
                if (range >= d) {
                    return 1;
                } else {
                    return 0;
                }
            } else if (r1 <= 0) {
                if (range / 2 >= d1) {
                    double intD2 = (range / 2) - (d2 - r2);
                    double area2 = calculateCircleSegment(r2, intD2);
                    double area22 = 0;

                    if (d2 + (range / 2) < r2) {
                        double intD22 = r2 - (d2 + (range / 2));
                        area22 = calculateCircleSegment(r2, intD22);
                    }

                    double factor = (area2 - area22) / (PI * r2 * r2);
                    return factor;
                } else {
                    return 0;
                }
            } else if (r2 <= 0) {
                if (range / 2 >= d2) {
                    double intD1 = (range / 2) - (d1 - r1);
                    double area1 = calculateCircleSegment(r1, intD1);
                    double area12 = 0;
                    if (d1 + (range / 2) < r1) {
                        double intD12 = r1 - (d1 + (range / 2));
                        area12 = calculateCircleSegment(r1, intD12);
                    }
                    double factor = (area1 - area12) / (PI * r1 * r1);

                    return factor;
                } else {
                    return 0;
                }
            } else {

                double intD1 = (range / 2) - (d1 - r1);
                double intD2 = (range / 2) - (d2 - r2);

                double area1 = calculateCircleSegment(r1, intD1);
                double area2 = calculateCircleSegment(r2, intD2);

                double area12 = 0;
                double area22 = 0;

                if (d2 + (range / 2) < r2) {
                    double intD22 = r2 - (d2 + (range / 2));
                    area22 = calculateCircleSegment(r2, intD22);
                }

                if (d1 + (range / 2) < r1) {
                    double intD12 = r1 - (d1 + (range / 2));
                    area12 = calculateCircleSegment(r1, intD12);
                }

                double factor = (area1 - area12 + area2 - area22)
                        / (PI * r1 * r1 + PI * r2 * r2);

                return factor;
            }

        }
    }
}

double MDMLib::OneSidedCircleSegmentFactor(double d, double r1, double r2,
        double range) {

    if (range > d + r1 + r2) {
        return 1;
    } else {
        if (range < d - r1 - r2) {
            return 0;
        } else {

            double d1 = 0;
            double d2 = 0;
            if (d > 0) {
                d1 = (r1 * r1 + d * d - r2 * r2) / (2 * d);
                d2 = (r2 * r2 + d * d - r1 * r1) / (2 * d);
                if ((d1 + r1) < range / 2 && (d2 + r2) > range / 2) {
                    d2 = d2 - (range / 2 - (d1 + r1));
                    d1 = d1 + (range / 2 - (d1 + r1));
                }

                if ((d2 + r2) < range / 2 && (d1 + r1) > range / 2) {
                    d1 = d1 - (range / 2 - (d2 + r2));
                    d2 = d2 + (range / 2 - (d2 + r2));
                }
            }
            if (r1 <= 0 && r2 <= 0) {
                if (range >= d) {
                    return 1;
                } else {
                    return 0;
                }
            } else if (r1 <= 0) {
                if (range / 2 >= d1) {
                    double intD2 = (range / 2) - (d2 - r2);
                    double area2 = calculateCircleSegment(r2, intD2);

                    double factor = (area2) / (PI * r2 * r2);
                    return factor;
                } else {
                    return 0;
                }
            } else if (r2 <= 0) {
                if (range / 2 >= d2) {
                    double intD1 = (range / 2) - (d1 - r1);
                    double area1 = calculateCircleSegment(r1, intD1);

                    double factor = (area1) / (PI * r1 * r1);

                    return factor;
                } else {
                    return 0;
                }
            } else {

                double intD1 = (range / 2) - (d1 - r1);
                double intD2 = (range / 2) - (d2 - r2);

                double area1 = calculateCircleSegment(r1, intD1);
                double area2 = calculateCircleSegment(r2, intD2);

                double factor = (area1 + area2) / (PI * r1 * r1 + PI * r2 * r2);

                return factor;
            }

        }
    }
}

double noCircles = 0;

void MDMLib::countCircles(double rc, double rl, double rs) {
    double no = (int) ((2 * PI * rc) / (2 * rs));
    double x0 = rc * cos(0 * 2 * PI / no);
    double y0 = rc * sin(0 * 2 * PI / no);
    double x1 = rc * cos(1 * 2 * PI / no);
    double y1 = rc * sin(1 * 2 * PI / no);
    double dist = pow((pow((x0 - x1), 2) + pow((y0 - y1), 2)), 0.5);
    if (dist < 2 * rs)
        no = no - 1;
    for (int i = 0; i < no; i++) {
        double x = rc * cos(i * 2 * PI / no);
        double y = rc * sin(i * 2 * PI / no);
        noCircles = noCircles + 1;
    }
    double rcNext = rc - (2 * rs);
    if (rcNext >= rs) {
        countCircles(rcNext, rl, rs);
    } else if (rc > 2 * rs) {
        noCircles = noCircles + 1;
    }
}

double MDMLib::calculateCircles(double dl, double ds) {
    noCircles = 0;
    double rl = dl / 2;
    double rs = ds / 2;
    if (rl > 0 & rs > 0) {
        double rc = rl - rs;
        if (rs > rl) {
            //std::cout<<"Inside smaller diameters larger than outside diameter!"<<'\n';
            return 0;
        } else if (rl < 2 * rs) {
            //std::cout<<"Maximum number of smaller pipes or circles innside the larger one: " << 1  <<'\n';
            return 1;
        } else {
            countCircles(rc, rl, rs);
            //std::cout<<"Maximum number of smaller pipes or circles innside the larger one: " << noCircles << " dl:" <<dl  <<" ds:"<<ds  <<'\n';
            return noCircles;
        }
    } else {
        //std::cout<<"Diameters must be larger than 0!"<<'\n';
        return 0;
    }
}

double MDMLib::gaussianSum(double x, double sig) {
    x = x / sig;

    // constants
    double a1 = 0.254829592;
    double a2 = -0.284496736;
    double a3 = 1.421413741;
    double a4 = -1.453152027;
    double a5 = 1.061405429;
    double p = 0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x) / sqrt(2.0);

    // A&S formula 7.1.26
    double t = 1.0 / (1.0 + p * x);
    double y = 1.0
            - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t
                    * exp(-x * x);

    return 0.5 * (1.0 + sign * y);
}

double MDMLib::boundedGaussianSum(double x1, double x2, double sig) {
    return gaussianSum(x2, sig) - gaussianSum(x1, sig);
}

double MDMLib::intersectionFactor(double conf1, double conf2, double d,
        double initRadius) {
    double interDistance = 3.1;

    double r1 = conf1 + interDistance * 0.5;
    double r2 = conf2 + interDistance * 0.5;

    double s1 = 0;
    double e1 = 0;
    double s2 = 0;
    double e2 = 0;
    double overlap = r2 + r1 - d;

    if (overlap < 0) {
        return 0;
    }

    if (r2 > r1) {
        if (overlap < 2 * r1) {
            s1 = -r1;
            e1 = -r1 + overlap;

            s2 = -r2;
            e2 = -r2 + overlap;
        } else {
            s1 = -r1;
            e1 = r1;
            s2 = -(d + r1);
            e2 = -(d - r1);
        }
    } else {
        if (overlap < 2 * r2) {
            s2 = -r2;
            e2 = -r2 + overlap;

            s1 = -r1;
            e1 = -r1 + overlap;
        } else {
            s2 = -r2;
            e2 = r2;

            s1 = -(d + r2);
            e1 = -(d - r2);
        }
    }

    double sig1 = r1 / 3;
    double sig2 = r2 / 3;

    double factor1 = 0;
    double factor2 = 0;

    if (s1 == -r1) {
        if (e1 == r1) {
            factor1 = 1;
        } else {
            factor1 = gaussianSum(e1, sig1);
        }
    } else {
        factor1 = boundedGaussianSum(s1, e1, sig1);
    }
    if (s2 == -r2) {
        if (e2 == r2) {
            factor2 = 1;

        } else {
            factor2 = gaussianSum(e2, sig2);
        }
    } else {
        factor2 = boundedGaussianSum(s2, e2, sig2);
    }

    factor1 = (factor1) / ((e1 - s1) / (2 * r1));
    factor2 = (factor2) / ((e2 - s2) / (2 * r2));

    double areaIntersection = calculateCircleCircleIntersection(r1, r2, d);

    double nbrCirles1 = calculateCircles(r1 * 2, interDistance);
    double nbrCirles2 = calculateCircles(r2 * 2, interDistance);
    double circleInt1 = (areaIntersection / (PI * r1 * r1)) * nbrCirles1;
    double circleInt2 = (areaIntersection / (PI * r2 * r2)) * nbrCirles2;
    double intCircles = (circleInt1 + circleInt2) / 2;

    double areaFactor1 = areaIntersection / (PI * r1 * r1);
    double areaFactor2 = areaIntersection / (PI * r2 * r2);

    double factor = factor1 * areaFactor1 * factor2 * areaFactor2
            / (intCircles);

    return factor;
}

double MDMLib::RectRectFactor(Coord c1, Coord c2, double heading1,
        double heading2, Coord size1, Coord size2) {

    RectIntLib rit;
    double intArea = rit.RectIntArea(c1.x, c1.y, size1.x, size1.y, heading1,
            c2.x, c2.y, size2.x, size2.y, heading2);

    return intArea / (size2.x * size2.y);
}
