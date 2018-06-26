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
#include "mdChecks/MDModule.h"
#include "mdChecks/MDModuleV2.h"
#include "mdChecks/MDModuleV3.h"
#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;
using namespace Veins;

#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"
#include "veins/modules/mobility/traci/TraCIColor.h"

#include "supportClasses/GeneralLib.h"
#include "mdAuthority/MDAuthority.h"

#include "mdApplications/MDApplication.h"
#include "mdApplications/ThresholdApp.h"
#include "mdApplications/AggrigationApp.h"
#include "mdApplications/BehavioralApp.h"

#include "supportClasses/VarThrePrintable.h"
#include "supportClasses/XmlWriter.h"

#include <veins/modules/application/misbehaviorDetection/mdReport/OneMessageReport.h>
#include <veins/modules/application/misbehaviorDetection/mdReport/EvidenceReport.h>

static int targetNodes[MAXTARGETLENGTH];
static int targetNodesLength = 0;
static double targetClearTime = 0;
static int accusedNodes[MAXACCUSEDLENGTH];
static int accusedNodesLength = 0;
static double accusedClearTime = 0;

class JosephVeinsApp: public BaseWaveApplLayer {
private:
    NodeTable detectedNodes;
    GeneralLib genLib = GeneralLib();

public:
    virtual void initialize(int stage);
    virtual void finish();
protected:

    virtual void onBSM(BasicSafetyMessage* bsm);
    virtual void onBSMold(BasicSafetyMessage* bsm);
    virtual void onWSM(WaveShortMessage* wsm);
    virtual void onWSA(WaveServiceAdvertisment* wsa);

    virtual void handleSelfMsg(cMessage* msg);
    virtual void handlePositionUpdate(cObject* obj);

    double induceMisbehaviorProb(double faulty, double attackers);
    std::string induceMisbehavior(double attackers);
    void LocalMisbehaviorDetection(BasicSafetyMessage* bsm, int version);

    void treatAttackFlags();

    void addTargetNode(int id);
    void removeTargetNode(int id);
    void clearTargetNodes();
    bool isTargetNode(int id);

    void addAccusedNode(int id);
    void removeAccusedNode(int id);
    void clearAccusedNodes();
    bool isAccusedNode(int id);

    typedef std::list<Obstacle*> ObstacleGridCell;
    typedef std::vector<ObstacleGridCell> ObstacleGridRow;
    typedef std::vector<ObstacleGridRow> Obstacles;
};

#endif
