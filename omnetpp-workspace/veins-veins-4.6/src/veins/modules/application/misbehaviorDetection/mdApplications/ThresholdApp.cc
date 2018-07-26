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

    prntApp.incAll(bsm.getSenderMbType());
    prntAppInst.incAll(bsm.getSenderMbType());

    int senderId = bsm.getSenderPseudonym();

    if(bsmCheck.getRangePlausibility()<minFactor){
        minFactor = bsmCheck.getRangePlausibility();
    }
    if (bsmCheck.getRangePlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("RangePlausibility", bsm.getSenderMbType());
        prntAppInst.incFlags("RangePlausibility", bsm.getSenderMbType());
    }

    if(bsmCheck.getRangePlausibility()<minFactor){
        minFactor = bsmCheck.getRangePlausibility();
    }
    if (bsmCheck.getPositionConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionConsistancy", bsm.getSenderMbType());
        prntAppInst.incFlags("PositionConsistancy", bsm.getSenderMbType());
    }


    if(bsmCheck.getPositionSpeedConsistancy()<minFactor){
        minFactor = bsmCheck.getPositionSpeedConsistancy();
    }
    if (bsmCheck.getPositionSpeedConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionSpeedConsistancy", bsm.getSenderMbType());
        prntAppInst.incFlags("PositionSpeedConsistancy", bsm.getSenderMbType());
    }

    if(bsmCheck.getSpeedConsistancy()<minFactor){
        minFactor = bsmCheck.getSpeedConsistancy();
    }
    if (bsmCheck.getSpeedConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("SpeedConsistancy", bsm.getSenderMbType());
        prntAppInst.incFlags("SpeedConsistancy", bsm.getSenderMbType());
    }

    if(bsmCheck.getSpeedPlausibility()<minFactor){
        minFactor = bsmCheck.getSpeedPlausibility();
    }
    if (bsmCheck.getSpeedPlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("SpeedPlausibility", bsm.getSenderMbType());
        prntAppInst.incFlags("SpeedPlausibility", bsm.getSenderMbType());
    }

    if(bsmCheck.getPositionPlausibility()<minFactor){
        minFactor = bsmCheck.getPositionPlausibility();
    }
    if (bsmCheck.getPositionPlausibility() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionPlausibility", bsm.getSenderMbType());
        prntAppInst.incFlags("PositionPlausibility", bsm.getSenderMbType());
    }

    if(bsmCheck.getBeaconFrequency()<minFactor){
        minFactor = bsmCheck.getBeaconFrequency();
    }
    if (bsmCheck.getBeaconFrequency() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("BeaconFrequency", bsm.getSenderMbType());
        prntAppInst.incFlags("BeaconFrequency", bsm.getSenderMbType());
    }

    if(bsmCheck.getSuddenAppearence()<minFactor){
   //     minFactor = bsmCheck.getSuddenAppearence();
    }
    if (bsmCheck.getSuddenAppearence() <= Threshold) {
   //     checkFailed = true;
        prntApp.incFlags("SuddenAppearence", bsm.getSenderMbType());
        prntAppInst.incFlags("SuddenAppearence", bsm.getSenderMbType());
    }

    if( bsmCheck.getPositionHeadingConsistancy()<minFactor){
        minFactor = bsmCheck.getPositionHeadingConsistancy();
    }
    if (bsmCheck.getPositionHeadingConsistancy() <= Threshold) {
        checkFailed = true;
        prntApp.incFlags("PositionHeadingConsistancy", bsm.getSenderMbType());
        prntAppInst.incFlags("PositionHeadingConsistancy", bsm.getSenderMbType());
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
                prntApp.incFlags("Intersection", bsm.getSenderMbType());
                prntAppInst.incFlags("Intersection", bsm.getSenderMbType());
                minInterFound = true;
            }
        }

    }
    if (checkFailed) {
        prntApp.incCumulFlags(bsm.getSenderMbType());
        prntAppInst.incCumulFlags(bsm.getSenderMbType());
    }

    return checkFailed;
}

double ThresholdApp::getMinFactor(){
    return minFactor;
}
