//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

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
