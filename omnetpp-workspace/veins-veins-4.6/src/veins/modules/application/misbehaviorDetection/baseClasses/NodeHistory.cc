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

#include "NodeHistory.h"

NodeHistory::NodeHistory() {
    nodeId = 0;
    bsmNum = 0;
}

NodeHistory::NodeHistory(int id) {
    nodeId = id;
    bsmNum = 0;
}

NodeHistory::NodeHistory(int id, BasicSafetyMessage bsm) {
    nodeId = id;
    bsmNum = 0;
    addBSM(bsm);
}

BasicSafetyMessage NodeHistory::getLatestBSM() {
    return bsmList[0];
}

BasicSafetyMessage NodeHistory::getSecondLatestBSM() {
    return bsmList[1];
}

BasicSafetyMessage NodeHistory::getBSM(int index) {
    return bsmList[index];
}


double NodeHistory::getArrivalTime(int index) {
    return bsmList[index].getArrivalTime().dbl();
}

Coord NodeHistory::getSenderPos(int index) {
    return bsmList[index].getSenderPos();
}

Coord NodeHistory::getSenderSize(int index) {
    return Coord(bsmList[index].getSenderWidth(),
            bsmList[index].getSenderLength());
}

double NodeHistory::getSenderSpeed(int index) {
    return sqrt(
            pow(bsmList[index].getSenderSpeed().x, 2.0)
                    + pow(bsmList[index].getSenderSpeed().y, 2.0)
                    + pow(bsmList[index].getSenderSpeed().z, 2.0));
}

Coord NodeHistory::getSenderHeading(int index) {
    return bsmList[index].getSenderHeading();
}

double NodeHistory::getDeltaTime(int index1, int index2) {
    return fabs(
            bsmList[index1].getArrivalTime().dbl()
                    - bsmList[index2].getArrivalTime().dbl());
}

BasicSafetyMessage* NodeHistory::getBSMList() {
    return bsmList;
}

int NodeHistory::getBSMNum() {
    return bsmNum;
}

void NodeHistory::addBSM(BasicSafetyMessage bsm) {
    if (bsmNum < MAXBSMLENGTH) {
        bsmNum++;
    }
    for (int var = bsmNum - 1; var > 0; --var) {
        bsmList[var] = bsmList[var - 1];
    }
    bsmList[0] = bsm;
}
