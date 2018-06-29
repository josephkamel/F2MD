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

#ifndef __VEINS_BehavioralApp_H_
#define __VEINS_BehavioralApp_H_

#include <tuple>
#include <omnetpp.h>

#include "MDApplication.h"

using namespace Veins;
using namespace omnetpp;

class BehavioralApp: public MDApplication {
public:

    int version = 0;

    double Threshold = 0.5;
    double deltaTrustTime = 10;
    int maxBsmTrustNum = 5;
    double Augmentation = 1;
    double minFactor = 1;


    BehavioralApp(const char* name, int version, double deltaTrustTime,
            int maxBsmTrustNum, double Augmentation);

    bool CheckNodeForReport(int myId,
            BasicSafetyMessage bsm, BsmCheck bsmCheck, NodeTable detectedNodes);

//    bool AggregateFactorsList(double curFactor, double *factorList,
//            int factorListSize, double zeroSum, int zeroCount);
    double AggregateFactorsListDouble(double curFactor, double *factorList,
            int factorListSize, double zeroSum, int zeroCount);

    std::tuple<double, int> getZeroNumber(BasicSafetyMessage bsm, BsmCheck bsmCheck,
            NodeTable detectedNodes);

    double getMinFactor();
};

#endif
