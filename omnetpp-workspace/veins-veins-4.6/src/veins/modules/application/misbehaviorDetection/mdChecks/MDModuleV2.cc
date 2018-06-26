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

#include "MDModuleV2.h"

#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace boost;

MDModuleV2::MDModuleV2(int myId, Coord myPosition, Coord myPositionConfidence,
        Coord myHeading, Coord myHeadingConfidence, Coord mySize) {
    this->myId = myId;
    this->myPosition = myPosition;
    this->myPositionConfidence = myPositionConfidence;
    this->myHeading = myHeading;
    this->myHeadingConfidence = myHeadingConfidence;
    this->mySize = mySize;
}

double MDModuleV2::RangePlausibilityCheck(Coord receiverPosition,
        Coord receiverPositionConfidence, Coord senderPosition,
        Coord senderPositionConfidence) {

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
        double factor = (fabs(speedConfidence) / 2
                + (MAX_PLAUSIBLE_SPEED - fabs(speed))) / fabs(speedConfidence);

        return factor;
    }
}

double MDModuleV2::PositionSpeedConsistancyCheckOld(Coord curPosition,
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
//                << " oldR:" << oldR << " maxspeed - MIN_PSS:" << maxspeed - MIN_PSS
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

        double factor = 1;

        if (minfactor < maxfactor) {
            factor = minfactor;
        } else {
            factor = maxfactor;
        }

        factor = (factor - 0.5) * 2;
        factor = mdmLib.gaussianSum(factor, (1.0 / 4.5));
        if (factor > 0.75) {
            factor = 1;
        }
      //  std::cout << " Old Min:" << minfactor << " Max:" << maxfactor << '\n';
        return factor;

    } else {
        return 1;
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

        double maxfactor = mdmLib.OneSidedCircleSegmentFactor(
                maxspeed - theoreticalSpeed, curR, oldR, MAX_PSS);

        double minfactor = mdmLib.OneSidedCircleSegmentFactor(
                theoreticalSpeed - minspeed, curR, oldR, - MIN_PSS);

        double factor = 1;

        if (minfactor < maxfactor) {
            factor = minfactor;
        } else {
            factor = maxfactor;
        }

        factor = (factor - 0.5) * 2;
        factor = mdmLib.gaussianSum(factor, (1.0 / 4.5));
        if (factor > 0.75) {
            factor = 1;
        }

        double factorOld = PositionSpeedConsistancyCheckOld(curPosition,
                curPositionConfidence, oldPosition, oldPositionConfidence,
                curSpeed, curSpeedConfidence, oldspeed, oldSpeedConfidence,
                time);

//        std::cout << " Min:" << minfactor << " Max:" << maxfactor << '\n';
//
//        if(factorOld !=factor){
//            std::cout << " factorOld:" << factorOld << " factor:" << factor << '\n';
//            std::cout << "============================================" << '\n';
//        }


        return factor;

    } else {
        return 1;
    }
}

double MDModuleV2::IntersectionCheck(Coord nodePosition1,
        Coord nodePositionConfidence1, Coord nodePosition2,
        Coord nodePositionConfidence2, Coord nodeHeading1, Coord nodeHeading2,
        Coord nodeSize1, Coord nodeSize2) {

//    double distance = mdmLib.calculateDistance(nodePosition1, nodePosition2);
//    double intFactor = mdmLib.CircleIntersectionFactor(
//            nodePositionConfidence1.x, nodePositionConfidence2.x, distance,
//            MIN_INT_DIST);
//    intFactor = 1 - intFactor;
//    return intFactor;

    double heading1 = mdmLib.calculateHeadingAngle(nodeHeading1);
    double heading2 = mdmLib.calculateHeadingAngle(nodeHeading2);

    double intFactor2 = mdmLib.EllipseEllipseIntersectionFactor(nodePosition1,
            nodePositionConfidence1, nodePosition2, nodePositionConfidence2,
            heading1, heading2, nodeSize1, nodeSize2);
    intFactor2 = 1 - intFactor2;
    double factor = intFactor2;

    return factor;

}

InterTest MDModuleV2::MultipleIntersectionCheck(NodeTable detectedNodes,
        BasicSafetyMessage bsm) {
    int senderId = bsm.getSenderAddress();
    Coord senderPos = bsm.getSenderPos();
    Coord senderPosConfidence = bsm.getSenderPosConfidence();
    Coord senderHeading = bsm.getSenderHeading();

    Coord senderSize = Coord(bsm.getSenderWidth(), bsm.getSenderLength());

    NodeHistory senderNode = detectedNodes.getNodeHistory(senderId);
    NodeHistory varNode;

    std::map<std::string, double> result;
    double INTScore = 0;
    int INTNum = 0;

    INTScore = IntersectionCheck(myPosition, myPositionConfidence, senderPos,
            senderPosConfidence, myHeading, senderHeading, mySize, senderSize);
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

                Coord varSize = Coord(varNode.getLatestBSM().getSenderWidth(),
                        varNode.getLatestBSM().getSenderLength());

                INTScore = IntersectionCheck(
                        varNode.getLatestBSM().getSenderPos(),
                        varNode.getLatestBSM().getSenderPosConfidence(),
                        senderPos, senderPosConfidence,
                        varNode.getLatestBSM().getSenderHeading(),
                        senderHeading, varSize, senderSize);
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

    double factor = (1 - (Intersection / count));

    return factor;
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

        double curFactorLow = 1;
        if (curPositionConfidence.x == 0) {
            if (angleLow <= MAX_HEADING_CHANGE) {
                curFactorLow = 1;
            } else {
                curFactorLow = 0;
            }
        } else {
            curFactorLow = mdmLib.calculateCircleSegment(
                    curPositionConfidence.x, curPositionConfidence.x + xLow)
                    / (PI * curPositionConfidence.x * curPositionConfidence.x);
        }

        double oldFactorLow = 1;
        if (oldPositionConfidence.x == 0) {
            if (angleLow <= MAX_HEADING_CHANGE) {
                oldFactorLow = 1;
            } else {
                oldFactorLow = 0;
            }
        } else {
            oldFactorLow = 1
                    - mdmLib.calculateCircleSegment(oldPositionConfidence.x,
                            oldPositionConfidence.x - xLow)
                            / (PI * oldPositionConfidence.x
                                    * oldPositionConfidence.x);
        }

        double xHigh = distance * cos(angleHigh * PI / 180);
        double curFactorHigh = 1;
        if (curPositionConfidence.x == 0) {
            if (angleHigh <= MAX_HEADING_CHANGE) {
                curFactorHigh = 1;
            } else {
                curFactorHigh = 0;
            }
        } else {
            curFactorHigh = mdmLib.calculateCircleSegment(
                    curPositionConfidence.x, curPositionConfidence.x + xHigh)
                    / (PI * curPositionConfidence.x * curPositionConfidence.x);
        }

        double oldFactorHigh = 1;
        if (oldPositionConfidence.x == 0) {
            if (angleHigh <= MAX_HEADING_CHANGE) {
                oldFactorHigh = 1;
            } else {
                oldFactorHigh = 0;
            }
        } else {
            oldFactorHigh = 1
                    - mdmLib.calculateCircleSegment(oldPositionConfidence.x,
                            oldPositionConfidence.x - xHigh)
                            / (PI * oldPositionConfidence.x
                                    * oldPositionConfidence.x);
        }

        double factor = (curFactorLow + oldFactorLow + curFactorHigh
                + oldFactorHigh) / 4;

//    if(factor<=0.0){
//
//        std::cout<<"curPos: "<<curPosition<<'\n';
//        std::cout<<"oldPos: "<<oldPosition<<'\n';
//
//        std::cout<<"relativePos: "<<relativePos<<'\n';
//
//        std::cout<<"curFactorLow: "<<curFactorLow<<'\n';
//        std::cout<<"oldFactorLow: "<<oldFactorLow<<'\n';
//        std::cout<<"curFactorHigh: "<<curFactorHigh<<'\n';
//        std::cout<<"oldFactorHigh: "<<oldFactorHigh<<'\n';
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
//
//     //   exit(0);
//    }

        factor = (factor - 0.5) * 2;
        factor = mdmLib.gaussianSum(factor, (1.0 / 4.5));
        if (factor > 0.75) {
            factor = 1;
        }

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

void MDModuleV2::SendReport(MDAuthority* mdAuthority, MDReport mbReport) {
    char nameV2[32] = "mdaV2";
    mdAuthority->sendReport(nameV2, mbReport);
}

