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

#include "BehavioralApp.h"

using namespace std;
using namespace boost;

BehavioralApp::BehavioralApp(const char* name, int version, double deltaTrustTime, double Threshold) :
        MDApplication(name) {
    this->version = version;
    this->deltaTrustTime = deltaTrustTime;
    this->Threshold = Threshold;
}

bool BehavioralApp::CheckNodeForReport(unsigned long myPseudonym,
        BasicSafetyMessage bsm, BsmCheck bsmCheck, NodeTable detectedNodes) {

    MDReport mbReport;

    double tempFactor = 0;
    minFactor = 1;

    prntApp.incAll(mbTypes::intMbs[bsm.getSenderMbType()]);
    prntAppInst.incAll(mbTypes::intMbs[bsm.getSenderMbType()]);

    int senderId = bsm.getSenderPseudonym();

    int bsmCheckListSize = 0;

    double factorList[bsmCheckListSize];

    //std::cout<< "RangePlausibility" << '\n';

    tempFactor = bsmCheck.getRangePlausibility();
    if(tempFactor<minFactor){
        minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::RangePlausibility, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::RangePlausibility, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    //std::cout<< "PositionConsistancy" << '\n';
    tempFactor = bsmCheck.getPositionConsistancy();
    if(tempFactor<minFactor){
        minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::PositionConsistancy, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::PositionConsistancy, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    //std::cout<< "PositionSpeedConsistancy" << '\n';

    tempFactor = bsmCheck.getPositionSpeedConsistancy();
    if(tempFactor<minFactor){
        minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::PositionSpeedConsistancy, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::PositionSpeedConsistancy, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    //std::cout<< "SpeedConsistancy" << '\n';

    tempFactor = bsmCheck.getSpeedConsistancy();
    if(tempFactor<minFactor){
        minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::SpeedConsistancy, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::SpeedConsistancy, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    //std::cout<< "SpeedPlausibility" << '\n';

    tempFactor = bsmCheck.getSpeedPlausibility();
    if(tempFactor<minFactor){
        minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::SpeedPlausibility, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::SpeedPlausibility, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    //std::cout<< "PositionPlausibility" << '\n';

    tempFactor = bsmCheck.getPositionPlausibility();
    if(tempFactor<minFactor){
        minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::PositionPlausibility, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::PositionPlausibility, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    //std::cout<< "BeaconFrequency" << '\n';

    tempFactor = bsmCheck.getBeaconFrequency();
    if(tempFactor<minFactor){
        minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::BeaconFrequency, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::BeaconFrequency, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    //std::cout<< "SuddenAppearence" << '\n';
    tempFactor = bsmCheck.getSuddenAppearence();
    if(tempFactor<minFactor){
        //std::cout<< "SuddenAppearence" << '\n';
   //     minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::SuddenAppearence, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::SuddenAppearence, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    //std::cout<< "PositionHeadingConsistancy" << '\n';

    tempFactor = bsmCheck.getPositionHeadingConsistancy();
    if(tempFactor<minFactor){
        minFactor = tempFactor;
    }
    if (tempFactor<Threshold) {
        prntApp.incFlags(mdChecksTypes::PositionHeadingConsistancy, mbTypes::intMbs[bsm.getSenderMbType()]);
        prntAppInst.incFlags(mdChecksTypes::PositionHeadingConsistancy, mbTypes::intMbs[bsm.getSenderMbType()]);
    }

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        double curInferFactor = inter.getInterValue(var);
        //std::cout<< "Intersection" << '\n';
        tempFactor = curInferFactor;
        if(tempFactor<minFactor){
            minFactor = tempFactor;
        }
        if (tempFactor<Threshold) {
            prntApp.incFlags(mdChecksTypes::Intersection, mbTypes::intMbs[bsm.getSenderMbType()]);
            prntAppInst.incFlags(mdChecksTypes::Intersection, mbTypes::intMbs[bsm.getSenderMbType()]);
        }
    }

    bool checkFailed = false;

    int indexTMO = getIndexTMO(senderId);

    if (minFactor<Threshold) {

        int TMOadd = 10*(Threshold - minFactor);
        //TMOadd = 1;

        if(indexTMO == -1){
            TimeOut[TimeOutNum] = TMOadd;
            PseudonymsToTMO[TimeOutNum] = senderId;
            indexTMO = TimeOutNum;
            TimeOutNum ++;
        }else{
            TimeOut[indexTMO] = TimeOut[indexTMO] + TMOadd;
        }

        //std::cout<<version<<"=>"<<10*(Threshold - minFactor)<<":"<< TimeOut[indexTMO]<<"\n";
    }

    if(indexTMO>=0){
        if(TimeOut[indexTMO]>0){
            prntApp.incCumulFlags(mbTypes::intMbs[bsm.getSenderMbType()]);
            prntAppInst.incCumulFlags(mbTypes::intMbs[bsm.getSenderMbType()]);
            bsmCheck.setReported(true);
            checkFailed = true;



            TimeOut[indexTMO] = TimeOut[indexTMO] -1;
        }
        if(TimeOut[indexTMO] == 0){
            removePseudoTMO(indexTMO);
        }
    }
    return checkFailed;
}

void BehavioralApp::removePseudoTMO(int index){
    for (int var = index; var < TimeOutNum; ++var) {
             TimeOut[var] = TimeOut[var + 1];
    }
    TimeOutNum --;
}

int BehavioralApp::getIndexTMO(unsigned long pseudo){
    for (int var = 0; var < TimeOutNum; ++var) {
        if(PseudonymsToTMO[var] == pseudo){
            return var;
        }
    }
    return -1;
}

double BehavioralApp::getMinFactor(){
    return minFactor;
}

