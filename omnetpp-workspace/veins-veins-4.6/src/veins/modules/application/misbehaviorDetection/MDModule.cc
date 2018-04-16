/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
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
        return 0;
    } else {
        return distance;
    }
}

double MDModule::PositionConsistancyCheck(Coord curPosition, Coord oldPosition,
        double time) {
    double distance = mdmLib.calculateDistance(curPosition, oldPosition);

    if (distance < MAX_CONSISTANT_DISTANCE * time) {
        return 0;
    } else {
        return distance;
    }
}

double MDModule::SpeedConsistancyCheck(double curSpeed, double oldspeed,
        double time) {
    double speedDelta = curSpeed - oldspeed;

    if (speedDelta > 0) {
        if (speedDelta < MAX_PLAUSIBLE_ACCEL * time) {
            return 0;
        } else {
            return speedDelta;
        }
    } else {
        if (speedDelta < MAX_PLAUSIBLE_DECEL * time) {
            return 0;
        } else {
            return speedDelta;
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
            return deltaMax - MIN_PSS;
        } else {
            if (deltaMin > MAX_PSS) {
                return deltaMin - MAX_PSS;
            } else {
                return 0;
            }
        }
    } else {
        return 0;
    }
}

double MDModule::SpeedPlausibilityCheck(double speed) {

    if (fabs(speed) < MAX_PLAUSIBLE_SPEED) {
        return 0;
    } else {
        return fabs(speed);
    }
}

double MDModule::IntersectionCheck(Coord nodePosition1, Coord nodeSize1,
        Coord head1, Coord nodePosition2, Coord nodeSize2,
        Coord head2) {

    double heading1 = mdmLib.calculateHeadingAngle(head1);
    double heading2 = mdmLib.calculateHeadingAngle(head2);

    double distance = mdmLib.calculateDistance(nodePosition1, nodePosition2);

    double inter = mdmLib.RectRectFactor(nodePosition1, nodePosition2, heading1,
            heading2, nodeSize1, nodeSize2);

    if (inter > 0) {

//        std::cout << " nodePosition1:" << nodePosition1 << " heading1:"
//                << heading1 <<  " nodeSize1:"<< nodeSize1<< " nodePosition2:" << nodePosition2
//                << " heading2:" << heading2 <<  " nodeSize2:"<< nodeSize2<< '\n';
//        std::cout << " inter:" << inter << " distance:" << distance << '\n';

        return inter;
    } else {
        return 0;
    }

}

double MDModule::SuddenAppearenceCheck(Coord senderPosition,
        Coord receiverPosition) {
    double distance = mdmLib.calculateDistance(senderPosition,
            receiverPosition);
    if (distance < SUDDEN_APPEARENCE_RANGE) {
        return distance;
    } else {
        return 0;
    }
}

double MDModule::PositionPlausibilityCheck(Coord senderPosition,
        double senderSpeed) {
    if (senderSpeed <= MAX_NON_ROUTE_SPEED) {
        return 0;
    }
    double Intersection = 0;
    ObstacleControl* obstacles = ObstacleControlAccess().getIfExists();
    for (double var = 0.5; var <= 2; var = var + 0.5) {
        Intersection = Intersection
                + obstacles->calculateInsersion(senderPosition, var, var);
    }
    if (Intersection >= 5*2) {
        return Intersection;
    } else {
        return 0;
    }
}

double MDModule::BeaconFrequencyCheck(double timeNew, double timeOld) {
    double timeDelta = timeNew - timeOld;
    if (timeDelta < MAX_BEACON_FREQUENCY) {
        return 1;
    } else {
        return 0;
    }
}


double MDModule::PositionHeadingConsistancyCheck(Coord curHeading,
        Coord curPosition, Coord oldPosition, double deltaTime) {
    if(deltaTime<1.1){
        double distance = mdmLib.calculateDistance(curPosition, oldPosition);
        if(distance<0.1){
            return 0;
        }

        double curHeadingAngle = mdmLib.calculateHeadingAngle(curHeading);
        Coord relativePos = Coord(curPosition.x - oldPosition.x, curPosition.y - oldPosition.y,
                curPosition.z - oldPosition.z);
        double positionAngle = mdmLib.calculateHeadingAngle(relativePos);
        double angleDelta = fabs(curHeadingAngle - positionAngle);
        if(angleDelta > 180){
            angleDelta = 360 - angleDelta;
        }

        if(MAX_HEADING_CHANGE < angleDelta){
            return angleDelta - MAX_HEADING_CHANGE;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

std::map<std::string, double> MDModule::CheckBSM(NodeTable detectedNodes,
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


        result["PositionHeadingConsistancy"] = PositionHeadingConsistancyCheck(senderNode.getSenderHeading(0),
                senderNode.getSenderPos(0),                senderNode.getSenderPos(1),
                mdmLib.calculateDeltaTime(senderNode.getBSM(0),
                        senderNode.getBSM(1)));


    } else {
        result["SuddenAppearence"] = SuddenAppearenceCheck(myPosition,
                senderNode.getLatestBSM().getSenderPos());
    }

    result["INTId_0"] = myId;
    result["INTCheck_0"] = IntersectionCheck(myPosition, mySize, myHeading,
            senderNode.getSenderPos(0), senderNode.getSenderSize(0),
            senderNode.getSenderHeading(0));

    NodeHistory varNode;
    int INTScore = 0;
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

    return result;
}

void MDModule::reportMB(std::map<std::string, double> result, int senderId,
        double mbType) {
    MBReport mbReport;

    int INTindex = 0;
    char INTId_string[64] = "INTId_";

    prntLong.incAll(mbType);
    prntTemp.incAll(mbType);

    bool checkFailed = false;

    for (std::map<std::string, double>::iterator it = result.begin();
            it != result.end(); ++it) {

        if (it->first.compare(0, 6, "INTId_") != 0
                && it->first.compare(0, 9, "INTCheck_") != 0
                && it->first.compare(0, 6, "INTNum") != 0) {
            if (it->second != 0) {
                if (it->first.compare("SuddenAppearence") != 0) {
                    std::cout << "########## " << it->first << " => " << it->second
                            << " A:" << myId << " B:" << senderId << '\n';
                    checkFailed = true;
                }
                prntLong.incFlags(it->first, mbType);
                prntTemp.incFlags(it->first, mbType);
            }
        } else {
            if (it->first.compare(0, 9, "INTCheck_") == 0) {
                if (it->second > 0) {
                    vector<string> fields;
                    split(fields, it->first, is_any_of("_"));

                    strncpy(INTId_string, "INTId_", sizeof(INTId_string));

                    strcat(INTId_string, fields[1].data());
                    checkFailed = true;

                    prntLong.incFlags("Intersection", mbType);
                    prntTemp.incFlags("Intersection", mbType);

                    std::cout << "########## " << "Intersection " << " => "
                            << it->second << " A:" << myId << " B:" << senderId
                            << " C:" << result[INTId_string] << '\n';
                }
            }
        }
    }

    if (checkFailed) {
        mbReport.setGenerationTime(simTime().dbl());
        mbReport.setSenderId(myId);
        mbReport.setReportedId(senderId);
        mbReport.setMbType(mbType);
        SendReport(mbReport);

        prntLong.incCumulFlags(mbType);
        prntTemp.incCumulFlags(mbType);
    }
}

void MDModule::SendReport(MBReport mbReport) {
    MDAuthority mdAuthority;
    mdAuthority.sendReportV1(mbReport);
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

    prntTemp.resetAll();
}