/*******************************************************************************
 * @author  Joseph Kamel 
* @email   joseph.kamel@gmail.com
 * @date    11/04/2018
 * @version 1.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/

#ifndef __VEINS_ThresholdApp_H_
#define __VEINS_ThresholdApp_H_

#include <tuple>
#include <omnetpp.h>

#include "MDApplication.h"

using namespace Veins;
using namespace omnetpp;

class ThresholdApp: public MDApplication {
public:

    double Threshold = 0;

    ThresholdApp(const char* name, double Threshold);

    std::tuple<double, MBReport> CheckNodeForReport(int myId,BasicSafetyMessage bsm,
            BsmCheck bsmCheck, NodeTable detectedNodes, double mbType);
};

#endif
