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

#ifndef __VEINS_MDApplication_H_
#define __VEINS_MDApplication_H_


#include <tuple>
#include <omnetpp.h>
#include <veins/modules/application/misbehaviorDetection/baseClasses/NodeMDMHistory.h>

#include "../baseClasses/NodeTable.h"
#include "../baseClasses/InterTest.h"
#include "../baseClasses/BsmCheck.h"
#include "../baseClasses/InterTest.h"
#include "../supportClasses/MDMLib.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "../mdAuthority/MBReport.h"
#include "../mdAuthority/MDAuthority.h"

#include "../supportClasses/Printable.h"

using namespace Veins;
using namespace omnetpp;

class MDApplication {
protected:
    MDMLib mdmLib;

    char appName[32];

    Printable prntApp;
    Printable prntAppInst;

public:

    MDApplication(const char* name);
    void setName(const char* name);

    void SendReport(MDAuthority* mdAuthority, MBReport mbReport);

    void saveLine(std::string path, std::string serial, double density,
            double deltaT);
    void resetInstFlags();
    void resetAllFlags();

};

#endif
