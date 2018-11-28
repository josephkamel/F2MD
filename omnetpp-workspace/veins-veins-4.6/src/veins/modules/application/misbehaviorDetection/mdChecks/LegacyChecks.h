/*******************************************************************************
* @author  Joseph Kamel 
* @email   josephekamel@gmail.com
* @date    28/11/2018
* @version 1.1
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_MDModule_H_
#define __VEINS_MDModule_H_

#include <tuple>

#include <omnetpp.h>

#include "../baseClasses/NodeTable.h"
#include "../supportClasses/MDMLib.h"
#include "../supportClasses/NetworkLinksLib/LinkControl.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "../mdReport/MDReport.h"
#include "../mdStats/MDStatistics.h"

using namespace Veins;
using namespace omnetpp;

class LegacyChecks {

private:

    unsigned long myPseudonym;
    Coord myPosition;
    Coord mySpeed;
    Coord mySize;
    Coord myHeading;

    MDMLib mdmLib;

    LinkControl* LinkC;

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

    InterTest MultipleIntersectionCheck(NodeTable * detectedNodes,
            BasicSafetyMessage * bsm);

public:

    LegacyChecks(unsigned long myPseudonym, Coord myPosition, Coord mySpeed,Coord mySize, Coord myHeading, LinkControl* LinkC);
    BsmCheck CheckBSM(BasicSafetyMessage * bsm, NodeTable * detectedNodes);

};

#endif
