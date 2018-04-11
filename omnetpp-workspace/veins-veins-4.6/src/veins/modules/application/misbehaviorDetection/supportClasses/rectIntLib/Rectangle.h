/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_RectangleLib_H_
#define __VEINS_RectangleLib_H_

using namespace std;

class Rectangle {
public:
    Rectangle(double, double, double, double, double);
    double cx;
    double cy;
    double w;
    double h;
    double r;
};

#endif
