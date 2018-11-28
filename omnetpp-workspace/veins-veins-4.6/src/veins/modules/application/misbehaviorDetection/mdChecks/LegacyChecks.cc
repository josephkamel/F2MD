/*******************************************************************************
 * @author  Joseph Kamel
 * @email   josephekamel@gmail.com
 * @date    28/11/2018
 * @version 2.0
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
#include <unistd.h>
#include <veins/modules/application/misbehaviorDetection/mdChecks/LegacyChecks.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::max

using namespace std;
using namespace boost;

LegacyChecks::LegacyChecks(unsigned long myPseudonym, Coord myPosition,
        Coord mySpeed, Coord mySize, Coord myHeading, LinkControl* LinkC) {
    this->myPseudonym = myPseudonym;
    this->myPosition = myPosition;
    this->mySpeed = mySpeed;
    this->mySize = mySize;
    this->myHeading = myHeading;

    this->LinkC = LinkC;
}

static double maxRange = 0;
double LegacyChecks::RangePlausibilityCheck(Coord senderPosition,
        Coord receiverPosition) {
    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);

    if (distance > maxRange) {
        maxRange = distance;
    }

    if (distance < MAX_PLAUSIBLE_RANGE) {
        return 1;
    } else {
        return 0; //distance
    }
}


static double maxPC = 0;
double LegacyChecks::PositionConsistancyCheck(Coord curPosition,
        Coord oldPosition, double time) {
    double distance = mdmLib.calculateDistance(curPosition, oldPosition);

    if(distance > maxPC * time){
        maxPC = distance/time;
    }

    if (distance < MAX_CONSISTANT_DISTANCE * time) {
        return 1;
    } else {
        return 0; //distance
    }
}

static double maxSCaccel = 0;
static double maxSCdecel = 0;

double LegacyChecks::SpeedConsistancyCheck(double curSpeed, double oldspeed,
        double time) {

    double speedDelta = curSpeed - oldspeed;

    double attFact = mdmLib.gaussianSum(1, 1 / 3);
    if (time >= 1) {
        attFact = time;
    }

    if (speedDelta > 0) {

        if(speedDelta > maxSCaccel * attFact){
            maxSCaccel = speedDelta/attFact;
        }

        if (speedDelta < MAX_PLAUSIBLE_ACCEL * attFact) {
            return 1;
        } else {
            return 0; //distance
        }
    } else {

        if(fabs(speedDelta) > maxSCdecel * attFact){
            maxSCdecel = fabs(speedDelta)/attFact;
        }

        if (fabs(speedDelta) < MAX_PLAUSIBLE_DECEL * attFact) {
            return 1;
        } else {
            return 0; //distance
        }
    }

}


static double saveMin = 0;
static double saveMax = 0;


double LegacyChecks::PositionSpeedConsistancyCheck(Coord curPosition,
        Coord oldPosition, double curSpeed, double oldspeed, double time) {
    if (time < MAX_TIME_DELTA) {
        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        double theoreticalSpeed = distance / time;
        //double realspeed = fabs(curSpeed + oldspeed)/2;
        double maxspeed = std::max(curSpeed, oldspeed);
        double minspeed = std::min(curSpeed, oldspeed);

        double deltaMax = maxspeed - theoreticalSpeed;
        double deltaMin = minspeed - theoreticalSpeed;

        if (deltaMax > saveMax) {
            saveMax = deltaMax;
        }
        if ((-deltaMin) > saveMin) {
            saveMin = -deltaMin;
        }

//        std::cout<< "saveMax"<< saveMax <<"\n";
//        std::cout<< "saveMin"<< saveMin <<"\n";


        if (deltaMax >  MAX_PLAUSIBLE_DECEL*time) {
            return 0; // deltaMax - MIN_PSS
        } else {
            if (deltaMin >  MAX_PLAUSIBLE_ACCEL*time) {
                return 0; // deltaMin - MAX_PSS
            } else {
                return 1;
            }
        }
    } else {
        return 1;
    }
}


static double maxSP = 0;
double LegacyChecks::SpeedPlausibilityCheck(double speed) {

    if (fabs(speed) > maxSP) {
        maxSP = fabs(speed);
    }

    if (fabs(speed) < MAX_PLAUSIBLE_SPEED) {
        return 1;
    } else {
        return 0; // fabs(speed)
    }
}

double LegacyChecks::IntersectionCheck(Coord nodePosition1, Coord nodeSize1,
        Coord head1, Coord nodePosition2, Coord nodeSize2, Coord head2) {

    double heading1 = mdmLib.calculateHeadingAngle(head1);
    double heading2 = mdmLib.calculateHeadingAngle(head2);

    double distance = mdmLib.calculateDistance(nodePosition1, nodePosition2);

    double inter = mdmLib.RectRectFactor(nodePosition1, nodePosition2, heading1,
            heading2, nodeSize1, nodeSize2);

    if (inter > 0.1) {
        return 0; //inter
    } else {
        return 1;
    }

}


InterTest LegacyChecks::MultipleIntersectionCheck(NodeTable * detectedNodes,
        BasicSafetyMessage * bsm) {

    unsigned long senderPseudonym = bsm->getSenderPseudonym();

    NodeHistory *senderNode = detectedNodes->getNodeHistoryAddr(senderPseudonym);
    NodeHistory *varNode;

    const  int maxInterNum = 100;
     unsigned long resultPseudo[maxInterNum];
     double resultCheck[maxInterNum];

    int INTNum = 0;
    double INTScore = 1;

    INTScore = IntersectionCheck(myPosition, mySize, myHeading, bsm->getSenderPos(),
            Coord(bsm->getSenderWidth(), bsm->getSenderLength()),
            bsm->getSenderHeading());


    if (INTScore < 1 && INTNum<maxInterNum) {
        resultPseudo[INTNum] = myPseudonym;
        resultCheck[INTNum] = INTScore;
        INTNum++;
    }

    for (int var = 0; var < detectedNodes->getNodesNum(); ++var) {
        if (detectedNodes->getNodePseudo(var) != senderPseudonym) {
            varNode = detectedNodes->getNodeHistoryAddr(
                    detectedNodes->getNodePseudo(var));

            if (mdmLib.calculateDeltaTime(varNode->getLatestBSMAddr(),
                    bsm) < MAX_DELTA_INTER) {

                INTScore = IntersectionCheck(varNode->getSenderPos(0),
                        varNode->getSenderSize(0), varNode->getSenderHeading(0),
                        bsm->getSenderPos(),
                        Coord(bsm->getSenderWidth(), bsm->getSenderLength()),
                        bsm->getSenderHeading());
                if (INTScore < 1 && INTNum<maxInterNum) {
                    resultPseudo[INTNum] = detectedNodes->getNodePseudo(var);
                    resultCheck[INTNum] = INTScore;
                    INTNum++;
                }
            }
        }
    }

    InterTest intertTest = InterTest(INTNum);

    for (int var = 0; var < INTNum; ++var) {
        intertTest.addInterValue(resultPseudo[var], resultCheck[var]);
    }

    return intertTest;
}

static double maxSA = 1000;

double LegacyChecks::SuddenAppearenceCheck(Coord senderPosition,
        Coord receiverPosition) {
    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);

    if(distance < maxSA){
        maxSA = distance;
    }
    if (distance < MAX_SA_RANGE) {
        return 0; //distance
    } else {
        return 1;
    }
}

static double maxPPC = 0;

double LegacyChecks::PositionPlausibilityCheck(Coord senderPosition,
        double senderSpeed) {
    if (senderSpeed <= MAX_NON_ROUTE_SPEED) {
        return 1;
    }

    double distance = LinkC->calculateDistance(senderPosition, 50, 50);

    if(distance>maxPPC){
        maxPPC = distance;
    }

    if (distance > MAX_DISTANCE_FROM_ROUTE) {
        return 0;
    } else {
        return 1;
    }

    double Intersection = 0;
    ObstacleControl* obstacles = ObstacleControlAccess().getIfExists();
    for (double var = 0.5; var <= 2; var = var + 0.5) {
        Intersection = Intersection
                + obstacles->calculateInsersion(senderPosition, var, var);
    }

    if (Intersection >= 5 * 2) {
        return 0; //Intersection
    } else {
        return 1;
    }
}

static double maxBF = 10;

double LegacyChecks::BeaconFrequencyCheck(double timeNew, double timeOld) {
    double timeDelta = timeNew - timeOld;
    if(timeDelta < maxBF){
        maxBF = timeDelta;
    }

    if (timeDelta < MAX_BEACON_FREQUENCY) {
        return 0;
    } else {
        return 1;
    }
}

double LegacyChecks::PositionHeadingConsistancyCheck(Coord curHeading,
        Coord curPosition, Coord oldPosition, double deltaTime,
        double curSpeed) {
    if (deltaTime < 1.1) {
        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        if (distance < 1) {
            return 1;
        }

        if (curSpeed < 1) {
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

        if (MAX_HEADING_CHANGE < angleDelta) {
            return 0; //  angleDelta - MAX_HEADING_CHANGE
        } else {
            return 1;
        }
    } else {
        return 1;
    }
}

BsmCheck LegacyChecks::CheckBSM(BasicSafetyMessage *bsm,
        NodeTable *detectedNodes) {
    BsmCheck bsmCheck = BsmCheck();

    unsigned long senderPseudonym = bsm->getSenderPseudonym();
    Coord senderPos = bsm->getSenderPos();
    Coord senderPosConfidence = bsm->getSenderPosConfidence();

    NodeHistory * senderNode = detectedNodes->getNodeHistoryAddr(senderPseudonym);

    bsmCheck.setRangePlausibility(
            RangePlausibilityCheck(myPosition, bsm->getSenderPos()));

    bsmCheck.setSpeedPlausibility(
            SpeedPlausibilityCheck(
                    mdmLib.calculateSpeed(bsm->getSenderSpeed())));


    if (detectedNodes->getNodeHistoryAddr(senderPseudonym)->getBSMNum() > 0) {

        bsmCheck.setPositionConsistancy(
                PositionConsistancyCheck(senderPos,
                        senderNode->getLatestBSMAddr()->getSenderPos(),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode->getLatestBSMAddr())));

        bsmCheck.setSpeedConsistancy(
                SpeedConsistancyCheck(
                        mdmLib.calculateSpeed(bsm->getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                senderNode->getLatestBSMAddr()->getSenderSpeed()),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode->getLatestBSMAddr())));

        bsmCheck.setPositionSpeedConsistancy(
                PositionSpeedConsistancyCheck(senderPos,
                        senderNode->getLatestBSMAddr()->getSenderPos(),
                        mdmLib.calculateSpeed(bsm->getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                senderNode->getLatestBSMAddr()->getSenderSpeed()),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode->getLatestBSMAddr())));

        bsmCheck.setBeaconFrequency(
                BeaconFrequencyCheck(bsm->getArrivalTime().dbl(),
                        senderNode->getLatestBSMAddr()->getArrivalTime().dbl()));

        bsmCheck.setPositionHeadingConsistancy(
                PositionHeadingConsistancyCheck(bsm->getSenderHeading(),
                        bsm->getSenderPos(),
                        senderNode->getLatestBSMAddr()->getSenderPos(),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode->getLatestBSMAddr()),
                        mdmLib.calculateSpeed(bsm->getSenderSpeed())));




    } else {
        bsmCheck.setSuddenAppearence(
                SuddenAppearenceCheck(senderPos, myPosition));
    }

    bsmCheck.setPositionPlausibility(
            PositionPlausibilityCheck(senderPos,
                    mdmLib.calculateSpeed(bsm->getSenderSpeed())));


    bsmCheck.setIntersection(MultipleIntersectionCheck(detectedNodes, bsm));
/*
    std::cout<<"-------------------------------------------------------"<<"\n";
    std::cout<<"maxRange:"<<maxRange<<"\n";
    std::cout<<"maxPC:"<<maxPC<<"\n";
    std::cout<<"maxSP:"<<maxSP<<"\n";
    std::cout<<"maxSCaccel:"<<maxSCaccel<<"\n";
    std::cout<<"maxSCdecel:"<<maxSCdecel<<"\n";
    std::cout<<"PSCminDeltaMax:"<<PSCminDeltaMax<<"\n";
    std::cout<<"PSCmaxDeltaMin:"<<PSCmaxDeltaMin<<"\n";
    std::cout<<"maxSA:"<<maxSA<<"\n";
    std::cout<<"maxPPC:"<<maxPPC<<"\n";
    std::cout<<"maxBF:"<<maxBF<<"\n";
    std::cout<<"-------------------------------------------------------"<<"\n";
*/
    return bsmCheck;
}

