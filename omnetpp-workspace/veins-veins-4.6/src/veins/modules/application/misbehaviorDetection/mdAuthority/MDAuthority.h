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

#ifndef __VEINS_MDAuthority_H_
#define __VEINS_MDAuthority_H_

#include "MDABase.h"
#include "../mdReport/MDReport.h"

using namespace omnetpp;

#define ABaseNbr 2

class MDAuthority {
private:

    MDABase baseList[ABaseNbr];

    int baseListNum = 0;
    void treatReport(MDABase* base, int index, MDReport report);


public:
    MDAuthority();

    void registerNewBase(char* baseName);

    void addNewNode(unsigned long pseudo, mbTypes::Mbs mbType, double time);
    void addReportedNode(unsigned long pseudo, mbTypes::Mbs mbType, double time);
    void sendReport(const char* baseName, MDReport report);

    void saveLine(std::string path, std::string serial,double time);

    void resetAll();

};

#endif
