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

#ifndef __VEINS_PCPolicy_H_
#define __VEINS_PCPolicy_H_

#include <tuple>
#include <omnetpp.h>


#include "../baseClasses/NodeTable.h"
#include "../baseClasses/InterTest.h"
#include "../baseClasses/BsmCheck.h"
#include "../baseClasses/InterTest.h"

#include "../mdAuthority/MDAuthority.h"
#include "../supportClasses/MDMLib.h"
#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"

#include "../enumTypes/PseudoChangeTypes.h"
#include "../enumTypes/MbTypes.h"

using namespace Veins;
using namespace omnetpp;


class PCPolicy {
protected:
    GeneralLib genLib = GeneralLib();

    Coord* curPosition;
    int* myId;
    int* pseudoNum;
    unsigned long* myPseudonym;


    mbTypes::Mbs mbType;
    MDAuthority* mdAuthority;

public:
    unsigned long getNextPseudonym();

public:

    PCPolicy();
    void setCurPosition(Coord* curPosition);
    void setMyId(int* myId);
    void setMyPseudonym(unsigned long* myPseudonym);
    void setPseudoNum(int* pseudoNum);

    void checkPseudonymChange(pseudoChangeTypes::PseudoChange);

    double messageToleranceBuffer = 0;
    double disposablePCP();

    double lastChangeTime = 0;
    double periodicalPCP();

    double cumulativeDistance = 0;
    Coord lastPos = Coord(0, 0, 0);
    double distanceBasedPCP();

    double randomPCP();

    void setMbType(mbTypes::Mbs mbType);
    void setMdAuthority(MDAuthority* mdAuthority);

};

#endif
