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

#include "InterTest.h"

InterTest::InterTest() {
    interNum = 0;
    currentNum = 0;
    interId = new int[interNum];
    interValue = new double[interNum];
}

InterTest::InterTest(int interNum) {
    this->interNum = interNum;
    currentNum = 0;
    interId = new int[interNum];
    interValue = new double[interNum];
}

void InterTest::addInterValue(int id, double value) {
    interId[currentNum] = id;
    interValue[currentNum] = value;
    currentNum++;
}

int InterTest::getInterNum() {
    return interNum;
}

int InterTest::getInterId(int index) {
    return interId[index];
}

double InterTest::getInterValue(int index) {
    return interValue[index];
}

int InterTest::getIdIndex(int id) {
    for (int var = 0; var < currentNum; ++var) {
        if(getInterId(var) == id){
            return var;
        }
    }
    return -1;
}

