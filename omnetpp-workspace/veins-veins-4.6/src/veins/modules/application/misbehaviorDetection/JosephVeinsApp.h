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
#include <veins/modules/application/misbehaviorDetection/mdChecks/LegacyChecks.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;
using namespace Veins;

#include "veins/modules/messages/BasicSafetyMessage_m.h"

#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"
#include "veins/modules/mobility/traci/TraCIColor.h"

#include "supportClasses/GeneralLib.h"
#include <veins/modules/application/misbehaviorDetection/mdStats/MDStatistics.h>

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
#include "enumTypes/MdAppTypes.h"

#include "supportClasses/BsmPrintable.h"

#include "pcPolicies/PCPolicy.h"
#include "mdAttacks/MDAttack.h"

#include "supportClasses/HTTPRequest.h"

#define mlPortV1 9997
#define mlPortV2 9998

static unsigned long targetNodes[MAXTARGETLENGTH];
static int targetNodesLength = 0;
static double targetClearTime = 0;
static unsigned long accusedNodes[MAXACCUSEDLENGTH];
static int accusedNodesLength = 0;
static double accusedClearTime = 0;

static bool linkInit = false;
static LinkControl linkControl = LinkControl();

class JosephVeinsApp: public BaseWaveApplLayer {
private:
    GeneralLib genLib = GeneralLib();
public:
    NodeTable detectedNodes;

public:
    virtual void initialize(int stage);
    virtual void finish();
protected:

    MDApplication *AppV1;
    MDApplication *AppV2;

    virtual void onBSM(BasicSafetyMessage* bsm);
    virtual void onWSM(WaveShortMessage* wsm);
    virtual void onWSA(WaveServiceAdvertisment* wsa);

    virtual void handleSelfMsg(cMessage* msg);
    virtual void handlePositionUpdate(cObject* obj);

    mbTypes::Mbs induceMisbehavior(double attackers);
    void LocalMisbehaviorDetection(BasicSafetyMessage* bsm, int version);

    void writeReport(MDReport reportBase, std::string version, BsmCheck bsmCheck,
            BasicSafetyMessage *bsm);

    void sendReport(MDReport reportBase, std::string version,
            BsmCheck bsmCheck, BasicSafetyMessage *bsm);

    void writeMdBsm(std::string version, BsmCheck bsmCheck,
            BasicSafetyMessage *bsm);

    void writeSelfBsm(BasicSafetyMessage bsm);

    void treatAttackFlags();
    MDAttack mdAttack;

    pseudoChangeTypes::PseudoChange myPcType;
    PCPolicy pcPolicy;

    typedef std::list<Obstacle*> ObstacleGridCell;
    typedef std::vector<ObstacleGridCell> ObstacleGridRow;
    typedef std::vector<ObstacleGridRow> Obstacles;

    ThresholdApp ThreV1 = ThresholdApp(1, 0.5);
    ThresholdApp ThreV2 = ThresholdApp(2, 0.5);

    AggrigationApp AggrV1 = AggrigationApp(1,0.5, 10.0, 10);
    AggrigationApp AggrV2 = AggrigationApp(2,0.5, 10.0, 10);

    BehavioralApp BehaV1 = BehavioralApp(1, 0.5);
    BehavioralApp BehaV2 = BehavioralApp(2, 0.5);

    ExperiApp ExperV1 = ExperiApp(1, 10.0, 10, 3);
    ExperiApp ExperV2 = ExperiApp(2, 10.0, 10, 3);

    PyBridgeApp PybgV1 = PyBridgeApp(1, mlPortV1, "localhost");
    PyBridgeApp PybgV2 = PyBridgeApp(2, mlPortV2, "localhost");

public:

    void setMDApp(mdAppTypes::App, mdAppTypes::App);

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
