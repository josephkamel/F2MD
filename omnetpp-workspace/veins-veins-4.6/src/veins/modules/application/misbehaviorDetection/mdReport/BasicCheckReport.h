/*******************************************************************************
* @author  Joseph Kamel 
* @email   josephekamel@gmail.com
* @date    28/11/2018
* @version 1.1
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_BasicCheckReport_H_
#define __VEINS_BasicCheckReport_H_

#include <omnetpp.h>
#include <veins/modules/application/misbehaviorDetection/mdReport/MDReport.h>
#include <veins/modules/messages/BasicSafetyMessage_m.h>

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "../supportClasses/XmlWriter.h"
#include "ReportPrintable.h"


using namespace omnetpp;

class BasicCheckReport: public MDReport {

    private:
        BsmCheck reportedCheck;

    public:
        BasicCheckReport(MDReport baseReport);
        void setReportedCheck(BsmCheck reportedCheck);
        std::string getReportPrintableXml();
        std::string getReportPrintableJson();

    };

#endif
