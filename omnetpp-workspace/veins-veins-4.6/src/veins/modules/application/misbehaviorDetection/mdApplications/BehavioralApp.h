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
#include "../enumTypes/MdChecksTypes.h"
#include "../enumTypes/MbTypes.h"

using namespace Veins;
using namespace omnetpp;

#define MAX_DETECTED_NODES 100

class BehavioralApp: public MDApplication {
public:

    int version = 0;
    double Threshold = 0.5;
    double deltaTrustTime = 10;
    double minFactor = 1;

    unsigned long PseudonymsToTMO[MAX_DETECTED_NODES];
    unsigned long TimeOut[MAX_DETECTED_NODES];
    int TimeOutNum = 0;

    BehavioralApp(const char* name, int version, double BehavioralApp,
            double Threshold);

    bool CheckNodeForReport(unsigned long myPseudonym,
            BasicSafetyMessage bsm, BsmCheck bsmCheck, NodeTable detectedNodes);


    void removePseudoTMO(int index);
    int getIndexTMO(unsigned long pseudo);

    double getMinFactor();
};

#endif
