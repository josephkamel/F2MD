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

#ifndef __VEINS_MDAttack_H_
#define __VEINS_MDAttack_H_

#include <tuple>
#include <omnetpp.h>

#include "../baseClasses/NodeTable.h"
#include "../pcPolicies/PCPolicy.h"

class MDAttack {
protected:
    GeneralLib genLib = GeneralLib();

    BasicSafetyMessage* myBsm;
    int* myBsmNum;

    NodeTable* detectedNodes;

    unsigned long* myPseudonym;

    Coord* curPosition;
    Coord* curPositionConfidence;
    Coord* curSpeed;
    Coord* curSpeedConfidence;
    Coord* curHeading;
    Coord* curHeadingConfidence;

    double* myWidth;
    double* myLength;

    simtime_t* beaconInterval;

    PCPolicy* pcPolicy;

    BasicSafetyMessage nextAttackBsm = BasicSafetyMessage();

    BasicSafetyMessage StopBsm;
    bool StopInitiated;

    bool DoSInitiated;

    unsigned long SybilMyOldPseudo = 0;
    unsigned long SybilPseudonyms[MAX_SYBIL_NUM];
    int SybilVehSeq = 0;

    unsigned long targetNode = 0;

public:

    MDAttack();

    void init(attackTypes::Attacks myAttackType);

    BasicSafetyMessage launchAttack(attackTypes::Attacks myAttackType);

    void setBeaconInterval(simtime_t* beaconInterval);
    void setCurHeading(Coord* curHeading);
    void setCurHeadingConfidence(Coord* curHeadingConfidence);
    void setCurPosition(Coord* curPosition);
    void setCurPositionConfidence(Coord* curPositionConfidence);
    void setCurSpeed(Coord* curSpeed) ;
    void setCurSpeedConfidence(Coord* curSpeedConfidence);
    void setDetectedNodes(NodeTable* detectedNodes);
    void setMyBsm(BasicSafetyMessage* myBsm);
    void setMyBsmNum(int* myBsmNum);
    void setMyLength(double* myLength);
    void setMyPseudonym(unsigned long * myPseudonym);
    void setMyWidth(double* myWidth);
    void setPcPolicy(PCPolicy* pcPolicy);

    unsigned long getTargetNode();
};

#endif
