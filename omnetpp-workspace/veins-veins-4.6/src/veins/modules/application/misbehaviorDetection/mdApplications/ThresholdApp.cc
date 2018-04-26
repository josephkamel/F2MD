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

#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>

#include "ThresholdApp.h"

using namespace std;
using namespace boost;


ThresholdApp::ThresholdApp(const char* name):MDApplication(name){

}

std::tuple<double, MBReport> ThresholdApp::CheckNodeForReport(int myId,BasicSafetyMessage bsm,
               BsmCheck bsmCheck, NodeTable detectedNodes, double mbType, double Threshold){

    bool checkFailed = false;

    MBReport mbReport;

    prntApp.incAll(mbType);
    prntAppInst.incAll(mbType);

    int senderId = bsm.getSenderAddress();

    if (bsmCheck.getRangePlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("RangePlausibility", mbType);
        prntAppInst.incFlags("RangePlausibility", mbType);
    }

    if (bsmCheck.getPositionConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionConsistancy", mbType);
        prntAppInst.incFlags("PositionConsistancy", mbType);
    }

    if (bsmCheck.getPositionSpeedConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionSpeedConsistancy", mbType);
        prntAppInst.incFlags("PositionSpeedConsistancy", mbType);
    }

    if (bsmCheck.getSpeedConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("SpeedConsistancy", mbType);
        prntAppInst.incFlags("SpeedConsistancy", mbType);
    }

    if (bsmCheck.getSpeedPlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("SpeedPlausibility", mbType);
        prntAppInst.incFlags("SpeedPlausibility", mbType);
    }

    if (bsmCheck.getPositionPlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionPlausibility", mbType);
        prntAppInst.incFlags("PositionPlausibility", mbType);
    }

    if (bsmCheck.getBeaconFrequency() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("BeaconFrequency", mbType);
        prntAppInst.incFlags("BeaconFrequency", mbType);
    }

    if (bsmCheck.getSuddenAppearence() <= Threshold) {
        prntApp.incFlags("SuddenAppearence", mbType);
        prntAppInst.incFlags("SuddenAppearence", mbType);
    }

    if (bsmCheck.getPositionHeadingConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionHeadingConsistancy", mbType);
        prntAppInst.incFlags("PositionHeadingConsistancy", mbType);
    }

    bool maxInterFound = false;
    bool minInterFound = false;

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {

        double IT = inter.getInterValue(var);

        if (IT <= Threshold) {
            checkFailed = true;
            if (!minInterFound) {
                prntApp.incFlags("Intersection", mbType);
                prntAppInst.incFlags("Intersection", mbType);
                minInterFound = true;
            }
        }

    }
    if (checkFailed) {
        mbReport.setGenerationTime(simTime().dbl());
        mbReport.setSenderId(myId);
        mbReport.setReportedId(senderId);
        mbReport.setMbType(mbType);

        prntApp.incCumulFlags(mbType);
        prntAppInst.incCumulFlags(mbType);
    }

    return std::make_tuple(checkFailed, mbReport);
}
