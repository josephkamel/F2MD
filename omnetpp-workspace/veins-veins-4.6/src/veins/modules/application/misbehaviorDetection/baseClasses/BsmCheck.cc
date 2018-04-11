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

#include "BsmCheck.h"

BsmCheck::BsmCheck() {
     rangePlausibility = 1;
     speedConsistancy = 1;
     positionConsistancy = 1;
     speedPlausibility = 1;
     positionSpeedConsistancy = 1;
     intersection = InterTest();
     suddenAppearence = 1;
     beaconFrequency = 1;
     positionPlausibility = 1;

     reported = false;
}

double BsmCheck::getRangePlausibility() {
    return rangePlausibility;
}

double BsmCheck::getPositionSpeedConsistancy() {
    return positionSpeedConsistancy;
}

double BsmCheck::getPositionConsistancy() {
    return positionConsistancy;
}

double BsmCheck::getSpeedConsistancy() {
    return speedConsistancy;
}

double BsmCheck::getSpeedPlausibility() {
    return speedPlausibility;
}

InterTest BsmCheck::getIntersection() {
    return intersection;
}

double BsmCheck::getSuddenAppearence() {
    return suddenAppearence;
}

double BsmCheck::getBeaconFrequency() {
    return beaconFrequency;
}

double BsmCheck::getPositionPlausibility() {
    return positionPlausibility;
}

bool BsmCheck::getReported() {
    return reported;
}

void BsmCheck::setRangePlausibility(double rangePlausibility) {
    this->rangePlausibility = rangePlausibility;
}

void BsmCheck::setPositionConsistancy(double positionConsistancy) {
    this->positionConsistancy = positionConsistancy;
}

void BsmCheck::setSpeedConsistancy(double speedConsistancy) {
    this->speedConsistancy = speedConsistancy;
}

void BsmCheck::setSpeedPlausibility(double speedPlausibility) {
    this->speedPlausibility = speedPlausibility;
}

void BsmCheck::setIntersection(InterTest intersection) {
    this->intersection = intersection;
}

void BsmCheck::setSuddenAppearence(double suddenAppearence) {
    this->suddenAppearence = suddenAppearence;
}

void BsmCheck::setBeaconFrequency(double beaconFrequency) {
    this->beaconFrequency = beaconFrequency;
}

void BsmCheck::setPositionPlausibility(double positionPlausibility) {
    this->positionPlausibility = positionPlausibility;
}

void BsmCheck::setPositionSpeedConsistancy(double positionSpeedConsistancy) {
    this->positionSpeedConsistancy = positionSpeedConsistancy;
}

void BsmCheck::setReported(bool reported) {
    this->reported = reported;
}


