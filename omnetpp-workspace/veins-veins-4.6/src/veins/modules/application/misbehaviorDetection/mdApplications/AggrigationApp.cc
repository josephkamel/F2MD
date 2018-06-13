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

#include "AggrigationApp.h"

using namespace std;
using namespace boost;

AggrigationApp::AggrigationApp(const char* name, int version, double deltaTrustTime, int maxBsmTrustNum) :
        MDApplication(name) {
    this->version = version;
    this->deltaTrustTime = deltaTrustTime;
    this->maxBsmTrustNum = maxBsmTrustNum;
}

std::tuple<double, MBReport> AggrigationApp::CheckNodeForReport(int myId,
        BasicSafetyMessage bsm, BsmCheck bsmCheck, NodeTable detectedNodes,
        double mbType) {

    bool checkFailed = false;
    MBReport mbReport;

    prntApp.incAll(mbType);
    prntAppInst.incAll(mbType);

    int senderId = bsm.getSenderAddress();

    MDMHistory mdmHist = detectedNodes.getMDMHistory(senderId);
    NodeHistory nodeHist = detectedNodes.getNodeHistory(senderId);

    BsmCheck bsmCheckList[maxBsmTrustNum];
    int bsmCheckListSize = 0;

    for (int var = 0; var < nodeHist.getBSMNum(); ++var) {
        if (bsmCheckListSize < maxBsmTrustNum) {
            if (mdmLib.calculateDeltaTime(bsm,
                    nodeHist.getBSM(var))<deltaTrustTime) {
                bsmCheckList[bsmCheckListSize] = mdmHist.getBsmCheck(var,
                        version);
                bsmCheckListSize++;
            }
        }
    }

    double factorList[bsmCheckListSize];

    //std::cout<< "RangePlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getRangePlausibility();
    }

    if (AggregateFactorsList(bsmCheck.getRangePlausibility(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("RangePlausibility", mbType);
        prntAppInst.incFlags("RangePlausibility", mbType);
    }

    //std::cout<< "PositionConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionConsistancy();
    }

    if (AggregateFactorsList(bsmCheck.getPositionConsistancy(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("PositionConsistancy", mbType);
        prntAppInst.incFlags("PositionConsistancy", mbType);
    }

    //std::cout<< "PositionSpeedConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionSpeedConsistancy();
    }

    if (AggregateFactorsList(bsmCheck.getPositionSpeedConsistancy(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("PositionSpeedConsistancy", mbType);
        prntAppInst.incFlags("PositionSpeedConsistancy", mbType);
    }

    //std::cout<< "SpeedConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSpeedConsistancy();
    }

    if (AggregateFactorsList(bsmCheck.getSpeedConsistancy(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("SpeedConsistancy", mbType);
        prntAppInst.incFlags("SpeedConsistancy", mbType);
    }

    //std::cout<< "SpeedPlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSpeedPlausibility();
    }

    if (AggregateFactorsList(bsmCheck.getSpeedPlausibility(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("SpeedPlausibility", mbType);
        prntAppInst.incFlags("SpeedPlausibility", mbType);
    }

    //std::cout<< "PositionPlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionPlausibility();
    }
    if (AggregateFactorsList(bsmCheck.getPositionPlausibility(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("PositionPlausibility", mbType);
        prntAppInst.incFlags("PositionPlausibility", mbType);
    }

    //std::cout<< "BeaconFrequency" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getBeaconFrequency();
    }

    if (AggregateFactorsList(bsmCheck.getBeaconFrequency(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("BeaconFrequency", mbType);
        prntAppInst.incFlags("BeaconFrequency", mbType);
    }

    //std::cout<< "SuddenAppearence" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSuddenAppearence();
    }

    if (AggregateFactorsList(bsmCheck.getSuddenAppearence(), factorList,
            bsmCheckListSize)) {
        prntApp.incFlags("SuddenAppearence", mbType);
        prntAppInst.incFlags("SuddenAppearence", mbType);
    }

    //std::cout<< "PositionHeadingConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionHeadingConsistancy();
    }

    if (AggregateFactorsList(bsmCheck.getPositionHeadingConsistancy(),
            factorList, bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("PositionHeadingConsistancy", mbType);
        prntAppInst.incFlags("PositionHeadingConsistancy", mbType);
    }

    for (int var = 0; var < bsmCheckListSize; ++var) {
        bsmCheckList[var].getIntersection();
    }

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        double curInferFactor = inter.getInterValue(var);

        for (int i = 0; i < bsmCheckListSize; ++i) {
            double IdIndex = bsmCheckList[i].getIntersection().getIdIndex(
                    inter.getInterId(var));
            if (IdIndex != -1) {
                factorList[i] = bsmCheckList[i].getIntersection().getInterValue(
                        IdIndex);
            } else {
                factorList[i] = 1;
            }
        }

        //std::cout<< "Intersection" << '\n';
        if (AggregateFactorsList(curInferFactor, factorList,
                bsmCheckListSize)) {
            checkFailed = true;
            prntApp.incFlags("Intersection", mbType);
            prntAppInst.incFlags("Intersection", mbType);
        }
    }

    if (checkFailed) {
        mbReport.setGenerationTime(simTime().dbl());
        mbReport.setSenderId(myId);
        mbReport.setReportedId(senderId);
        mbReport.setMbType(mbType);
//        SendReport(mbReport);

        prntApp.incCumulFlags(mbType);
        prntAppInst.incCumulFlags(mbType);

        bsmCheck.setReported(true);
    }
    return std::make_tuple(checkFailed, mbReport);
}


//fair chance
//bool AggrigationApp::AggregateFactorsList(double curFactor, double *factorList,
//        int factorListSize) {
//
//    double averageFactor = curFactor;
//    for (int var = 0; var < factorListSize; ++var) {
//        averageFactor = averageFactor + factorList[var];
//    }
//    averageFactor = averageFactor / (factorListSize + 1);
//    if ((averageFactor) <= 0.5) {
//        return true;
//    } else {
//        return false;
//    }
//}

//double AggrigationApp::AggregateFactorsListDouble(double curFactor, double *factorList,
//        int factorListSize) {
//    double averageFactor = curFactor;
//    for (int var = 0; var < factorListSize; ++var) {
//        averageFactor = averageFactor + factorList[var];
//    }
//    averageFactor = averageFactor / (factorListSize + 1);
//    return averageFactor;
//}

double AggrigationApp::AggregateFactorsListDouble(double curFactor, double *factorList,
        int factorListSize) {
    if (version == 1) {
        double averageFactor = curFactor;
        for (int var = 0; var < factorListSize; ++var) {
            averageFactor = averageFactor + factorList[var];
        }
        averageFactor = averageFactor / (factorListSize + 1);

        return averageFactor;
    } else {
        if (curFactor <= 0) {
            return 0;
        } else {
            double averageFactor = curFactor;
            for (int var = 0; var < factorListSize; ++var) {
                averageFactor = averageFactor + factorList[var];
            }
            averageFactor = averageFactor / (factorListSize + 1);
            return averageFactor;
        }
    }
}

//best rate / faulty
bool AggrigationApp::AggregateFactorsList(double curFactor, double *factorList,
        int factorListSize) {
    if (version == 1) {
        double averageFactor = curFactor;
        for (int var = 0; var < factorListSize; ++var) {
            averageFactor = averageFactor + factorList[var];
        }
        averageFactor = averageFactor / (factorListSize + 1);
        if ((averageFactor) <= 0.5) {
            return true;
        } else {
            return false;
        }
    } else {
        if (curFactor <= 0) {
            return true;
        } else {
            double averageFactor = curFactor;
            for (int var = 0; var < factorListSize; ++var) {
                averageFactor = averageFactor + factorList[var];
            }
            averageFactor = averageFactor / (factorListSize + 1);
            if ((averageFactor) <= 0.5) {
                return true;
            } else {
                return false;
            }
        }
    }
}

//moderate rate / faulty
//bool AggrigationApp::AggregateFactorsList(double curFactor, double *factorList,
//        int factorListSize) {
//
//    if (version == 1) {
//        double averageFactor = curFactor;
//        for (int var = 0; var < factorListSize; ++var) {
//            averageFactor = averageFactor + factorList[var];
//        }
//        if (factorListSize > 0) {
//            averageFactor = averageFactor / (factorListSize + 1);
//        }else{
//            averageFactor = 1;
//        }
//
//        if ((averageFactor) <= 0.5) {
//            return true;
//        } else {
//            return false;
//        }
//
//    } else {
//        if (curFactor <= 0) {
//            return true;
//        } else if (curFactor >= 1) {
//            return false;
//        } else {
//            double averageFactor = curFactor;
//            for (int var = 0; var < factorListSize; ++var) {
//                averageFactor = averageFactor + factorList[var];
//            }
//            if (factorListSize > 0) {
//                averageFactor = averageFactor / (factorListSize + 1);
//            }else{
//                averageFactor = 1;
//            }
//
//            if ((averageFactor) <= 0.5) {
//                return true;
//            } else {
//                return false;
//            }
//        }
//    }
//}

//SUPER RATE
//bool AggrigationApp::AggregateFactorsList(double curFactor, double *factorList,
//        int factorListSize) {
//
//    if (version == 1) {
//        double averageFactor = curFactor;
//        for (int var = 0; var < factorListSize; ++var) {
//            averageFactor = averageFactor + factorList[var];
//        }
//        if (factorListSize > 0) {
//            averageFactor = averageFactor / (factorListSize + 1);
//        }
//
//        if ((averageFactor) <= 0) {
//            return true;
//        } else {
//            return false;
//        }
//
//    } else {
//        if (curFactor <= 0) {
//            return true;
//        } else if (curFactor >= 1) {
//            return false;
//        } else {
//            double averageFactor = curFactor;
//            for (int var = 0; var < factorListSize; ++var) {
//                averageFactor = averageFactor + factorList[var];
//            }
//            if (factorListSize > 0) {
//                averageFactor = averageFactor / (factorListSize + 1);
//            }else{
//                averageFactor = 1;
//            AggrigationApp}
//
//            if ((averageFactor) <= 0.5) {
//                return true;
//            } else {
//                return false;
//            }
//        }
//    }
//}


double AggrigationApp::getMinFactor(int myId,BasicSafetyMessage bsm,
        BsmCheck bsmCheck, NodeTable detectedNodes, double mbType){

    double minFactor = 1;

    std::string minName = "";

    int senderId = bsm.getSenderAddress();

    MDMHistory mdmHist = detectedNodes.getMDMHistory(senderId);
    NodeHistory nodeHist = detectedNodes.getNodeHistory(senderId);

    BsmCheck bsmCheckList[maxBsmTrustNum];
    int bsmCheckListSize = 0;

    for (int var = 0; var < nodeHist.getBSMNum(); ++var) {
        if (bsmCheckListSize < maxBsmTrustNum) {
            if (mdmLib.calculateDeltaTime(bsm,
                    nodeHist.getBSM(var))<deltaTrustTime) {
                bsmCheckList[bsmCheckListSize] = mdmHist.getBsmCheck(var,
                        version);
                bsmCheckListSize++;
            }
        }
    }

    double factorList[bsmCheckListSize];

    //std::cout<< "RangePlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getRangePlausibility();
    }

    if (AggregateFactorsListDouble(bsmCheck.getRangePlausibility(), factorList,
            bsmCheckListSize) <= minFactor) {
        minName = "RangePlausibility";
        minFactor = AggregateFactorsListDouble(bsmCheck.getRangePlausibility(), factorList,
                bsmCheckListSize);
    }

    //std::cout<< "PositionConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionConsistancy();
    }

    if (AggregateFactorsListDouble(bsmCheck.getPositionConsistancy(), factorList,
            bsmCheckListSize)  <= minFactor) {
        minName = "PositionConsistancy";
        minFactor = AggregateFactorsListDouble(bsmCheck.getPositionConsistancy(), factorList,
                bsmCheckListSize);
    }

    //std::cout<< "PositionSpeedConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionSpeedConsistancy();
    }

    if (AggregateFactorsListDouble(bsmCheck.getPositionSpeedConsistancy(), factorList,
            bsmCheckListSize)<= minFactor) {
        minName = "PositionSpeedConsistancy";
        minFactor = AggregateFactorsListDouble(bsmCheck.getPositionSpeedConsistancy(), factorList,
                bsmCheckListSize);
    }

    //std::cout<< "SpeedConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSpeedConsistancy();
    }

    if (AggregateFactorsListDouble(bsmCheck.getSpeedConsistancy(), factorList,
            bsmCheckListSize)<= minFactor) {
        minName = "SpeedConsistancy";
        minFactor = AggregateFactorsListDouble(bsmCheck.getSpeedConsistancy(), factorList,
                bsmCheckListSize);
    }

    //std::cout<< "SpeedPlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSpeedPlausibility();
    }

    if (AggregateFactorsListDouble(bsmCheck.getSpeedPlausibility(), factorList,
            bsmCheckListSize)<= minFactor) {
        minName = "SpeedPlausibility";
        minFactor = AggregateFactorsListDouble(bsmCheck.getSpeedPlausibility(), factorList,
                bsmCheckListSize);
    }

    //std::cout<< "PositionPlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionPlausibility();
    }
    if (AggregateFactorsListDouble(bsmCheck.getPositionPlausibility(), factorList,
            bsmCheckListSize)<= minFactor) {
        minName = "PositionPlausibility";
        minFactor = AggregateFactorsListDouble(bsmCheck.getPositionPlausibility(), factorList,
                bsmCheckListSize);
    }

    //std::cout<< "BeaconFrequency" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getBeaconFrequency();
    }

    if (AggregateFactorsListDouble(bsmCheck.getBeaconFrequency(), factorList,
            bsmCheckListSize)<= minFactor) {
        minName = "BeaconFrequency";
        minFactor = AggregateFactorsListDouble(bsmCheck.getBeaconFrequency(), factorList,
                bsmCheckListSize);
    }

    //std::cout<< "PositionHeadingConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionHeadingConsistancy();
    }

    if (AggregateFactorsListDouble(bsmCheck.getPositionHeadingConsistancy(),
            factorList, bsmCheckListSize)<= minFactor) {
        minName = "PositionHeadingConsistancy";
        minFactor = AggregateFactorsListDouble(bsmCheck.getPositionHeadingConsistancy(),
                factorList, bsmCheckListSize);
    }

    for (int var = 0; var < bsmCheckListSize; ++var) {
        bsmCheckList[var].getIntersection();
    }

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        double curInferFactor = inter.getInterValue(var);

        for (int i = 0; i < bsmCheckListSize; ++i) {
            double IdIndex = bsmCheckList[i].getIntersection().getIdIndex(
                    inter.getInterId(var));
            if (IdIndex != -1) {
                factorList[i] = bsmCheckList[i].getIntersection().getInterValue(
                        IdIndex);
            } else {
                factorList[i] = 1;
            }
        }

        //std::cout<< "Intersection" << '\n';
        if (AggregateFactorsListDouble(curInferFactor, factorList,
                bsmCheckListSize)<= minFactor) {
            minName = "Intersection";
            minFactor = AggregateFactorsListDouble(curInferFactor, factorList,
                    bsmCheckListSize);
        }
    }
//    if(mbType == 1){
//        std::cout<< "V"<< version <<" minName=" << minName << " minFactor="<<minFactor<< '\n';
//    }
    return minFactor;
}

