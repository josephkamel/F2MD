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

#ifndef __VEINS_MDMHistory_H_
#define __VEINS_MDMHistory_H_

#include <omnetpp.h>
#include "BsmCheck.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;
#include "../supportClasses/MDMLib.h"

class MDMHistory {
    private:
    unsigned long nodePseudonym;
        int BSMNumV1;
        int BSMNumV2;
        BsmCheck bsmCheckListV1[MAXMDMLENGTH];
        BsmCheck bsmCheckListV2[MAXMDMLENGTH];

        void addBsmCheck(BsmCheck bsmCheckV1, BsmCheck bsmCheckV2);
        void setBsmCheck(int index, BsmCheck bsmCheckV1,BsmCheck bsmCheckV2);

    public:
        MDMHistory();
        MDMHistory(unsigned long);
        int getMDMNum();

        BsmCheck getBsmCheck(int index, int version);

        void addBsmCheck(BsmCheck bsmCheck, int version);

        void setBsmCheck(int index, BsmCheck bsmCheck, int version);
    };

#endif
