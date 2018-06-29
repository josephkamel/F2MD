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


ThresholdApp::ThresholdApp(const char* name, double Threshold):MDApplication(name){
    this->Threshold = Threshold;
}

bool ThresholdApp::CheckNodeForReport(int myId,BasicSafetyMessage bsm,
        BsmCheck bsmCheck, NodeTable detectedNodes){

    bool checkFailed = false;

    MDReport mbReport;

    minFactor = 1;

    prntApp.incAll(bsm.getSenderMbTypeStr());
    prntAppInst.incAll(bsm.getSenderMbTypeStr());

    int senderId = bsm.getSenderAddress();

    if(bsmCheck.getRangePlausibility()<minFactor){
        minFactor = bsmCheck.getRangePlausibility();
    }
    if (bsmCheck.getRangePlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("RangePlausibility", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("RangePlausibility", bsm.getSenderMbTypeStr());
    }

    if(bsmCheck.getRangePlausibility()<minFactor){
        minFactor = bsmCheck.getRangePlausibility();
    }
    if (bsmCheck.getPositionConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionConsistancy", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("PositionConsistancy", bsm.getSenderMbTypeStr());
    }


    if(bsmCheck.getPositionSpeedConsistancy()<minFactor){
        minFactor = bsmCheck.getPositionSpeedConsistancy();
    }
    if (bsmCheck.getPositionSpeedConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionSpeedConsistancy", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("PositionSpeedConsistancy", bsm.getSenderMbTypeStr());
    }

    if(bsmCheck.getSpeedConsistancy()<minFactor){
        minFactor = bsmCheck.getSpeedConsistancy();
    }
    if (bsmCheck.getSpeedConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("SpeedConsistancy", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("SpeedConsistancy", bsm.getSenderMbTypeStr());
    }

    if(bsmCheck.getSpeedPlausibility()<minFactor){
        minFactor = bsmCheck.getSpeedPlausibility();
    }
    if (bsmCheck.getSpeedPlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("SpeedPlausibility", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("SpeedPlausibility", bsm.getSenderMbTypeStr());
    }

    if(bsmCheck.getPositionPlausibility()<minFactor){
        minFactor = bsmCheck.getPositionPlausibility();
    }
    if (bsmCheck.getPositionPlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionPlausibility", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("PositionPlausibility", bsm.getSenderMbTypeStr());
    }

    if(bsmCheck.getBeaconFrequency()<minFactor){
        minFactor = bsmCheck.getBeaconFrequency();
    }
    if (bsmCheck.getBeaconFrequency() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("BeaconFrequency", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("BeaconFrequency", bsm.getSenderMbTypeStr());
    }

    if(bsmCheck.getSuddenAppearence()<minFactor){
   //     minFactor = bsmCheck.getSuddenAppearence();
    }
    if (bsmCheck.getSuddenAppearence() <= Threshold) {
   //     checkFailed = true;
        prntApp.incFlags("SuddenAppearence", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("SuddenAppearence", bsm.getSenderMbTypeStr());
    }

    if( bsmCheck.getPositionHeadingConsistancy()<minFactor){
        minFactor = bsmCheck.getPositionHeadingConsistancy();
    }
    if (bsmCheck.getPositionHeadingConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionHeadingConsistancy", bsm.getSenderMbTypeStr());
        prntAppInst.incFlags("PositionHeadingConsistancy", bsm.getSenderMbTypeStr());
    }

    bool maxInterFound = false;
    bool minInterFound = false;

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {

        double IT = inter.getInterValue(var);

        if(IT<minFactor){
            minFactor = IT;
        }

        if (IT <= Threshold) {
            checkFailed = true;
            if (!minInterFound) {
                prntApp.incFlags("Intersection", bsm.getSenderMbTypeStr());
                prntAppInst.incFlags("Intersection", bsm.getSenderMbTypeStr());
                minInterFound = true;
            }
        }

    }
    if (checkFailed) {
        prntApp.incCumulFlags(bsm.getSenderMbTypeStr());
        prntAppInst.incCumulFlags(bsm.getSenderMbTypeStr());
    }

    return checkFailed;
}

double ThresholdApp::getMinFactor(){
    return minFactor;
}
