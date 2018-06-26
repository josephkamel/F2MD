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

#ifndef __VEINS_ReportPrintable_H_
#define __VEINS_ReportPrintable_H_

#include <omnetpp.h>
#include <veins/modules/application/misbehaviorDetection/mdReport/MDReport.h>
#include <veins/modules/messages/BasicSafetyMessage_m.h>

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "../supportClasses/XmlWriter.h"

using namespace omnetpp;

class ReportPrintable {

private:

public:

    std::string getCheckXml(BsmCheck Check);
    std::string getBsmXml(BasicSafetyMessage bsm);

};

#endif
