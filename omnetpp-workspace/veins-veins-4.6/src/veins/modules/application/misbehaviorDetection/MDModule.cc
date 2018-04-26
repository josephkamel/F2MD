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

#include "MDModule.h"

#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::max

#include "supportClasses/Printable.h"

using namespace std;
using namespace boost;

static char const *name = "cumulV1";
static char const *nametemp = "tempV1";

static Printable prntLong(name);
static Printable prntTemp(nametemp);

MDModule::MDModule(int myId, Coord myPosition, Coord mySpeed, Coord mySize,
        Coord myHeading) {
    this->myId = myId;
    this->myPosition = myPosition;
    this->mySpeed = mySpeed;
    this->mySize = mySize;
    this->myHeading = myHeading;
}

double MDModule::RangePlausibilityCheck(Coord senderPosition,
        Coord receiverPosition) {
    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);

    if (distance < MAX_PLAUSIBLE_RANGE) {
        return 1;
    } else {
        return 0;//distance
    }
}

double MDModule::PositionConsistancyCheck(Coord curPosition, Coord oldPosition,
        double time) {
    double distance = mdmLib.calculateDistance(curPosition, oldPosition);

    if (distance < MAX_CONSISTANT_DISTANCE * time) {
        return 1;
    } else {
        return 0;//distance
    }
}

double MDModule::SpeedConsistancyCheck(double curSpeed, double oldspeed,
        double time) {
    double speedDelta = curSpeed - oldspeed;

    if (speedDelta > 0) {
        if (speedDelta < MAX_PLAUSIBLE_ACCEL * time) {
            return 1;
        } else {
            return 0;//distance
        }
    } else {
        if (speedDelta < MAX_PLAUSIBLE_DECEL * time) {
            return 1;
        } else {
            return 0;//distance
        }
    }

}

//static double minDeltaMax = 10;
//static double maxDeltaMin = 0;

double MDModule::PositionSpeedConsistancyCheck(Coord curPosition,
        Coord oldPosition, double curSpeed, double oldspeed, double time) {
    if (time < MAX_TIME_DELTA) {
        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        double theoreticalSpeed = distance / time;
        //double realspeed = fabs(curSpeed + oldspeed)/2;
        double maxspeed = std::max(curSpeed, oldspeed);
        double minspeed = std::min(curSpeed, oldspeed);

        double deltaMax = maxspeed - theoreticalSpeed;
        double deltaMin = minspeed - theoreticalSpeed;

//        if (deltaMax < minDeltaMax) {
//            minDeltaMax = deltaMax;
//        }
//        if (deltaMin > maxDeltaMin) {
//            maxDeltaMin = deltaMin;
//        }
//
//        std::cout<<"minDeltaMax:"<<minDeltaMax<<'\n';
//        std::cout<<"maxDeltaMin:"<<maxDeltaMin<<'\n';

        if (deltaMax < MIN_PSS) {
            return 0; // deltaMax - MIN_PSS
        } else {
            if (deltaMin > MAX_PSS) {
                return 0;// deltaMin - MAX_PSS
            } else {
                return 1;
            }
        }
    } else {
        return 1;
    }
}

double MDModule::SpeedPlausibilityCheck(double speed) {

    if (fabs(speed) < MAX_PLAUSIBLE_SPEED) {
        return 1;
    } else {
        return 0; // fabs(speed)
    }
}

double MDModule::IntersectionCheck(Coord nodePosition1, Coord nodeSize1,
        Coord head1, Coord nodePosition2, Coord nodeSize2, Coord head2) {

    double heading1 = mdmLib.calculateHeadingAngle(head1);
    double heading2 = mdmLib.calculateHeadingAngle(head2);

    double distance = mdmLib.calculateDistance(nodePosition1, nodePosition2);

    double inter = mdmLib.RectRectFactor(nodePosition1, nodePosition2, heading1,
            heading2, nodeSize1, nodeSize2);

    if (inter > 0) {
        return 0; //inter
    } else {
        return 1;
    }

}

InterTest MDModule::MultipleIntersectionCheck(NodeTable detectedNodes,
        BasicSafetyMessage bsm) {
    int senderId = bsm.getSenderAddress();
    Coord senderPos = bsm.getSenderPos();

    NodeHistory senderNode = detectedNodes.getNodeHistory(senderId);
    NodeHistory varNode;

    std::map<std::string, double> result;
    double INTScore = 0;
    int INTNum = 0;

    INTScore = IntersectionCheck(myPosition, mySize, myHeading, senderPos,
            Coord(bsm.getSenderWidth(), bsm.getSenderLength()),
            bsm.getSenderHeading());
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

                INTScore = IntersectionCheck(varNode.getSenderPos(0),
                        varNode.getSenderSize(0), varNode.getSenderHeading(0),
                        senderPos,
                        Coord(bsm.getSenderWidth(), bsm.getSenderLength()),
                        bsm.getSenderHeading());
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

double MDModule::SuddenAppearenceCheck(Coord senderPosition,
        Coord receiverPosition) {
    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);
    if (distance < SUDDEN_APPEARENCE_RANGE) {
        return 0; //distance
    } else {
        return 1;
    }
}

double MDModule::PositionPlausibilityCheck(Coord senderPosition,
        double senderSpeed) {
    if (senderSpeed <= MAX_NON_ROUTE_SPEED) {
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

double MDModule::BeaconFrequencyCheck(double timeNew, double timeOld) {
    double timeDelta = timeNew - timeOld;
    if (timeDelta < MAX_BEACON_FREQUENCY) {
        return 0;
    } else {
        return 1;
    }
}

double MDModule::PositionHeadingConsistancyCheck(Coord curHeading,
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

std::map<std::string, double> MDModule::CheckBSMold(NodeTable detectedNodes,
        int senderId) {

    std::map<std::string, double> result;

    NodeHistory senderNode = detectedNodes.getNodeHistory(senderId);

    result["RangePlausibility"] = RangePlausibilityCheck(myPosition,
            senderNode.getLatestBSM().getSenderPos());
    result["PositionPlausibility"] = PositionPlausibilityCheck(
            senderNode.getSenderPos(0), senderNode.getSenderSpeed(0));
    result["SpeedPlausibility"] = SpeedPlausibilityCheck(
            senderNode.getSenderSpeed(0));

    if (detectedNodes.getNodeHistory(senderId).getBSMNum() > 1) {
        result["PositionConsistancy"] = PositionConsistancyCheck(
                senderNode.getSenderPos(0), senderNode.getSenderPos(1),
                senderNode.getDeltaTime(0, 1));

        result["SpeedConsistancy"] = SpeedConsistancyCheck(
                senderNode.getSenderSpeed(0), senderNode.getSenderSpeed(1),
                senderNode.getDeltaTime(0, 1));

        result["PositionSpeedConsistancy"] = PositionSpeedConsistancyCheck(
                senderNode.getSenderPos(0), senderNode.getSenderPos(1),
                senderNode.getSenderSpeed(0), senderNode.getSenderSpeed(1),
                senderNode.getDeltaTime(0, 1));

        result["BeaconFrequency"] = BeaconFrequencyCheck(
                senderNode.getArrivalTime(0), senderNode.getArrivalTime(1));

        result["PositionHeadingConsistancy"] = PositionHeadingConsistancyCheck(
                senderNode.getSenderHeading(0), senderNode.getSenderPos(0),
                senderNode.getSenderPos(1),
                mdmLib.calculateDeltaTime(senderNode.getBSM(0),
                        senderNode.getBSM(1)), senderNode.getSenderSpeed(0));

    } else {
        result["SuddenAppearence"] = SuddenAppearenceCheck(myPosition,
                senderNode.getLatestBSM().getSenderPos());
    }

    result["INTId_0"] = myId;
    result["INTCheck_0"] = IntersectionCheck(myPosition, mySize, myHeading,
            senderNode.getSenderPos(0), senderNode.getSenderSize(0),
            senderNode.getSenderHeading(0));

    NodeHistory varNode;
    double INTScore = 0;
    int INTNum = 1;

    char num_string[32];
    char INTId_string[64] = "INTId_";
    char INTCheck_string[64] = "INTCheck_";

    for (int var = 0; var < detectedNodes.getNodesNum(); ++var) {
        if (detectedNodes.getNodeId(var) != senderId) {
            if (detectedNodes.getDeltaTime(detectedNodes.getNodeId(var),
                    senderId) < MAX_DELTA_INTER) {
                varNode = detectedNodes.getNodeHistory(
                        detectedNodes.getNodeId(var));
                INTScore = IntersectionCheck(varNode.getSenderPos(0),
                        varNode.getSenderSize(0), varNode.getSenderHeading(0),
                        senderNode.getSenderPos(0), senderNode.getSenderSize(0),
                        senderNode.getSenderHeading(0));
                if (INTScore > 0) {
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

    return result;
}

BsmCheck MDModule::CheckBSM(BasicSafetyMessage bsm, NodeTable detectedNodes) {

    BsmCheck bsmCheck = BsmCheck();

    int senderId = bsm.getSenderAddress();
    Coord senderPos = bsm.getSenderPos();
    Coord senderPosConfidence = bsm.getSenderPosConfidence();

    NodeHistory senderNode = detectedNodes.getNodeHistory(senderId);

    bsmCheck.setRangePlausibility(RangePlausibilityCheck(myPosition,
            bsm.getSenderPos()));

    bsmCheck.setSpeedPlausibility(
            SpeedPlausibilityCheck(
                    mdmLib.calculateSpeed(bsm.getSenderSpeed())));

    if (detectedNodes.getNodeHistory(senderId).getBSMNum() > 0) {

        bsmCheck.setPositionConsistancy(
                PositionConsistancyCheck(senderPos,
                        senderNode.getLatestBSM().getSenderPos(),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode.getLatestBSM())));

        bsmCheck.setSpeedConsistancy(
                SpeedConsistancyCheck(
                        mdmLib.calculateSpeed(bsm.getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                senderNode.getLatestBSM().getSenderSpeed()),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode.getLatestBSM())));

        bsmCheck.setPositionSpeedConsistancy(
                PositionSpeedConsistancyCheck(senderPos,
                        senderNode.getLatestBSM().getSenderPos(),
                        mdmLib.calculateSpeed(bsm.getSenderSpeed()),
                        mdmLib.calculateSpeed(
                                senderNode.getLatestBSM().getSenderSpeed()),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode.getLatestBSM())));

        bsmCheck.setBeaconFrequency(
                BeaconFrequencyCheck(bsm.getArrivalTime().dbl(),
                        senderNode.getLatestBSM().getArrivalTime().dbl()));

        bsmCheck.setPositionHeadingConsistancy(
                PositionHeadingConsistancyCheck(bsm.getSenderHeading(),
                        bsm.getSenderPos(),
                        senderNode.getLatestBSM().getSenderPos(),
                        mdmLib.calculateDeltaTime(bsm,
                                senderNode.getLatestBSM()),
                        mdmLib.calculateSpeed(bsm.getSenderSpeed())));

    } else {
        bsmCheck.setSuddenAppearence(
                SuddenAppearenceCheck(senderPos, myPosition));
    }

    bsmCheck.setPositionPlausibility(
            PositionPlausibilityCheck(senderPos,
                    mdmLib.calculateSpeed(bsm.getSenderSpeed())));

    bsmCheck.setIntersection(MultipleIntersectionCheck(detectedNodes, bsm));

    return bsmCheck;
}

std::tuple<bool, MBReport> MDModule::CheckNodeByThreshold(
        BasicSafetyMessage bsm, BsmCheck bsmCheck, NodeTable detectedNodes,
        double mbType) {

    bool checkFailed = false;

    MBReport mbReport;

    prntLong.incAll(mbType);
    prntTemp.incAll(mbType);

    int senderId = bsm.getSenderAddress();

    if (bsmCheck.getRangePlausibility() <= 0) {
        checkFailed = true;
        prntLong.incFlags("RangePlausibility", mbType);
        prntTemp.incFlags("RangePlausibility", mbType);
    }

    if (bsmCheck.getPositionConsistancy() <= 0) {
        checkFailed = true;
        prntLong.incFlags("PositionConsistancy", mbType);
        prntTemp.incFlags("PositionConsistancy", mbType);
    }

    if (bsmCheck.getPositionSpeedConsistancy() <= 0) {
        checkFailed = true;
        prntLong.incFlags("PositionSpeedConsistancy", mbType);
        prntTemp.incFlags("PositionSpeedConsistancy", mbType);
    }

    if (bsmCheck.getSpeedConsistancy() <= 0) {
        checkFailed = true;
        prntLong.incFlags("SpeedConsistancy", mbType);
        prntTemp.incFlags("SpeedConsistancy", mbType);
    }

    if (bsmCheck.getSpeedPlausibility() <= 0) {
        checkFailed = true;
        prntLong.incFlags("SpeedPlausibility", mbType);
        prntTemp.incFlags("SpeedPlausibility", mbType);
    }

    if (bsmCheck.getPositionPlausibility() <= 0) {
        checkFailed = true;
        prntLong.incFlags("PositionPlausibility", mbType);
        prntTemp.incFlags("PositionPlausibility", mbType);
    }

    if (bsmCheck.getBeaconFrequency() <= 0) {
        checkFailed = true;
        prntLong.incFlags("BeaconFrequency", mbType);
        prntTemp.incFlags("BeaconFrequency", mbType);
    }

    if (bsmCheck.getSuddenAppearence() <= 0) {
        prntLong.incFlags("SuddenAppearence", mbType);
        prntTemp.incFlags("SuddenAppearence", mbType);
    }

    if (bsmCheck.getPositionHeadingConsistancy() <= 0) {
        checkFailed = true;
        prntLong.incFlags("PositionHeadingConsistancy", mbType);
        prntTemp.incFlags("PositionHeadingConsistancy", mbType);
    }

    bool maxInterFound = false;
    bool minInterFound = false;

    InterTest inter = bsmCheck.getIntersection();
    for (int var = 0; var < inter.getInterNum(); ++var) {

        double IT = inter.getInterValue(var);

        if (IT <= 0) {
            checkFailed = true;
            if (!minInterFound) {
                prntLong.incFlags("Intersection", mbType);
                prntTemp.incFlags("Intersection", mbType);
                minInterFound = true;
            }
        }

    }
    if (checkFailed) {
        mbReport.setGenerationTime(simTime().dbl());
        mbReport.setSenderId(myId);
        mbReport.setReportedId(senderId);
        mbReport.setMbType(mbType);

        prntLong.incCumulFlags(mbType);
        prntTemp.incCumulFlags(mbType);
    }

    return std::make_tuple(checkFailed, mbReport);
}

void MDModule::SendReport(MDAuthority * mdAuthority, MBReport mbReport) {
    char nameV1[32] = "mdaV1";
    mdAuthority->sendReport(nameV1, mbReport);
}

static bool init = true;
void MDModule::saveLine(std::string path, std::string serial, double density,
        double deltaT) {
    char fileNameCumul[] = "mdmResultV1Cumul.txt";
    char fileNameTemp[] = "mdmResultV1Temp.txt";

    char outChar[1024];
    char directoryPathGen[1024] = "";
    char filePathGen[1024] = "";
    const char * pathChar = path.c_str();
    const char * serialChar = serial.c_str();
    strcat(directoryPathGen, pathChar);
    strcat(directoryPathGen, serialChar);

    struct stat info;

    if (stat(directoryPathGen, &info) != 0) {
//        printf( "cannot access %s\n", directoryPathGen );
        mkdir(directoryPathGen, 0777);
//          S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH
//        if (0 != mkdir(directoryPathGen,0777)) {
//          printf("When executing: mkdir(\"%s\")\n", directoryPathGen);
//          perror("mkdir");
//          exit(1);
//        }

    } else if (info.st_mode & S_IFDIR) {
        // S_ISDIR() doesn't exist on my windows
//        printf( "%s is a directory\n", directoryPathGen );
    } else {
//        printf( "%s is no directory\n", directoryPathGen );
        mkdir(directoryPathGen, 0777);
    }

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameCumul);

    prntLong.getPrintable(outChar, density, deltaT);
    prntLong.writeFile(filePathGen, outChar, init);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameTemp);

    prntTemp.getPrintable(outChar, density, deltaT);
    prntTemp.writeFile(filePathGen, outChar, init);

    if (init) {
        init = false;
    }
}

void MDModule::resetTempFlags() {
    prntTemp.resetAll();
}

void MDModule::resetAllFlags() {
    prntLong.resetAll();
    prntTemp.resetAll();
}
