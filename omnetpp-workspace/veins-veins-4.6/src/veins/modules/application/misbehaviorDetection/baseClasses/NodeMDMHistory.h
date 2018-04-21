/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_MDMHistory_H_
#define __VEINS_MDMHistory_H_

#include <omnetpp.h>
#include "BsmCheck.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;
#include "../supportClasses/MDMLib.h"

class MDMHistory {
    private:
        int nodeId;
        int BSMNum;
        BsmCheck bsmCheckList[MAXMDMLENGTH];

    public:
        MDMHistory();
        MDMHistory(int);
        int getMDMNum();
        BsmCheck getBsmCheck(int);
        void addBsmCheck(BsmCheck bsmCheck);
        void setBsmCheck(int index, BsmCheck bsmCheck);
    };

#endif
