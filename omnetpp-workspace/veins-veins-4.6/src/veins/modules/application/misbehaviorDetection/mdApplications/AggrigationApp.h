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
#include "../enumTypes/MdChecksTypes.h"
#include "../enumTypes/MbTypes.h"

using namespace Veins;
using namespace omnetpp;

class AggrigationApp: public MDApplication {
public:

    double Threshold = 0.5;
    double devValue = 0.5;
    double deltaTrustTime = 10;
    int maxBsmTrustNum = 5;
    double minFactor = 1;



    AggrigationApp(int version, double devValue ,double deltaTrustTime,
            int maxBsmTrustNum);

    bool CheckNodeForReport(unsigned long myPseudonym,
            BasicSafetyMessage * bsm, BsmCheck bsmCheck, NodeTable * detectedNodes);

    double AggregateFactorsListDouble(double curFactor, double *factorList,
            int factorListSize);

    double getMinFactor();
};

#endif
