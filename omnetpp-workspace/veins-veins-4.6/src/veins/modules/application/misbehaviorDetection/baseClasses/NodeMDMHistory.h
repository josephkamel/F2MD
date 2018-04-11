
#ifndef __VEINS_MDMHistory_H_
#define __VEINS_MDMHistory_H_

#include <omnetpp.h>
#include "BsmCheck.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;

#define MAXMDMLENGTH 3

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
