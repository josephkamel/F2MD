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

#include "PCPolicy.h"


PCPolicy::PCPolicy() {
    messageToleranceBuffer = 0;
    lastChangeTime = simTime().dbl();
    cumulativeDistance = 0;
    lastPos = Coord(0, 0, 0);
}

void PCPolicy::setMbType(mbTypes::Mbs mbType) {
    this->mbType = mbType;
}

void PCPolicy::setMdAuthority(MDStatistics* mdAuthority) {
    this->mdAuthority = mdAuthority;
}

void PCPolicy::setCurPosition(Coord* curPosition) {
    this->curPosition = curPosition;
}

void PCPolicy::setMyId(int* myId) {
    this->myId = myId;
}

void PCPolicy::setMyPseudonym(unsigned long* myPseudonym) {
    this->myPseudonym = myPseudonym;
}

void PCPolicy::setPseudoNum(int* pseudoNum) {
    this->pseudoNum = pseudoNum;
}

unsigned long PCPolicy::getNextPseudonym() {
    (*pseudoNum)++;
    double simTimeDbl = simTime().dbl();
    while (simTimeDbl > 9) {
        simTimeDbl = simTimeDbl / 10;
    }
    simTimeDbl = (int) simTimeDbl;
    unsigned long pseudo = (*myId) * 10 + simTimeDbl;
    unsigned long digitNumber = (unsigned long) (log10(pseudo) + 1);
    unsigned long pseudoNumAdd = (*pseudoNum) * pow(10, digitNumber + 1);
    pseudo = pseudo + pseudoNumAdd;

    mdAuthority->addNewNode(pseudo, mbType, simTime().dbl());

    return pseudo;
}

void PCPolicy::checkPseudonymChange(pseudoChangeTypes::PseudoChange myPcType) {
    switch (myPcType) {
    case pseudoChangeTypes::Periodical:
        periodicalPCP();
        break;
    case pseudoChangeTypes::Disposable:
        disposablePCP();
        break;
    case pseudoChangeTypes::DistanceBased:
        distanceBasedPCP();
        break;
    case pseudoChangeTypes::Random:
        randomPCP();
        break;
    default:
        break;
    }
}

double PCPolicy::periodicalPCP() {
    if ((simTime().dbl() - lastChangeTime) > Period_Change_Time) {
        lastChangeTime = simTime().dbl();
        (*myPseudonym) = getNextPseudonym();
    }
}

double PCPolicy::disposablePCP() {
    if (messageToleranceBuffer > Tolerance_Buffer) {
        messageToleranceBuffer = 0;
        (*myPseudonym) = getNextPseudonym();
    } else {
        messageToleranceBuffer++;
    }
}

double PCPolicy::distanceBasedPCP() {
    MDMLib mdmLib = MDMLib();
    double stepDistance = mdmLib.calculateDistance(lastPos, (*curPosition));
    lastPos = (*curPosition);

    cumulativeDistance = cumulativeDistance + stepDistance;
    if (cumulativeDistance > Period_Change_Distance) {
        (*myPseudonym) = getNextPseudonym();
        cumulativeDistance = 0;
    }
}

double PCPolicy::randomPCP() {
    double rnd = genLib.RandomDouble(0, 1);
    if (rnd < Random_Change_Chance) {
        (*myPseudonym) = getNextPseudonym();
    }
}
