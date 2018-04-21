/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_NODEHISTORY_H_
#define __VEINS_NODEHISTORY_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "../supportClasses/MDMLib.h"

using namespace omnetpp;

class NodeHistory {
    private:
        int nodeId;
        int bsmNum;
        BasicSafetyMessage bsmList[MAXBSMLENGTH];
        BasicSafetyMessage* getBSMList();

    public:
        NodeHistory();
        NodeHistory(int);
        NodeHistory(int, BasicSafetyMessage);
        void addBSM(BasicSafetyMessage bsm);
        BasicSafetyMessage getLatestBSM();
        BasicSafetyMessage getSecondLatestBSM();
        BasicSafetyMessage getBSM(int index) ;

        Coord getSenderPos(int);
        Coord getSenderSize(int);
        double getSenderSpeed(int);
        Coord getSenderHeading(int);
        double getDeltaTime(int, int);
        int getBSMNum();
        double getArrivalTime(int);
    };

#endif
