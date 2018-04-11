/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_MBReport_H_
#define __VEINS_MBReport_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "../baseClasses/BsmCheck.h"

using namespace omnetpp;

class MBReport {

    private:
        double generationTime;
        int senderId;
        int reportedId;
        double mbType;
//        std::string failedCheckList[];
//        int failedCheckListNum;
//        BsmCheck reportedBsmList[];
//        int bsmListNum;

    public:
        MBReport();

        void setGenerationTime(double time);
        void setSenderId(int id);
        void setReportedId(int id);
        void setMbType(double type);
//        void addFailedCheck(std::string check);
//        void addBsmCheck(BsmCheck bsmCheck);

        double getGenerationTime();
        int getSenderId();
        int getReportedId();
        double getMbType();

    };

#endif
