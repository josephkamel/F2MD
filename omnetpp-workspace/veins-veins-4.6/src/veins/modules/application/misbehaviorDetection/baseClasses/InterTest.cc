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

