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

#ifndef __VEINS_MDAuthority_H_
#define __VEINS_MDAuthority_H_
#include <string.h>
#include <omnetpp.h>
#include "MBReport.h"
#include "MDABase.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"


using namespace omnetpp;

#define ABaseNbr 2

class MDAuthority {
private:

    MDABase baseList[ABaseNbr];

    int baseListNum = 0;
    void treatReport(MDABase* base, int index, MBReport report);


public:
    MDAuthority();

    void registerNewBase(char* baseName);

    void addNewNode(int id, double mbType, double time);
    void addReportedNode(int id, double mbType, double time);
    void sendReport(char* baseName, MBReport report);

    void saveLine(std::string path, std::string serial,double time);

    void resetAll();

};

#endif
