/*******************************************************************************
 * @author  Joseph Kamel 
 * @email   josephekamel@gmail.com
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
    double Threshold = 0.5;
    double deltaTrustTime = 10;
    int maxBsmTrustNum = 5;
    int minFactor = 1;

    AggrigationApp(const char* name, int version, double deltaTrustTime,
            int maxBsmTrustNum);

    std::tuple<double, MDReport> CheckNodeForReport(int myId,
            BasicSafetyMessage bsm, BsmCheck bsmCheck, NodeTable detectedNodes);

//    bool AggregateFactorsList(double curFactor, double *factorList,
//            int factorListSize);
    double AggregateFactorsListDouble(double curFactor, double *factorList,
            int factorListSize);

    double getMinFactor();
};

#endif
