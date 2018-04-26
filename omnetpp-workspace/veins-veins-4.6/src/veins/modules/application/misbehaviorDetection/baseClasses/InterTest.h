/*******************************************************************************
* @author  Joseph Kamel 
* @email   joseph.kamel@gmail.com
* @date    11/04/2018
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_InterTest_H_
#define __VEINS_InterTest_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;

class InterTest {
    private:
        int interNum;
        int currentNum;
        int* interId;
        double* interValue;

    public:
        InterTest();
        InterTest(int interNum);
        int getInterNum();
        void addInterValue(int id, double value);
        int getInterId(int index);
        double getInterValue(int index);

        int getIdIndex(int id);

    };

#endif
