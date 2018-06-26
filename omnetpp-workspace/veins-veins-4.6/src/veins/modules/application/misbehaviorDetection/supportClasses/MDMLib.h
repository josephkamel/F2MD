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

#ifndef __VEINS_MDMLib_H_
#define __VEINS_MDMLib_H_

#include <omnetpp.h>
#include <veins/modules/application/misbehaviorDetection/supportClasses/rectIntLib/RectIntLib.h>

#include "ellipseIntLib/EllipseIntLib.h"

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#define MAX_CONFIDENCE_RANGE 10 //3
#define MAX_PLAUSIBLE_RANGE 417 // 415
#define MIN_INT_DIST 0.9 // 3.292091

#define MIN_PSS -5.1 // -5.0377 position speed shift
#define MAX_PSS 5.8 // 5.73402
#define MAX_TIME_DELTA 1.1

#define MAX_DELTA_INTER 0.5 //1.001

#define SUDDEN_APPEARENCE_RANGE 210 // AggrigationApp420/2

#define MAX_CONSISTANT_DISTANCE 43 // 42.77
#define MAX_PLAUSIBLE_SPEED 43 // 42.77

#define MAX_PLAUSIBLE_ACCEL 3.0 // 3.0
#define MAX_PLAUSIBLE_DECEL 4.5 // 4.5

#define MAX_BEACON_FREQUENCY 0.99 //0.99998

#define MAX_NON_ROUTE_SPEED 6 //3

#define MAX_HEADING_CHANGE 90 //90

#define DELTA_BSM_TIME 5 // application


#define DELTA_REPORT_TIME 5 // history report

#define POS_HEADING_TIME 1.1

#define MAXBSMLENGTH 20
#define MAXMDMLENGTH 20
#define MAXNODESLENGTH 50

#define MAXTARGETLENGTH 1000
#define MAXTARGETTIME 2

#define MAXACCUSEDLENGTH 1000
#define MAXACCUSEDTTIME 2


class MDMLib {

private:
    void countCircles(double rc, double rl, double rs);
    double calculateCircles(double dl, double ds);


    double importanceFactor(double r1, double r2, double d);

public:
    double gaussianSum(double x, double sig);
    double boundedGaussianSum(double x1, double x2, double sig);

    double calculateDistance(Coord, Coord);
    double calculateSpeed(Coord Speed);

    double calculatePolynom(long double coof[],const int coofNum, double x);
    double calculateHeadingAngle(Coord heading);
    double calculateDeltaTime(BasicSafetyMessage bsm1, BasicSafetyMessage bsm2);
    double calculateCircleSegment(double radius, double intDistance);
    double calculateCircleCircleIntersection(double r1, double r2, double d);
    double SegmentSegmentFactor(double d, double r1, double r2, double range);

    double CircleSegmentFactor(double d, double r1, double r2, double range);
    double CircleCircleFactor(double d, double r1, double r2, double range);
    double OneSidedCircleSegmentFactor(double d, double r1, double r2,
            double range);
    double CircleIntersectionFactor(double conf1, double conf2, double d,
            double initRadius);



    double RectRectFactor(Coord c1, Coord c2, double heading1, double heading2,
            Coord size1, Coord size2);

    double EllipseEllipseIntersectionFactor(Coord pos1, Coord posConf1, Coord pos2, Coord posConf2, double heading1,
            double heading2, Coord size1, Coord size2);

};

#endif
