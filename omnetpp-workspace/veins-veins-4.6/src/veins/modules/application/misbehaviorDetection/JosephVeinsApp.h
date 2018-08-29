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

#ifndef __VEINS_JOSEPHVEINSAPP_H_
#define __VEINS_JOSEPHVEINSAPP_H_

#include <math.h>
#include <omnetpp.h>
#include <veins/modules/application/misbehaviorDetection/mdChecks/CaTChChecks.h>
#include <veins/modules/application/misbehaviorDetection/mdChecks/ExperiChecks.h>
#include <veins/modules/application/misbehaviorDetection/mdChecks/LegacyChecks.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;
using namespace Veins;

#include "veins/modules/messages/BasicSafetyMessage_m.h"

#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"
#include "veins/modules/mobility/traci/TraCIColor.h"

#include "supportClasses/GeneralLib.h"
#include "mdAuthority/MDAuthority.h"

#include "mdApplications/MDApplication.h"
#include "mdApplications/ThresholdApp.h"
#include "mdApplications/AggrigationApp.h"
#include "mdApplications/BehavioralApp.h"
#include "mdApplications/PyBridgeApp.h"
#include <veins/modules/application/misbehaviorDetection/mdApplications/ExperiApp.h>

#include "supportClasses/VarThrePrintable.h"
#include "supportClasses/XmlWriter.h"

#include <veins/modules/application/misbehaviorDetection/mdReport/OneMessageReport.h>
#include <veins/modules/application/misbehaviorDetection/mdReport/EvidenceReport.h>
#include <veins/modules/application/misbehaviorDetection/mdReport/BasicCheckReport.h>

#include <stdlib.h>
#include <stdio.h>
#include <linux/limits.h>
#include "enumTypes/AttackTypes.h"
#include "enumTypes/PseudoChangeTypes.h"
#include "enumTypes/ReportTypes.h"
#include "enumTypes/MbTypes.h"

#include "supportClasses/BsmPrintable.h"

#include "pcPolicies/PCPolicy.h"
#include "mdAttacks/MDAttack.h"

static unsigned long targetNodes[MAXTARGETLENGTH];
static int targetNodesLength = 0;
static double targetClearTime = 0;
static unsigned long accusedNodes[MAXACCUSEDLENGTH];
static int accusedNodesLength = 0;
static double accusedClearTime = 0;

class JosephVeinsApp: public BaseWaveApplLayer {
private:
    GeneralLib genLib = GeneralLib();
public:
    NodeTable detectedNodes;

public:
    virtual void initialize(int stage);
    virtual void finish();
protected:

    virtual void onBSM(BasicSafetyMessage* bsm);
    virtual void onWSM(WaveShortMessage* wsm);
    virtual void onWSA(WaveServiceAdvertisment* wsa);

    virtual void handleSelfMsg(cMessage* msg);
    virtual void handlePositionUpdate(cObject* obj);

    mbTypes::Mbs induceMisbehavior(double attackers);
    void LocalMisbehaviorDetection(BasicSafetyMessage* bsm, int version);

    void sendReport(MDReport reportBase, std::string version, BsmCheck bsmCheck,
            BasicSafetyMessage *bsm);

    void writeMdBsm(std::string version, BsmCheck bsmCheck,
            BasicSafetyMessage *bsm);

    void writeSelfBsm(
            BasicSafetyMessage bsm);

    void treatAttackFlags();
    MDAttack mdAttack;

    pseudoChangeTypes::PseudoChange myPcType;
    PCPolicy pcPolicy;



    typedef std::list<Obstacle*> ObstacleGridCell;
    typedef std::vector<ObstacleGridCell> ObstacleGridRow;
    typedef std::vector<ObstacleGridRow> Obstacles;

public:
    void addTargetNode(unsigned long id);
    void removeTargetNode(unsigned long id);
    void clearTargetNodes();
    bool isTargetNode(unsigned long id);

    void addAccusedNode(unsigned long id);
    void removeAccusedNode(unsigned long id);
    void clearAccusedNodes();
    bool isAccusedNode(unsigned long id);
};

#endif
