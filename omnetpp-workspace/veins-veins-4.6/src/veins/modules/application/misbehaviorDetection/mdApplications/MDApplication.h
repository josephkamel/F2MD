/*******************************************************************************
 * @author  Joseph Kamel 
 * @email   josephekamel@gmail.com
 * @date    28/11/2018
 * @version 2.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/

#ifndef __VEINS_MDApplication_H_
#define __VEINS_MDApplication_H_

#include <tuple>
#include <omnetpp.h>
#include <veins/modules/application/misbehaviorDetection/baseClasses/NodeMDMHistory.h>
#include <veins/modules/application/misbehaviorDetection/mdStats/MDStatistics.h>
#include "../baseClasses/NodeTable.h"
#include "../baseClasses/InterTest.h"
#include "../baseClasses/BsmCheck.h"
#include "../baseClasses/InterTest.h"
#include "../supportClasses/MDMLib.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "../mdReport/MDReport.h"
#include "../supportClasses/AppPrintable.h"

using namespace Veins;
using namespace omnetpp;


static AppPrintable prntAppV1;
static AppPrintable prntAppInstV1;

static AppPrintable prntAppV2;
static AppPrintable prntAppInstV2;


class MDApplication {

protected:
    MDMLib mdmLib;

    int version;

    char const *AppV1Name = "AppV1";
    char const *AppV2Name = "AppV2";

    AppPrintable* prntApp;
    AppPrintable* prntAppInst;
public:

    MDApplication(int version);

    virtual bool CheckNodeForReport(unsigned long myPseudonym,
            BasicSafetyMessage * bsm, BsmCheck bsmCheck,
            NodeTable * detectedNodes)= 0;

    virtual double getMinFactor()= 0;

    void SendReport( MDStatistics* mdAuthority, MDReport mbReport);

    void saveLine( std::string path, std::string serial, double density,
            double deltaT, bool printOut);
    void resetInstFlags();
    void resetAllFlags();
};



#endif
