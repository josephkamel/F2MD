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

#ifndef __VEINS_AggrigationApp_H_
#define __VEINS_AggrigationApp_H_

#include <tuple>
#include <omnetpp.h>

#include "MDApplication.h"

using namespace Veins;
using namespace omnetpp;

class AggrigationApp: public MDApplication {
public:

    int version = 0;

    AggrigationApp(const char* name, int version);

    std::tuple<double, MBReport> CheckNodeForReport(int myId,BasicSafetyMessage bsm,
            BsmCheck bsmCheck, NodeTable detectedNodes, double mbType);

    bool AggregateFactorsList(double curFactor, double *factorList,
            int factorListSize);
};

#endif
