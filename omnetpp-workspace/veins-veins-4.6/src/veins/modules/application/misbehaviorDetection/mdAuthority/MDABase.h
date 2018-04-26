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

#ifndef __VEINS_MDABase_H_
#define __VEINS_MDABase_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;

#define TOTCARNUM 10000


class MDABase {
private:

    char name[32];

    bool init = true;

    int totalFaultyIdList[TOTCARNUM];
    double totalFaultyTimeList[TOTCARNUM];
    int totalFaultyNum = 0;

    int totalAttackerIdList[TOTCARNUM];
    double totalAttackerTimeList[TOTCARNUM];
    int totalAttackerNum = 0;

    int reportedAttackerIdList[TOTCARNUM];
    double reportedAttackerTimeList[TOTCARNUM];
    int reportedAttackerNum = 0;

    int reportedFaultyIdList[TOTCARNUM];
    double reportedFaultyTimeList[TOTCARNUM];
    int reportedFaultyNum = 0;

    double faultyAverageReportDelay = 0;
    double attackerAverageReportDelay = 0;

public:
    MDABase();
    MDABase(const char *);
    void addTotalFaulty(int id, double time);
    void addReportedFaulty(int id, double time);

    void addTotalAttacker(int id, double time);
    void addReportedAttacker(int id, double time);

    void getTotalFaulty(int id);
    void getReportedFaultyIdList(int id);

    void getTotalAttackerIdList(int id);
    void getReportedAttackerIdList(int id);

    bool alreadyReportedFaulty(int id);
    bool alreadyReportedAttacker(int id);

    int totalFaultyIndex(int id);
    int totalAttackerIndex(int id);

    void getPrintable(char* outStr, double time);
    void writeFile(std::string path, char* printStr);

    char* getName();

    void setName(const char*);

    void resetAll();



};

#endif
