/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_MDModule_H_
#define __VEINS_MDModule_H_

#include <omnetpp.h>

#include "baseClasses/NodeTable.h"
#include "supportClasses/MDMLib.h"
#include "JosephVeinsApp.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "mdAuthority/MBReport.h"
#include "mdAuthority/MDAuthority.h"

using namespace Veins;
using namespace omnetpp;

class MDModule {

private:

    int myId;
    Coord myPosition;
    Coord mySpeed;
    Coord mySize;
    Coord myHeading;

    MDMLib mdmLib;

    double RangePlausibilityCheck(Coord, Coord);
    double PositionConsistancyCheck(Coord, Coord, double);
    double SpeedConsistancyCheck(double, double, double);
    double PositionSpeedConsistancyCheck(Coord,
            Coord , double , double , double );
    double SpeedPlausibilityCheck(double);
    double IntersectionCheck(Coord nodePosition1, Coord nodeSize1,
            Coord head1, Coord nodePosition2, Coord nodeSize2,
            Coord head2);
    double SuddenAppearenceCheck(Coord, Coord);
    double BeaconFrequencyCheck(double, double);
    double PositionPlausibilityCheck(Coord, double);
    double PositionHeadingConsistancyCheck(Coord curHeading,
            Coord curPosition, Coord oldPosition, double deltaTime, double curSpeed);

    void SendReport(MBReport mbReport);

public:

    MDModule(int myId, Coord myPosition, Coord mySpeed,Coord mySize, Coord myHeading);
    std::map<std::string, double> CheckBSM(NodeTable detectedNodes,int senderId);
    void reportMB(std::map<std::string, double> result, int senderId , double mbType);

    void saveLine(std::string path, std::string serial, double density, double deltaT) ;

    void resetTempFlags();
    void resetAllFlags();

};

#endif
