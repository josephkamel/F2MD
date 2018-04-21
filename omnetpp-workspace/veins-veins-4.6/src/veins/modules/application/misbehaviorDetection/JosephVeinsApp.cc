/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#include "JosephVeinsApp.h"

Define_Module(JosephVeinsApp);

#define serialNumber "IRT-DEMO"
#define savePath "/home/joseph/Projects/ConfidenceRange/mdmSave/"

#define confPos 7.5
#define confSpd 0
#define confHea 0

#define FAULTY 0.95
#define ATTACKERS 0.05

#define SAVE_PERIOD 1 //60 seconds

#define START_SAVE 0 //60 seconds
#define START_ATTACK 10 //60 seconds

#define DISRUPTIVE false

static int targetNodes [MAXTARGETLENGTH];
static int targetNodesLength = 0;
static double targetClearTime = 0;

void JosephVeinsApp::initialize(int stage) {

    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        //joseph
        //Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;

        Coord vehSize = genLib.TypeToSize(traciVehicle->getTypeId());
        myWidth = vehSize.x;
        myLength = vehSize.y;

        myMdType = induceMisbehaviorFixed(FAULTY, ATTACKERS);
        mdAuthority.addNewNode(myId, myMdType, simTime().dbl());

        std::string stringId = std::to_string(myId);

        if (myMdType == 1) {
            curPositionConfidence = Coord(confPos, confPos, 0);
            curSpeedConfidence = Coord(confSpd, confSpd, 0);
            curHeadingConfidence = Coord(confHea, confHea, 0);
            TraCIColor color = TraCIColor(0,255,0,0);
            traciVehicle->setColor(color);
        }else if(myMdType == 2){
            curPositionConfidence = Coord(confPos, confPos, 0);
            curSpeedConfidence = Coord(confSpd, confSpd, 0);
            curHeadingConfidence = Coord(confHea, confHea, 0);
            TraCIColor color = TraCIColor(255,0,0,0);
            traciVehicle->setColor(color);
        }else{
            TraCIColor color = TraCIColor(0,255,0,0);
            traciVehicle->setColor(color);
        }

    } else if (stage == 1) {

        //Initializing members that require initialized other modules goes here
    }
}

void JosephVeinsApp::finish() {
    BaseWaveApplLayer::finish();
    //statistics recording goes here
}


static double totalFaulty = 0;
static double totalAttacker = 0;
double JosephVeinsApp::induceMisbehaviorFixed(double faulty, double attackers) {

    if(simTime().dbl() < START_ATTACK){
        return 1;
    }

    if((totalAttacker + totalFaulty) == 0){
        totalFaulty++;
        return 1;
    }

    double realFactor = totalAttacker / (totalFaulty+totalAttacker);
    double theoFactor = attackers / (faulty+attackers);

    if(theoFactor > realFactor){
        totalAttacker++;
        return 2;
    }else{
        totalFaulty++;
        return 1;
    }

}

double JosephVeinsApp::induceMisbehaviorProb(double faulty, double attackers) {
    double randVal = 0;
    double randInt = genLib.RandomInt(0, 1);

    if ((faulty + attackers) < 1) {
        randVal = genLib.RandomDouble(0, 1);
    } else {
        randVal = genLib.RandomDouble(0, faulty + attackers);
    }

    if (randInt == 0) {
        if (randVal < attackers) {
            return 2;
        }

        if (randVal >= attackers && randVal < (attackers + faulty)) {
            return 1;
        }
    } else {
        if (randVal < faulty) {
            return 1;
        }

        if (randVal >= faulty && randVal < (attackers + faulty)) {
            return 2;
        }
    }

    return 0;
}

static double deltaT = 0;
static bool init = false;
void JosephVeinsApp::onBSM(BasicSafetyMessage* bsm) {
    int senderID = bsm->getSenderAddress();
    if (bsm->getSenderMbType() == 0) {
        BsmCheck bsmCheck;
        if (!detectedNodes.includes(senderID)) {
            NodeHistory newNode(senderID);
            newNode.addBSM(*bsm);
            MDMHistory newMDM(senderID);
            newMDM.addBsmCheck(bsmCheck);
            detectedNodes.put(senderID, newNode, newMDM);
        } else {
            NodeHistory existingNode = detectedNodes.getNodeHistory(senderID);
            existingNode.addBSM(*bsm);
            MDMHistory existingMDM = detectedNodes.getMDMHistory(senderID);
            existingMDM.addBsmCheck(bsmCheck);
            detectedNodes.put(senderID, existingNode, existingMDM);
        }
    } else {
        MDModule mdm(myId, curPosition, curSpeed, Coord(myWidth, myLength),
                curHeading);
        MDModuleV2 mdmV2(myId, curPosition, curPositionConfidence);

        BsmCheck bsmCheck = mdmV2.CheckBSM(*bsm, detectedNodes);

        if (!detectedNodes.includes(senderID)) {
            NodeHistory newNode(senderID);
            newNode.addBSM(*bsm);
            MDMHistory newMDM(senderID);
            newMDM.addBsmCheck(bsmCheck);
            detectedNodes.put(senderID, newNode, newMDM);
        } else {
            NodeHistory existingNode = detectedNodes.getNodeHistory(senderID);
            existingNode.addBSM(*bsm);
            MDMHistory existingMDM = detectedNodes.getMDMHistory(senderID);
            existingMDM.addBsmCheck(bsmCheck);
            detectedNodes.put(senderID, existingNode, existingMDM);
        }

        mdm.reportMB(mdm.CheckBSM(detectedNodes, senderID), senderID,
                bsm->getSenderMbType());


            if(!init){
                mdm.resetAllFlags();
                mdmV2.resetAllFlags();
                //mdAuthority.resetAll();
                init = true;
            }

            if ((simTime().dbl() - deltaT) > SAVE_PERIOD) {
                deltaT = simTime().dbl();

                if(simTime().dbl() > START_SAVE){
                    mdm.saveLine(savePath, serialNumber,
                            mobility->getManager()->getManagedHosts().size(), deltaT);
                    mdmV2.saveLine(savePath, serialNumber,
                            mobility->getManager()->getManagedHosts().size(), deltaT);
                    mdAuthority.saveLine(savePath, serialNumber, deltaT);
                }
                mdm.resetTempFlags();
                mdmV2.resetTempFlags();
            }

        if (myMdType == 2) {
            if (detectedNodes.getNodesNum() > 0) {
                attackBsm = nextAttackBsm;
                if(DISRUPTIVE){
                    nextAttackBsm = detectedNodes.getRandomBSM();
                    addTargetNode(nextAttackBsm.getSenderAddress());
                }else{
                    nextAttackBsm = detectedNodes.getNextAttackedBsm(curPosition, nextAttackBsm.getSenderAddress(), nextAttackBsm.getArrivalTime().dbl());
                    addTargetNode(nextAttackBsm.getSenderAddress());
                }
            }
        }else{
            if(isTargetNode(myId)){
                TraCIColor color = TraCIColor(255,255,0,0);
                traciVehicle->setColor(color);
            }else{
                TraCIColor color = TraCIColor(0,255,0,0);
                traciVehicle->setColor(color);
            }
        }

        if ((simTime().dbl()-targetClearTime) > MAXTARGETTIME) {
            targetClearTime = simTime().dbl();
            clearTargetNodes();
        }


    }

//Your application has received a beacon message from another car or RSU
//code for handling the message goes here

}

void JosephVeinsApp::onWSM(WaveShortMessage* wsm) {
//Your application has received a data message from another car or RSU
//code for handling the message goes here, see TraciDemo11p.cc for examples

}

void JosephVeinsApp::onWSA(WaveServiceAdvertisment* wsa) {
//Your application has received a service advertisement from another car or RSU
//code for handling the message goes here, see TraciDemo11p.cc for examples

}

void JosephVeinsApp::handleSelfMsg(cMessage* msg) {
    BaseWaveApplLayer::handleSelfMsg(msg);

//this method is for self messages (mostly timers)
//it is important to call the BaseWaveApplLayer function for BSM and WSM transmission

}

void JosephVeinsApp::handlePositionUpdate(cObject* obj) {
    BaseWaveApplLayer::handlePositionUpdate(obj);

//    MDModuleV2 mdmV2(myId, curPosition, curPositionConfidence);
//    mdmV2.CheckNodesHistoryForReport(&detectedNodes);

//the vehicle has moved. Code that reacts to new positions goes here.
//member variables such as currentPosition and currentSpeed are updated in the parent class
}


void JosephVeinsApp::addTargetNode(int id){
    bool found = false;
    for (int var = 0; var < targetNodesLength; ++var) {
        if(targetNodes[var] == id){
            found = true;
        }
    }

    if(!found){
        targetNodes[targetNodesLength] = id;
        targetNodesLength ++;
    }
}
void JosephVeinsApp::removeTargetNode(int id){
    int index = -1;
    for (int var = 0; var < targetNodesLength; ++var) {
        if(targetNodes[var] == id){
            index = var;
            break;
        }
    }

    for (int var = index; var < targetNodesLength-1; ++var) {
        targetNodes[var] = targetNodes[var+1];
    }
    targetNodesLength --;
}
void JosephVeinsApp::clearTargetNodes(){
    targetNodesLength = 0;
}
bool JosephVeinsApp::isTargetNode(int id){
    for (int var = 0; var < targetNodesLength; ++var) {
        if(targetNodes[var] == id){
            return true;
        }
    }
    return false;
}

