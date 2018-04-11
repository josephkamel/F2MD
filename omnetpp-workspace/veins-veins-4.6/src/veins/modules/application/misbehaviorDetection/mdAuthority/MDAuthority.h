#ifndef __VEINS_MDAuthority_H_
#define __VEINS_MDAuthority_H_

#include <omnetpp.h>
#include "MBReport.h"
#include "MDABase.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"


using namespace omnetpp;

class MDAuthority {
private:

    void treatReportV1(MBReport report);
    void treatReportV2(MBReport report);


public:
    MDAuthority();

    void addNewNode(int id, double mbType, double time);

    void sendReportV1(MBReport report);
    void sendReportV2(MBReport report);

    void saveLine(std::string path, std::string serial,double time);
};

#endif
