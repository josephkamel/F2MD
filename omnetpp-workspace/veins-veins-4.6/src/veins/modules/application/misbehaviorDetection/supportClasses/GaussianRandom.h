/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_GaussianRandom_H_
#define __VEINS_GaussianRandom_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

using namespace omnetpp;

class GaussianRandom {
    private:

    double confidenceRadius;
    Coord curSpeedConfidence;

    double getRand(double mean, double stddev);

    public:
        GaussianRandom(double confidenceRadius, Coord curSpeedConfidence);
        Coord OffsetPosition(Coord curPosition);
        Coord OffsetSpeed(Coord curSpeed);

    };

#endif
