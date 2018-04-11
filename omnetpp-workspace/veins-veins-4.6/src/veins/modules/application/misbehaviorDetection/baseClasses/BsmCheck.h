/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_BsmCheck_H_
#define __VEINS_BsmCheck_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "InterTest.h"
using namespace omnetpp;

class BsmCheck {

//    double RangePlausibilityCheck(Coord, Coord);
//    double PositionPlausibilityCheck(Coord);
//    double PositionConsistancyCheck(Coord, Coord, double);
//    double SpeedConsistancyCheck(double, double, double);
//    double PositionSpeedConsistancyCheck(Coord, Coord, double, double, double);
//    double SpeedPlausibilityCheck(double);
//    double IntersectionCheck(Coord, Coord);
//    double SuddenAppearenceCheck(Coord, Coord);
//    double BeaconFrequencyCheck(double, double);


    private:
        double rangePlausibility;
        double speedConsistancy;
        double positionConsistancy;
        double positionSpeedConsistancy;
        double speedPlausibility;
        InterTest intersection;
        double suddenAppearence;
        double beaconFrequency;
        double positionPlausibility;

        bool reported;

    public:
        BsmCheck();

        double getRangePlausibility();
        double getPositionPlausibility();
        double getSpeedPlausibility();
        double getPositionConsistancy();
        double getSpeedConsistancy();
        double getPositionSpeedConsistancy();
        InterTest getIntersection();
        double getSuddenAppearence();
        double getBeaconFrequency();

        bool getReported();

        void setRangePlausibility(double);
        void setPositionPlausibility(double);
        void setSpeedPlausibility(double);
        void setSpeedConsistancy(double);
        void setPositionConsistancy(double);
        void setPositionSpeedConsistancy(double);
        void setIntersection(InterTest);
        void setSuddenAppearence(double);
        void setBeaconFrequency(double);

        void setReported(bool);


    };

#endif
