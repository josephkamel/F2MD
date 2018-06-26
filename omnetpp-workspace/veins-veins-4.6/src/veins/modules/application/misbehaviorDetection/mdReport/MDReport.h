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

#ifndef __VEINS_MDReport_H_
#define __VEINS_MDReport_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "../baseClasses/BsmCheck.h"
#include "../supportClasses/XmlWriter.h"

using namespace omnetpp;


class MDReport {

protected:
    double generationTime;
    int senderId;
    int reportedId;
    std::string mbType;
    std::string attackType;

public:
    MDReport();

    void setGenerationTime(double time);
    void setSenderId(int id);
    void setReportedId(int id);
    void setMbType(std::string type);
    void setAttackType(std::string type);

    double getGenerationTime();
    int getSenderId();
    int getReportedId();
    std::string getMbType();
    std::string getAttackType();

    std::string getBaseReportXml();
    bool writeStrToFile(const std::string strFileCnst, const std::string serial,
            const std::string version, const std::string outStr);

};

#endif
