/*******************************************************************************
 * @author  Joseph Kamel
 * @date    11/04/2014
 * @version 1.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/

#include "MDModuleV2.h"

#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>

#include "supportClasses/Printable.h"

using namespace std;
using namespace boost;

static char const *nameMin = "cumulV2Min";
static char const *nametempMin = "tempV2Min";

static char const *nameMax = "cumulV2Max";
static char const *nametempMax = "tempV2Max";

static char const *nameApp = "cumulV2App";
static char const *nametempApp = "tempV2App";

static Printable prntMin(nameMin);
static Printable prntMinTemp(nametempMin);

static Printable prntMax(nameMax);
static Printable prntMaxTemp(nametempMax);

static Printable prntApp(nameApp);
static Printable prntAppTemp(nametempApp);

MDModuleV2::MDModuleV2(int myId, Coord myPosition, Coord myPositionConfidence) {
    this->myId = myId;
    this->myPosition = myPosition;
    this->myPositionConfidence = myPositionConfidence;
}

double MDModuleV2::RangePlausibilityCheck(Coord receiverPosition,
        Coord receiverPositionConfidence, Coord senderPosition,
        Coord senderPositionConfidence) {

//    if(senderPositionConfidence.x>MAX_CONFIDENCE_RANGE){
//        return 0;
//    }

    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);
    double senderR = senderPositionConfidence.x;
    double receiverR = receiverPositionConfidence.x;
    double factor = mdmLib.CircleCircleFactor(distance, senderR, receiverR,
    MAX_PLAUSIBLE_RANGE);

    return factor;
}

double MDModuleV2::PositionConsistancyCheck(Coord curPosition,
        Coord curPositionConfidence, Coord oldPosition,
        Coord oldPositionConfidence, double time) {
    double distance = mdmLib.calculateDistance(curPosition, oldPosition);
    double curR = curPositionConfidence.x;
    double oldR = oldPositionConfidence.x;

    double factor = mdmLib.CircleCircleFactor(distance, curR, oldR,
    MAX_CONSISTANT_DISTANCE * time);
    return factor;
}

double MDModuleV2::SpeedConsistancyCheck(double curSpeed,
        double curSpeedConfidence, double oldspeed, double oldSpeedConfidence,
        double time) {

    double speedDelta = curSpeed - oldspeed;
    double factor = 1;
    if (speedDelta > 0) {
        factor = mdmLib.SegmentSegmentFactor(speedDelta, curSpeedConfidence,
                oldSpeedConfidence,
                MAX_PLAUSIBLE_ACCEL * time);
    } else {
        factor = mdmLib.SegmentSegmentFactor(speedDelta, curSpeedConfidence,
                oldSpeedConfidence,
                MAX_PLAUSIBLE_DECEL * time);
    }

    return factor;
}

double MDModuleV2::SpeedPlausibilityCheck(double speed,
        double speedConfidence) {
    if ((fabs(speed) + fabs(speedConfidence) / 2) < MAX_PLAUSIBLE_SPEED) {
        return 1;
    } else if ((fabs(speed) - fabs(speedConfidence) / 2) > MAX_PLAUSIBLE_SPEED) {
        return 0;
    } else {
        return (fabs(speedConfidence) / 2 + (MAX_PLAUSIBLE_SPEED - fabs(speed)))
                / fabs(speedConfidence);
    }
}

double MDModuleV2::PositionSpeedConsistancyCheck(Coord curPosition,
        Coord curPositionConfidence, Coord oldPosition,
        Coord oldPositionConfidence, double curSpeed, double curSpeedConfidence,
        double oldspeed, double oldSpeedConfidence, double time) {

    MDMLib mdmLib;

    if (time < MAX_TIME_DELTA) {

        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        double theoreticalSpeed = distance / time;
        double maxspeed = std::max(curSpeed, oldspeed);
        double minspeed = std::min(curSpeed, oldspeed);

        double curR = curPositionConfidence.x / time + curSpeedConfidence;
        double oldR = oldPositionConfidence.x / time + oldSpeedConfidence;

        double minfactor;

        minfactor = mdmLib.OneSidedCircleSegmentFactor(theoreticalSpeed, curR,
                oldR, maxspeed - MIN_PSS);
//        std::cout << " theoreticalSpeed:" << theoreticalSpeed << " curR:" << curR
//                << " oldR:" << oldR << " maxspeed - MIN_SPEED_DELTA:" << maxspeed - MIN_SPEED_DELTA
//                << '\n';

        double maxfactor;

        if (minspeed - MAX_PSS < 0) {
            maxfactor = 1;
        } else {
            maxfactor = mdmLib.OneSidedCircleSegmentFactor(theoreticalSpeed,
                    curR, oldR, minspeed - MAX_PSS);
            maxfactor = 1 - maxfactor;
        }

//        std::cout << " minfactor:" << minfactor << " maxfactor:" << maxfactor
//                << " theoreticalSpeed:" << theoreticalSpeed << " maxspeed:" << maxspeed
//                << " minspeed:" << minspeed << '\n';

        if (minfactor < maxfactor) {
            return minfactor;
        } else {
            return maxfactor;
        }

    } else {
        return 1;
    }
}

double MDModuleV2::IntersectionCheck(Coord nodePosition1,
        Coord nodePositionConfidence1, Coord nodePosition2,
        Coord nodePositionConfidence2) {

    double distance = mdmLib.calculateDistance(nodePosition1, nodePosition2);
    double intFactor = mdmLib.intersectionFactor(nodePositionConfidence1.x,
            nodePositionConfidence2.x, distance, MIN_INT_DIST);

    intFactor = 1 - intFactor;
    return intFactor;

}

InterTest MDModuleV2::MultipleIntersectionCheck(NodeTable detectedNodes,
        BasicSafetyMessage bsm) {
    int senderId = bsm.getSenderAddress();
    Coord senderPos = bsm.getSenderPos();
    Coord senderPosConfidence = bsm.getSenderPosConfidence();

    NodeHistory senderNode = detectedNodes.getNodeHistory(senderId);
    NodeHistory varNode;

    std::map<std::string, double> result;
    double INTScore = 0;
    int INTNum = 0;

    INTScore = IntersectionCheck(myPosition, myPositionConfidence, senderPos,
            senderPosConfidence);
    if (INTScore < 1) {
        result["INTId_0"] = myId;
        result["INTCheck_0"] = INTScore;
        INTNum++;
    }

    char num_string[32];
    char INTId_string[64] = "INTId_";
    char INTCheck_string[64] = "INTCheck_";

    for (int var = 0; var < detectedNodes.getNodesNum(); ++var) {
        if (detectedNodes.getNodeId(var) != senderId) {
            varNode = detectedNodes.getNodeHistory(
                    detectedNodes.getNodeId(var));

            if (mdmLib.calculateDeltaTime(varNode.getLatestBSM(),
                    bsm) < MAX_DELTA_INTER) {

                INTScore = IntersectionCheck(
                        varNode.getLatestBSM().getSenderPos(),
                        varNode.getLatestBSM().getSenderPosConfidence(),
                        senderPos, senderPosConfidence);
                if (INTScore < 1) {
                    sprintf(num_string, "%d", INTNum);
                    strcat(INTId_string, num_string);
                    strcat(INTCheck_string, num_string);
                    result[INTId_string] = detectedNodes.getNodeId(var);
                    result[INTCheck_string] = INTScore;

                    strncpy(INTId_string, "INTId_", sizeof(INTId_string));
                    strncpy(INTCheck_string, "INTCheck_",
                            sizeof(INTCheck_string));

                    INTNum++;
                }
            }
        }
    }

    result["INTNum"] = INTNum;

    InterTest intertTest = InterTest(INTNum);

    for (int var = 0; var < INTNum; ++var) {
        sprintf(num_string, "%d", var);
        strcat(INTId_string, num_string);
        strcat(INTCheck_string, num_string);

        intertTest.addInterValue(result.find(INTId_string)->second,
                result.find(INTCheck_string)->second);

        strncpy(INTId_string, "INTId_", sizeof(INTId_string));
        strncpy(INTCheck_string, "INTCheck_", sizeof(INTCheck_string));
    }

    return intertTest;
}

double MDModuleV2::SuddenAppearenceCheck(Coord receiverPosition,
        Coord receiverPositionConfidence, Coord senderPosition,
        Coord senderPositionConfidence) {
    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);
    double r1 = senderPositionConfidence.x;
    double r2 = SUDDEN_APPEARENCE_RANGE + receiverPositionConfidence.x;

    double factor = 0;
    if (r1 <= 0) {
        if (distance
                < (SUDDEN_APPEARENCE_RANGE + receiverPositionConfidence.x)) {
            factor = 0;
        } else {
            factor = 1;
        }
    } else {
        double area = mdmLib.calculateCircleCircleIntersection(r1, r2,
                distance);

        factor = area / (PI * r1 * r1);
        factor = 1 - factor;
    }

    return factor;
}

double MDModuleV2::PositionPlausibilityCheck(Coord senderPosition,
        Coord senderPositionConfidence, double senderSpeed,
        double senderSpeedConfidence) {

    if (senderSpeed - senderSpeedConfidence <= MAX_NON_ROUTE_SPEED) {
        return 1;
    }

    ObstacleControl* obstacles = ObstacleControlAccess().getIfExists();

    double resolution = senderPositionConfidence.x / 10;
    if (resolution < 1) {
        resolution = 1;
    }
    double resolutionDelta = resolution / 10;

    double Intersection = 0;
    int count = 5;

    Intersection = obstacles->calculateInsersion(senderPosition, resolution / 2,
            resolution / 2);

    for (double r = resolution; r < senderPositionConfidence.x;
            r = r + resolution) {
        int resolutionTheta = (int) (2 * PI * r / (resolution));
        //std::cout << r<< "#" << resolution << "^" << resolutionTheta<<"-";
        Coord pList[resolutionTheta];
        for (int t = 0; t < resolutionTheta; ++t) {
            Coord p(senderPosition.x + r * cos(2 * PI * t / resolutionTheta),
                    senderPosition.y + r * sin(2 * PI * t / resolutionTheta));
            pList[t] = p;
            count++;
        }

        double IntersectionTemp = obstacles->calculateInsersionList(
                senderPosition, r, r, pList, resolutionTheta);

        Intersection = Intersection + IntersectionTemp;

        resolution = resolution + resolutionDelta;
    }
    //return Intersection;

//    if ((1 - (Intersection / count)) < 0.1) {
//        std::cout
//                << "*******************************************************************************"
//                << '\n';
//        std::cout
//                << "*******************************************************************************Intersection: "
//                << Intersection << " count:" << count << '\n';
//        std::cout
//                << "*******************************************************************************"
//                << '\n';
//    }

    return (1 - (Intersection / count));
}

double MDModuleV2::BeaconFrequencyCheck(double timeNew, double timeOld) {
    double timeDelta = timeNew - timeOld;
    if (timeDelta < MAX_BEACON_FREQUENCY) {
        return 0;
    } else {
        return 1;
    }
}

double MDModuleV2::PositionHeadingConsistancyCheck(Coord curHeading,
        Coord curHeadingConfidence, Coord oldPosition,
        Coord oldPositionConfidence, Coord curPosition,
        Coord curPositionConfidence, double deltaTime, double curSpeed,
        double curSpeedConfidence) {
    if (deltaTime < POS_HEADING_TIME) {
        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        if (distance < 1) {
            return 1;
        }

        if (curSpeed - curSpeedConfidence < 1) {
            return 1;
        }

        double curHeadingAngle = mdmLib.calculateHeadingAngle(curHeading);

        Coord relativePos = Coord(curPosition.x - oldPosition.x,
                curPosition.y - oldPosition.y, curPosition.z - oldPosition.z);
        double positionAngle = mdmLib.calculateHeadingAngle(relativePos);
        double angleDelta = fabs(curHeadingAngle - positionAngle);
        if (angleDelta > 180) {
            angleDelta = 360 - angleDelta;
        }

        double angleLow = angleDelta - curHeadingConfidence.x;
        if (angleLow < 0) {
            angleLow = 0;
        }

        double angleHigh = angleDelta + curHeadingConfidence.x;
        if (angleHigh > 180) {
            angleHigh = 180;
        }

        double xLow = distance * cos(angleLow * PI / 180);

        double curFactorLow = mdmLib.calculateCircleSegment(
                curPositionConfidence.x, curPositionConfidence.x + xLow)
                / (PI * curPositionConfidence.x * curPositionConfidence.x);
        double oldFactorLow = 1
                - mdmLib.calculateCircleSegment(oldPositionConfidence.x,
                        oldPositionConfidence.x - xLow)
                        / (PI * oldPositionConfidence.x
                                * oldPositionConfidence.x);

        double xHigh = distance * cos(angleHigh * PI / 180);

        double curFactorHigh = mdmLib.calculateCircleSegment(
                curPositionConfidence.x, curPositionConfidence.x + xHigh)
                / (PI * curPositionConfidence.x * curPositionConfidence.x);
        double oldFactorHigh = 1
                - mdmLib.calculateCircleSegment(oldPositionConfidence.x,
                        oldPositionConfidence.x - xHigh)
                        / (PI * oldPositionConfidence.x
                                * oldPositionConfidence.x);

        double factor = (curFactorLow + oldFactorLow + curFactorHigh
                + oldFactorHigh) / 4;

//    if(factor<=0.001){
//
//        std::cout<<"relativePos: "<<relativePos<<'\n';
//
//        std::cout<<"curFactorLow: "<<curFactorLow<<'\n';
//        std::cout<<"oldFactorLow: "<<oldFactorLow<<'\n';
//        std::cout<<"positionAngle: "<<positionAngle<<'\n';
//        std::cout<<"curHeadingAngle: "<<curHeadingAngle<<'\n';
//        std::cout<<"angleDelta: "<<angleDelta<<'\n';
//        std::cout<<"distance: "<<distance<<'\n';
//        std::cout<<"distance: "<<distance<<'\n';
//        std::cout<<"xLow: "<<xLow<<'\n';
//        if(factor == 0){
//            std::cout<<"ZERO: "<<factor<<'\n';
//        }else{
//            std::cout<<"NONZ: "<<factor<<'\n';
//        }
//    }

        return factor;
    } else {
        return 1;
    }
}

BsmCheck MDModuleV2::CheckBSM(BasicSafetyMessage bsm, NodeTable detectedNodes) {

    BsmCheck bsmCheck = BsmCheck();

    int senderId = bsm.getSenderAddress();
    Coord senderPos = bsm.getSenderPos();
    Coord senderPosConfidence = bsm.getSenderPosConfidence();

    NodeHistory senderNode = detectedNodes.getNodeHistory(senderId);

    bsmCheck.setRangePlausibility(
            RangePlausibilityCheck(myPosition, myPositionConfidence, senderPos,
                    senderPosConfidence));

    bsmCheck.setSpeedPlausibility(
            SpeedPlausibilityCheck(mdmLib.calculateSpeed(bsm.getSenderSpeed()),
                    mdmLib.calculateSpeed(bsm.getSenderSpeedConfidence())));

    if (detectedNodes.getNodeHistory(senderId).getBSMNum() > 0) {
        bsmCheck.setPositionConsistancy(
                PositionConsistancyCheck(senderPos, senderPosConfidence,
                        senderNode.getLatestBSM().getSenderPos(),
                        senderNode.getLatestBSM().getSenderPosConfidence(),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode.getLatestBSM())));

        bsmCheck.setSpeedConsistancy(
                SpeedConsistancyCheck(
                        mdmLib.calculateSpeed(bsm.getSenderSpeed()),
                        mdmLib.calculateSpeed(bsm.getSenderSpeedConfidence()),
                        mdmLib.calculateSpeed(
                                senderNode.getLatestBSM().getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                senderNode.getLatestBSM().getSenderSpeedConfidence()),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode.getLatestBSM())));

        bsmCheck.setPositionSpeedConsistancy(
                PositionSpeedConsistancyCheck(senderPos, senderPosConfidence,
                        senderNode.getLatestBSM().getSenderPos(),
                        senderNode.getLatestBSM().getSenderPosConfidence(),
                        mdmLib.calculateSpeed(bsm.getSenderSpeed()),
                        mdmLib.calculateSpeed(bsm.getSenderSpeedConfidence()),
                        mdmLib.calculateSpeed(
                                senderNode.getLatestBSM().getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                senderNode.getLatestBSM().getSenderSpeedConfidence()),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode.getLatestBSM())));

        bsmCheck.setBeaconFrequency(
                BeaconFrequencyCheck(bsm.getArrivalTime().dbl(),
                        senderNode.getLatestBSM().getArrivalTime().dbl()));

        bsmCheck.setPositionHeadingConsistancy(
                PositionHeadingConsistancyCheck(bsm.getSenderHeading(),
                        bsm.getSenderHeadingConfidence(),
                        senderNode.getLatestBSM().getSenderPos(),
                        senderNode.getLatestBSM().getSenderPosConfidence(),
                        bsm.getSenderPos(), bsm.getSenderPosConfidence(),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode.getLatestBSM()),
                        mdmLib.calculateSpeed(bsm.getSenderSpeed()),
                        mdmLib.calculateSpeed(bsm.getSenderSpeedConfidence())));

    } else {
        bsmCheck.setSuddenAppearence(
                SuddenAppearenceCheck(senderPos, senderPosConfidence,
                        myPosition, myPositionConfidence));
    }

    bsmCheck.setPositionPlausibility(
            PositionPlausibilityCheck(senderPos, senderPosConfidence,
                    mdmLib.calculateSpeed(bsm.getSenderSpeed()),
                    mdmLib.calculateSpeed(bsm.getSenderSpeedConfidence())));

    bsmCheck.setIntersection(MultipleIntersectionCheck(detectedNodes, bsm));

    //PrintBsmCheck(senderId, bsmCheck);

    CheckNodeByThreshold(bsm, bsmCheck, detectedNodes, bsm.getSenderMbType());

    bsmCheck = CheckNodeByApplication2(bsm, bsmCheck, detectedNodes,
            bsm.getSenderMbType());

    return bsmCheck;
}

void MDModuleV2::PrintBsmCheck(int senderId, BsmCheck bsmCheck) {

    if (bsmCheck.getRangePlausibility() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "ART FAILED => "
                << bsmCheck.getRangePlausibility() << " A:" << myId << " B:"
                << senderId << '\n';
    }
    if (bsmCheck.getPositionConsistancy() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MGTD FAILED => "
                << bsmCheck.getPositionConsistancy() << " A:" << myId << " B:"
                << senderId << '\n';
    }
    if (bsmCheck.getSpeedConsistancy() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MGTS FAILED => "
                << bsmCheck.getSpeedConsistancy() << " A:" << myId << " B:"
                << senderId << '\n';
    }

    if (bsmCheck.getPositionSpeedConsistancy() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MGTSV FAILED => "
                << bsmCheck.getPositionSpeedConsistancy() << " A:" << myId
                << " B:" << senderId << '\n';
    }

    if (bsmCheck.getSpeedPlausibility() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MAXS FAILED => "
                << bsmCheck.getSpeedPlausibility() << " A:" << myId << " B:"
                << senderId << '\n';
    }
    if (bsmCheck.getPositionPlausibility() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "MAP FAILED => "
                << bsmCheck.getPositionPlausibility() << " A:" << myId << " B:"
                << senderId << '\n';
    }

    if (bsmCheck.getSuddenAppearence() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "SAW FAILED => "
                << bsmCheck.getSuddenAppearence() << " A:" << myId << " B:"
                << senderId << '\n';
    }

    if (bsmCheck.getPositionHeadingConsistancy() < 0.5) {
        std::cout << "^^^^^^^^^^^V2 " << "PHC FAILED => "
                << bsmCheck.getPositionHeadingConsistancy() << " A:" << myId
                << " B:" << senderId << '\n';
    }

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        if (inter.getInterValue(var) < 0.5) {
            std::cout << "^^^^^^^^^^^V2 " << "INT FAILED => "
                    << inter.getInterValue(var) << " A:" << myId << " B:"
                    << senderId << " C:" << inter.getInterId(var) << '\n';
        }
    }

}
BsmCheck MDModuleV2::CheckNodeByApplication(BasicSafetyMessage bsm,
        BsmCheck bsmCheck, NodeTable detectedNodes, double mbType) {

    bool checkFailed = false;
    MBReport mbReport;

    prntApp.incAll(mbType);
    prntAppTemp.incAll(mbType);

    int senderId = bsm.getSenderAddress();

    MDMHistory mdmHist = detectedNodes.getMDMHistory(senderId);
    NodeHistory nodeHist = detectedNodes.getNodeHistory(senderId);

    BsmCheck bsmCheck0 = BsmCheck();
    BsmCheck bsmCheck1 = BsmCheck();

    bool bsmCheckNew0 = true;
    bool bsmCheckNew1 = true;

    if (nodeHist.getBSMNum() > 0) {
        if (mdmLib.calculateDeltaTime(bsm,
                nodeHist.getLatestBSM())<DELTA_BSM_TIME) {
            bsmCheck0 = mdmHist.getBsmCheck(0);
            bsmCheckNew0 = false;
            if (nodeHist.getBSMNum() > 1) {
                if (mdmLib.calculateDeltaTime(bsm,
                        nodeHist.getSecondLatestBSM())<DELTA_BSM_TIME) {
                    bsmCheck1 = mdmHist.getBsmCheck(1);
                    bsmCheckNew1 = false;
                }
            }
        }
    }

    //std::cout<< "RangePlausibility" << '\n';
    if (AggregateFactors(bsmCheck.getRangePlausibility(),
            bsmCheck0.getRangePlausibility(),
            bsmCheck1.getRangePlausibility())) {
        checkFailed = true;
        prntApp.incFlags("RangePlausibility", mbType);
        prntAppTemp.incFlags("RangePlausibility", mbType);
    }

    //std::cout<< "PositionConsistancy" << '\n';
    if (AggregateFactors(bsmCheck.getPositionConsistancy(),
            bsmCheck0.getPositionConsistancy(),
            bsmCheck1.getPositionConsistancy())) {
        checkFailed = true;
        prntApp.incFlags("PositionConsistancy", mbType);
        prntAppTemp.incFlags("PositionConsistancy", mbType);
    }

    //std::cout<< "PositionSpeedConsistancy" << '\n';
    if (AggregateFactors(bsmCheck.getPositionSpeedConsistancy(),
            bsmCheck0.getPositionSpeedConsistancy(),
            bsmCheck1.getPositionSpeedConsistancy())) {
        checkFailed = true;
        prntApp.incFlags("PositionSpeedConsistancy", mbType);
        prntAppTemp.incFlags("PositionSpeedConsistancy", mbType);
    }

    //std::cout<< "SpeedConsistancy" << '\n';
    if (AggregateFactors(bsmCheck.getSpeedConsistancy(),
            bsmCheck0.getSpeedConsistancy(), bsmCheck1.getSpeedConsistancy())) {
        checkFailed = true;
        prntApp.incFlags("SpeedConsistancy", mbType);
        prntAppTemp.incFlags("SpeedConsistancy", mbType);
    }

    //std::cout<< "SpeedPlausibility" << '\n';
    if (AggregateFactors(bsmCheck.getSpeedPlausibility(),
            bsmCheck0.getSpeedPlausibility(),
            bsmCheck1.getSpeedPlausibility())) {
        checkFailed = true;
        prntApp.incFlags("SpeedPlausibility", mbType);
        prntAppTemp.incFlags("SpeedPlausibility", mbType);
    }

    //std::cout<< "PositionPlausibility" << '\n';
    if (AggregateFactors(bsmCheck.getPositionPlausibility(),
            bsmCheck0.getPositionPlausibility(),
            bsmCheck1.getPositionPlausibility())) {
        checkFailed = true;
        prntApp.incFlags("PositionPlausibility", mbType);
        prntAppTemp.incFlags("PositionPlausibility", mbType);
    }

    //std::cout<< "BeaconFrequency" << '\n';
    if (AggregateFactors(bsmCheck.getBeaconFrequency(),
            bsmCheck0.getBeaconFrequency(), bsmCheck1.getBeaconFrequency())) {
        checkFailed = true;
        prntApp.incFlags("BeaconFrequency", mbType);
        prntAppTemp.incFlags("BeaconFrequency", mbType);
    }

    //std::cout<< "SuddenAppearence" << '\n';
    if (AggregateFactors(bsmCheck.getSuddenAppearence(),
            bsmCheck0.getSuddenAppearence(), bsmCheck1.getSuddenAppearence())) {
        prntApp.incFlags("SuddenAppearence", mbType);
        prntAppTemp.incFlags("SuddenAppearence", mbType);
    }

    //std::cout<< "PositionHeadingConsistancy" << '\n';
    if (AggregateFactors(bsmCheck.getPositionHeadingConsistancy(),
            bsmCheck0.getPositionHeadingConsistancy(),
            bsmCheck1.getPositionHeadingConsistancy())) {
        checkFailed = true;
        prntApp.incFlags("PositionHeadingConsistancy", mbType);
        prntAppTemp.incFlags("PositionHeadingConsistancy", mbType);
    }

    InterTest inter = bsmCheck.getIntersection();
    InterTest inter0 = bsmCheck0.getIntersection();
    InterTest inter1 = bsmCheck1.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {

        double IT = inter.getInterValue(var);

        double IT0 = inter0.getIdIndex(inter.getInterId(var));
        double IT1 = inter1.getIdIndex(inter.getInterId(var));

        if (IT0 != -1) {
            IT0 = inter0.getInterValue(IT0);
        } else {
            IT0 = 1;
        }

        if (IT1 != -1) {
            IT1 = inter0.getInterValue(IT1);
        } else {
            IT1 = 1;
        }

        //std::cout<< "Intersection" << '\n';
        if (AggregateFactors(IT, IT0, IT1)) {
            checkFailed = true;
            prntApp.incFlags("Intersection", mbType);
            prntAppTemp.incFlags("Intersection", mbType);
        }
    }

    if (checkFailed) {
        mbReport.setGenerationTime(simTime().dbl());
        mbReport.setSenderId(myId);
        mbReport.setReportedId(senderId);
        mbReport.setMbType(mbType);
        SendReport(mbReport);

        prntApp.incCumulFlags(mbType);
        prntAppTemp.incCumulFlags(mbType);

        bsmCheck.setReported(true);

//        if (!bsmCheckNew0 && !bsmCheck0.getReported()) {
//            prntApp.incCumulFlags(mbType);
//            prntAppTemp.incCumulFlags(mbType);
//        }
//
//        if (!bsmCheckNew1 && !bsmCheck1.getReported()) {
//            prntApp.incCumulFlags(mbType);
//            prntAppTemp.incCumulFlags(mbType);
//        }
    }
    return bsmCheck;
}

bool MDModuleV2::AggregateFactors(double curFactor, double factor0,
        double factor1) {
    if (curFactor <= 0) {
        return true;
    } else if (curFactor >= 1) {
        return false;
    } else {
        if ((curFactor + factor0 + factor1) / 3 < 0.5) {
            return true;
        } else {
            return false;
        }
    }
}

BsmCheck MDModuleV2::CheckNodeByApplication2(BasicSafetyMessage bsm,
        BsmCheck bsmCheck, NodeTable detectedNodes, double mbType) {

    bool checkFailed = false;
    MBReport mbReport;

    prntApp.incAll(mbType);
    prntAppTemp.incAll(mbType);

    int senderId = bsm.getSenderAddress();

    MDMHistory mdmHist = detectedNodes.getMDMHistory(senderId);
    NodeHistory nodeHist = detectedNodes.getNodeHistory(senderId);

    BsmCheck bsmCheckList[MAXBSM_TRUST_LENGTH];
    int bsmCheckListSize = 0;


    for (int var = 0; var < nodeHist.getBSMNum(); ++var) {
        if (bsmCheckListSize < MAXBSM_TRUST_LENGTH) {
            if (mdmLib.calculateDeltaTime(bsm,
                    nodeHist.getBSM(var))<DELTA_TRUST_TIME) {
                bsmCheckList[bsmCheckListSize] = mdmHist.getBsmCheck(var);
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
        prntAppTemp.incFlags("RangePlausibility", mbType);
    }


    //std::cout<< "PositionConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionConsistancy();
    }

    if (AggregateFactorsList(bsmCheck.getPositionConsistancy(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("PositionConsistancy", mbType);
        prntAppTemp.incFlags("PositionConsistancy", mbType);
    }


    //std::cout<< "PositionSpeedConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionSpeedConsistancy();
    }

    if (AggregateFactorsList(bsmCheck.getPositionSpeedConsistancy(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("PositionSpeedConsistancy", mbType);
        prntAppTemp.incFlags("PositionSpeedConsistancy", mbType);
    }

    //std::cout<< "SpeedConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSpeedConsistancy();
    }

    if (AggregateFactorsList(bsmCheck.getSpeedConsistancy(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("SpeedConsistancy", mbType);
        prntAppTemp.incFlags("SpeedConsistancy", mbType);
    }

    //std::cout<< "SpeedPlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSpeedPlausibility();
    }

    if (AggregateFactorsList(bsmCheck.getSpeedPlausibility(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("SpeedPlausibility", mbType);
        prntAppTemp.incFlags("SpeedPlausibility", mbType);
    }

    //std::cout<< "PositionPlausibility" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionPlausibility();
    }
    if (AggregateFactorsList(bsmCheck.getPositionPlausibility(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("PositionPlausibility", mbType);
        prntAppTemp.incFlags("PositionPlausibility", mbType);
    }

    //std::cout<< "BeaconFrequency" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getBeaconFrequency();
    }

    if (AggregateFactorsList(bsmCheck.getBeaconFrequency(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("BeaconFrequency", mbType);
        prntAppTemp.incFlags("BeaconFrequency", mbType);
    }

    //std::cout<< "SuddenAppearence" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getSuddenAppearence();
    }

    if (AggregateFactorsList(bsmCheck.getSuddenAppearence(), factorList,
            bsmCheckListSize)) {
        prntApp.incFlags("SuddenAppearence", mbType);
        prntAppTemp.incFlags("SuddenAppearence", mbType);
    }

    //std::cout<< "PositionHeadingConsistancy" << '\n';
    for (int var = 0; var < bsmCheckListSize; ++var) {
        factorList[var] = bsmCheckList[var].getPositionHeadingConsistancy();
    }

    if (AggregateFactorsList(bsmCheck.getPositionHeadingConsistancy(), factorList,
            bsmCheckListSize)) {
        checkFailed = true;
        prntApp.incFlags("PositionHeadingConsistancy", mbType);
        prntAppTemp.incFlags("PositionHeadingConsistancy", mbType);
    }


    for (int var = 0; var < bsmCheckListSize; ++var) {
        bsmCheckList[var].getIntersection();
    }


    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {
        double curInferFactor = inter.getInterValue(var);

        for (int i = 0; i < bsmCheckListSize; ++i) {
            double IdIndex = bsmCheckList[i].getIntersection().getIdIndex(inter.getInterId(var));
                if (IdIndex != -1) {
                    factorList[i] = bsmCheckList[i].getIntersection().getInterValue(IdIndex);
                } else {
                    factorList[i] = 1;
                }
        }

        //std::cout<< "Intersection" << '\n';
        if (AggregateFactorsList(curInferFactor, factorList, bsmCheckListSize)) {
            checkFailed = true;
            prntApp.incFlags("Intersection", mbType);
            prntAppTemp.incFlags("Intersection", mbType);
        }
    }


    if (checkFailed) {
        mbReport.setGenerationTime(simTime().dbl());
        mbReport.setSenderId(myId);
        mbReport.setReportedId(senderId);
        mbReport.setMbType(mbType);
        SendReport(mbReport);

        prntApp.incCumulFlags(mbType);
        prntAppTemp.incCumulFlags(mbType);

        bsmCheck.setReported(true);
    }
    return bsmCheck;
}

bool MDModuleV2::AggregateFactorsList(double curFactor, double *factorList,
        int factorListSize) {

    if (curFactor <= 0) {
        return true;
    } else if (curFactor >= 1) {
        return false;
    } else {
        double averageFactor = curFactor;
        for (int var = 0; var < factorListSize; ++var) {
            averageFactor = averageFactor + factorList[var];
        }
        if(factorListSize > 0){
            averageFactor = averageFactor / (factorListSize+1);
        }else{
            averageFactor = 1;
        }
        if ((averageFactor) < 0.5) {
            return true;
        } else {
            return false;
        }
    }
}

BsmCheck MDModuleV2::CheckNodeByThreshold(BasicSafetyMessage bsm,
        BsmCheck bsmCheck, NodeTable detectedNodes, double mbType) {

    bool checkFailedMin = false;
    bool checkFailedMax = false;

    MBReport mbReportMin;
    MBReport mbReportMax;

    prntMin.incAll(mbType);
    prntMinTemp.incAll(mbType);
    prntMax.incAll(mbType);
    prntMaxTemp.incAll(mbType);

    int senderId = bsm.getSenderAddress();

    MDMHistory mdmHist = detectedNodes.getMDMHistory(senderId);
    NodeHistory nodeHist = detectedNodes.getNodeHistory(senderId);

    if (bsmCheck.getRangePlausibility() < 1) {
        checkFailedMax = true;
        prntMax.incFlags("RangePlausibility", mbType);
        prntMaxTemp.incFlags("RangePlausibility", mbType);

        if (bsmCheck.getRangePlausibility() <= 0) {
            checkFailedMin = true;
            prntMin.incFlags("RangePlausibility", mbType);
            prntMinTemp.incFlags("RangePlausibility", mbType);
        }
    }
    if (bsmCheck.getPositionConsistancy() < 1) {
        checkFailedMax = true;
        prntMax.incFlags("PositionConsistancy", mbType);
        prntMaxTemp.incFlags("PositionConsistancy", mbType);

        if (bsmCheck.getPositionConsistancy() <= 0) {
            checkFailedMin = true;
            prntMin.incFlags("PositionConsistancy", mbType);
            prntMinTemp.incFlags("PositionConsistancy", mbType);
        }

    }

    if (bsmCheck.getPositionSpeedConsistancy() < 1) {
        checkFailedMax = true;
        prntMax.incFlags("PositionSpeedConsistancy", mbType);
        prntMaxTemp.incFlags("PositionSpeedConsistancy", mbType);

        if (bsmCheck.getPositionSpeedConsistancy() <= 0) {
            checkFailedMin = true;
            prntMin.incFlags("PositionSpeedConsistancy", mbType);
            prntMinTemp.incFlags("PositionSpeedConsistancy", mbType);
        }
    }

    if (bsmCheck.getSpeedConsistancy() < 1) {
        checkFailedMax = true;
        prntMax.incFlags("SpeedConsistancy", mbType);
        prntMaxTemp.incFlags("SpeedConsistancy", mbType);

        if (bsmCheck.getSpeedConsistancy() <= 0) {
            checkFailedMin = true;
            prntMin.incFlags("SpeedConsistancy", mbType);
            prntMinTemp.incFlags("SpeedConsistancy", mbType);
        }

    }

    if (bsmCheck.getSpeedPlausibility() < 1) {
        checkFailedMax = true;
        prntMax.incFlags("SpeedPlausibility", mbType);
        prntMaxTemp.incFlags("SpeedPlausibility", mbType);

        if (bsmCheck.getSpeedPlausibility() <= 0) {
            checkFailedMin = true;
            prntMin.incFlags("SpeedPlausibility", mbType);
            prntMinTemp.incFlags("SpeedPlausibility", mbType);
        }
    }

    if (bsmCheck.getPositionPlausibility() < 1) {
        checkFailedMax = true;
        prntMax.incFlags("PositionPlausibility", mbType);
        prntMaxTemp.incFlags("PositionPlausibility", mbType);

        if (bsmCheck.getPositionPlausibility() <= 0) {
            checkFailedMin = true;
            prntMin.incFlags("PositionPlausibility", mbType);
            prntMinTemp.incFlags("PositionPlausibility", mbType);
        }

    }

    if (bsmCheck.getBeaconFrequency() < 1) {
        checkFailedMax = true;
        prntMax.incFlags("BeaconFrequency", mbType);
        prntMaxTemp.incFlags("BeaconFrequency", mbType);

        if (bsmCheck.getBeaconFrequency() <= 0) {
            checkFailedMin = true;
            prntMin.incFlags("BeaconFrequency", mbType);
            prntMinTemp.incFlags("BeaconFrequency", mbType);
        }
    }

    if (bsmCheck.getSuddenAppearence() < 1) {
        prntMax.incFlags("SuddenAppearence", mbType);
        prntMaxTemp.incFlags("SuddenAppearence", mbType);

        if (bsmCheck.getSuddenAppearence() <= 0) {
            prntMin.incFlags("SuddenAppearence", mbType);
            prntMinTemp.incFlags("SuddenAppearence", mbType);
        }
    }

    if (bsmCheck.getPositionHeadingConsistancy() < 1) {
        checkFailedMax = true;
        prntMax.incFlags("PositionHeadingConsistancy", mbType);
        prntMaxTemp.incFlags("PositionHeadingConsistancy", mbType);

        if (bsmCheck.getPositionHeadingConsistancy() <= 0) {
            checkFailedMin = true;
            prntMin.incFlags("PositionHeadingConsistancy", mbType);
            prntMinTemp.incFlags("PositionHeadingConsistancy", mbType);
        }
    }

    bool maxInterFound = false;
    bool minInterFound = false;

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {

        double IT = inter.getInterValue(var);

        if (IT < 1) {
            checkFailedMax = true;
            if (!maxInterFound) {
                prntMax.incFlags("Intersection", mbType);
                prntMaxTemp.incFlags("Intersection", mbType);
                maxInterFound = true;
            }
            if (IT <= 0) {
                checkFailedMin = true;
                if (!minInterFound) {
                    prntMin.incFlags("Intersection", mbType);
                    prntMinTemp.incFlags("Intersection", mbType);
                    minInterFound = true;
                }
            }
        }
    }

    if (checkFailedMax) {
        prntMax.incCumulFlags(mbType);
        prntMaxTemp.incCumulFlags(mbType);
        if (checkFailedMin) {
            prntMin.incCumulFlags(mbType);
            prntMinTemp.incCumulFlags(mbType);
        }
    }

    return bsmCheck;
}

void MDModuleV2::CheckNodesHistoryForReport(NodeTable* detectedNodes) {

    for (int var = 0; var < detectedNodes->getNodesNum(); ++var) {

        int senderId = detectedNodes->getNodeId(var);

        MDMHistory mdmHist = detectedNodes->getMDMHistory(senderId);
        NodeHistory nodeHist = detectedNodes->getNodeHistory(senderId);

        BsmCheck bsmCheck = mdmHist.getBsmCheck(0);
        BasicSafetyMessage bsm = nodeHist.getBSM(0);
        double mbType = bsm.getSenderMbType();

        if (mbType != 0) {
            std::cout << "OK " << myId << " " << senderId << " " << mbType
                    << '\n';
            std::cout << bsmCheck.getPositionSpeedConsistancy() << '\n';

            if (bsmCheck.getReported() == false) {
                if (simTime().dbl()
                        - bsm.getArrivalTime().dbl() > DELTA_REPORT_TIME) {

                    bool checkFailed = false;
                    MBReport mbReport;

                    if (bsmCheck.getRangePlausibility() < 0.5) {
                        std::cout << "##########V2 " << "ART FAILED => "
                                << bsmCheck.getRangePlausibility() << " A:"
                                << myId << " B:" << senderId << '\n';
                        checkFailed = true;
                        prntApp.incFlags("RangePlausibility", mbType);
                        prntAppTemp.incFlags("RangePlausibility", mbType);
                    }
                    if (bsmCheck.getPositionConsistancy() < 0.5) {
                        std::cout << "##########V2 " << "MGTD FAILED => "
                                << bsmCheck.getPositionConsistancy() << " A:"
                                << myId << " B:" << senderId << '\n';
                        checkFailed = true;
                        prntApp.incFlags("PositionConsistancy", mbType);
                        prntAppTemp.incFlags("PositionConsistancy", mbType);
                    }
                    if (bsmCheck.getSpeedConsistancy() < 0.5) {
                        std::cout << "##########V2 " << "MGTS FAILED => "
                                << bsmCheck.getSpeedConsistancy() << " A:"
                                << myId << " B:" << senderId << '\n';
                        checkFailed = true;
                        prntApp.incFlags("SpeedConsistancy", mbType);
                        prntAppTemp.incFlags("SpeedConsistancy", mbType);
                    }

                    if (bsmCheck.getPositionSpeedConsistancy() < 0.5) {
                        std::cout << "##########V2 " << "MGTSV FAILED => "
                                << bsmCheck.getPositionSpeedConsistancy()
                                << " A:" << myId << " B:" << senderId << '\n';
                        checkFailed = true;
                        prntApp.incFlags("PositionSpeedConsistancy", mbType);
                        prntAppTemp.incFlags("PositionSpeedConsistancy",
                                mbType);
                    }

                    if (bsmCheck.getSpeedPlausibility() < 0.5) {
                        std::cout << "##########V2 " << "MAXS FAILED => "
                                << bsmCheck.getSpeedPlausibility() << " A:"
                                << myId << " B:" << senderId << '\n';
                        checkFailed = true;
                        prntApp.incFlags("SpeedPlausibility", mbType);
                        prntAppTemp.incFlags("SpeedPlausibility", mbType);
                    }
                    if (bsmCheck.getPositionPlausibility() < 0.5) {
                        std::cout << "##########V2 " << "MAP FAILED => "
                                << bsmCheck.getPositionPlausibility() << " A:"
                                << myId << " B:" << senderId << '\n';
                        checkFailed = true;
                        prntApp.incFlags("PositionConsistancy", mbType);
                        prntAppTemp.incFlags("PositionConsistancy", mbType);
                    }

                    if (bsmCheck.getSuddenAppearence() < 0.5) {
                        std::cout << "##########V2 " << "SAW FAILED => "
                                << bsmCheck.getSuddenAppearence() << " A:"
                                << myId << " B:" << senderId << '\n';

                        prntApp.incFlags("SuddenAppearence", mbType);
                        prntAppTemp.incFlags("SuddenAppearence", mbType);
                    }

                    if (bsmCheck.getPositionHeadingConsistancy() < 0.5) {
                        std::cout << "##########V2 " << "PHC FAILED => "
                                << bsmCheck.getPositionHeadingConsistancy()
                                << " A:" << myId << " B:" << senderId << '\n';
                        checkFailed = true;

                        prntApp.incFlags("PositionHeadingConsistancy", mbType);
                        prntAppTemp.incFlags("PositionHeadingConsistancy",
                                mbType);
                    }

                    InterTest inter = bsmCheck.getIntersection();
                    for (int var = 0; var < inter.getInterNum(); ++var) {
                        if (inter.getInterValue(var) < 0.5) {
                            std::cout << "##########V2 " << "INT FAILED => "
                                    << inter.getInterValue(var) << " A:" << myId
                                    << " B:" << senderId << " C:"
                                    << inter.getInterId(var) << '\n';
                            checkFailed = true;

                            prntApp.incFlags("Intersection", mbType);
                            prntAppTemp.incFlags("Intersection", mbType);
                        }
                    }

                    if (checkFailed) {
                        mbReport.setGenerationTime(simTime().dbl());
                        mbReport.setSenderId(myId);
                        mbReport.setReportedId(senderId);
                        mbReport.setMbType(mbType);
                        SendReport(mbReport);

                        prntApp.incCumulFlags(mbType);
                        prntAppTemp.incCumulFlags(mbType);

                        bsmCheck.setReported(true);
                        mdmHist.setBsmCheck(0, bsmCheck);
                        detectedNodes->setMDMHistory(senderId, mdmHist);
                    }
                }
            }
        }
    }

}

void MDModuleV2::SendReport(MBReport mbReport) {
    MDAuthority mdAuthority;
    mdAuthority.sendReportV2(mbReport);
}

static bool init = true;

void MDModuleV2::saveLine(std::string path, std::string serial, double density,
        double deltaT) {
    char fileNameMinCumul[] = "mdmResultV2MinCumul.txt";
    char fileNameMinTemp[] = "mdmResultV2MinTemp.txt";

    char fileNameMaxCumul[] = "mdmResultV2MaxCumul.txt";
    char fileNameMaxTemp[] = "mdmResultV2MaxTemp.txt";

    char fileNameAppCumul[] = "mdmResultV2AppCumul.txt";
    char fileNameAppTemp[] = "mdmResultV2AppTemp.txt";

    char outChar[1024];
    char directoryPathGen[1024] = "";
    char filePathGen[1024] = "";
    const char * pathChar = path.c_str();
    const char * serialChar = serial.c_str();
    strcat(directoryPathGen, pathChar);
    strcat(directoryPathGen, serialChar);

    struct stat info;

    if (stat(directoryPathGen, &info) != 0) {
        mkdir(directoryPathGen, 0777);
    } else if (info.st_mode & S_IFDIR) {
    } else {
        mkdir(directoryPathGen, 0777);
    }

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameMinCumul);

    prntMin.getPrintable(outChar, density, deltaT);
    prntMin.writeFile(filePathGen, outChar, init);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameMinTemp);

    prntMinTemp.getPrintable(outChar, density, deltaT);
    prntMinTemp.writeFile(filePathGen, outChar, init);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameMaxCumul);

    prntMax.getPrintable(outChar, density, deltaT);
    prntMax.writeFile(filePathGen, outChar, init);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameMaxTemp);

    prntMaxTemp.getPrintable(outChar, density, deltaT);
    prntMaxTemp.writeFile(filePathGen, outChar, init);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameAppCumul);

    prntApp.getPrintable(outChar, density, deltaT);
    prntApp.writeFile(filePathGen, outChar, init);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameAppTemp);

    prntAppTemp.getPrintable(outChar, density, deltaT);
    prntAppTemp.writeFile(filePathGen, outChar, init);

    if (init) {
        init = false;
    }
}

void MDModuleV2::resetTempFlags() {
    prntMinTemp.resetAll();
    prntMaxTemp.resetAll();
    prntAppTemp.resetAll();
}

void MDModuleV2::resetAllFlags() {
    prntMin.resetAll();
    prntMax.resetAll();
    prntApp.resetAll();
    prntMinTemp.resetAll();
    prntMaxTemp.resetAll();
    prntAppTemp.resetAll();
}
