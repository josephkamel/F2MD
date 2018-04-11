#ifndef __VEINS_GeneralLib_H_
#define __VEINS_GeneralLib_H_

#include <omnetpp.h>
#include <string>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace std;
class GeneralLib {
public:
    GeneralLib();
    Coord TypeToSize(std::string type);
    double RandomDouble(double fMin, double fMax);
    int RandomInt(int min, int max);
};

#endif
