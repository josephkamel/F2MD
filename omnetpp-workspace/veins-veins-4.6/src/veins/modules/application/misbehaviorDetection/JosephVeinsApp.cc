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

#include "JosephVeinsApp.h"

Define_Module(JosephVeinsApp);

#define serialNumber "COLLECT-REPORTS-ConstAddition5050"
#define savePath "../../../../../mdmSave/"

#define confPos 5
#define confSpd 0
#define confHea 0

#define SAVE_PERIOD 1 //60 seconds

#define START_SAVE 0 //60 seconds
#define START_ATTACK 10 //60 seconds

// attack 1 OneMessageReport, 2 EvidenceReport
#define REPORT_VERSION "EvidenceReport"

#define ATTACKER_PROB 0.1
#define ATTACK_TYPE "ConstAddition"
// attack 1 ConstAddition, 2 Disruptive, 3 DataReplay
#define CONSTX 50
#define CONSTY 50

static bool EnableV1 = false;
static bool EnableV2 = true;

static MDAuthority mdAuthority = MDAuthority();
char const *AppV1Name = "AppV1";
char const *AppV2Name = "AppV2";

ThresholdApp AppV1(AppV1Name, 0.5);
ThresholdApp AppV2(AppV2Name, 0.5);

//AggrigationApp AppV1(AppV1Name,1,10.0,5);
//AggrigationApp AppV2(AppV2Name,2,10.0,5);

//BehavioralApp AppV1(AppV1Name,1,10.0,10,3);
//BehavioralApp AppV2(AppV2Name,2,10.0,10,3);

static bool PrintTPFP = false;
static VarThrePrintable varThrePrintableV1(AppV1Name);
static VarThrePrintable varThrePrintableV2(AppV2Name);
#include <stdlib.h>
#include <stdio.h>
#include <linux/limits.h>
void JosephVeinsApp::initialize(int stage) {

    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {

        //joseph
        //Initializing members and pointers of your application goes here
        EV << "Initializing " << par("appName").stringValue() << std::endl;

        Coord vehSize = genLib.TypeToSize(traciVehicle->getTypeId());
        myWidth = vehSize.x;
        myLength = vehSize.y;

        myMdType = induceMisbehavior(ATTACKER_PROB);

        mdAuthority.addNewNode(myId, myMdType, simTime().dbl());

        std::string stringId = std::to_string(myId);

        curPositionConfidence = Coord(confPos, confPos, 0);
        curSpeedConfidence = Coord(confSpd, confSpd, 0);
        curHeadingConfidence = Coord(confHea, confHea, 0);

        myReportType = REPORT_VERSION;

        if (!myMdType.compare("genuine")) {
            TraCIColor color = TraCIColor(0, 255, 0, 0);
            traciVehicle->setColor(color);
            myAttackType = "genuine";
        } else if (!myMdType.compare("attacker")) {

            myAttackType = ATTACK_TYPE;
            ConstX = CONSTX;
            ConstY = CONSTY;

            TraCIColor color = TraCIColor(255, 0, 0, 0);
            traciVehicle->setColor(color);
        } else {
            TraCIColor color = TraCIColor(0, 0, 0, 0);
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

static double totalGenuine = 0;
static double totalAttacker = 0;
std::string JosephVeinsApp::induceMisbehavior(double attacker) {

    double genuine = 1 - attacker;

    if (simTime().dbl() < START_ATTACK) {
        return "genuine";
    }

    if ((totalAttacker + totalGenuine) == 0) {
        totalGenuine++;
        return "genuine";
    }

    double realFactor = totalAttacker / (totalGenuine + totalAttacker);
    double theoFactor = attacker / (genuine + attacker);

    if (theoFactor > realFactor) {
        totalAttacker++;
        return "attacker";
    } else {
        totalGenuine++;
        return "genuine";
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


BsmCheck bsmCheckV1;
BsmCheck bsmCheckV2;
void JosephVeinsApp::onBSM(BasicSafetyMessage* bsm) {

    int senderID = bsm->getSenderAddress();

    if (EnableV1) {
        LocalMisbehaviorDetection(bsm, 1);
    }
    if (EnableV2) {
        LocalMisbehaviorDetection(bsm, 2);
    }

    if (!detectedNodes.includes(senderID)) {
        NodeHistory newNode(senderID);
        newNode.addBSM(*bsm);
        MDMHistory newMDM(senderID);
        if (EnableV1) {
            newMDM.addBsmCheck(bsmCheckV1, 1);
        }
        if (EnableV2) {
            newMDM.addBsmCheck(bsmCheckV2, 2);
        }
        detectedNodes.put(senderID, newNode, newMDM);
    } else {
        NodeHistory existingNode = detectedNodes.getNodeHistory(senderID);
        existingNode.addBSM(*bsm);
        MDMHistory existingMDM = detectedNodes.getMDMHistory(senderID);
        if (EnableV1) {
            existingMDM.addBsmCheck(bsmCheckV1, 1);
        }
        if (EnableV2) {
            existingMDM.addBsmCheck(bsmCheckV2, 2);
        }
        detectedNodes.put(senderID, existingNode, existingMDM);
    }

    treatAttackFlags();

//Your application has received a beacon message from another car or RSU
//code for handling the message goes here

}
void JosephVeinsApp::treatAttackFlags() {
    if (!myMdType.compare("attacker")) {
        if (!myAttackType.compare("Disruptive")
                || !myAttackType.compare("DataReplay")) {
            if (detectedNodes.getNodesNum() > 0) {
                attackBsm = nextAttackBsm;
                if (!myAttackType.compare("Disruptive")) {
                    nextAttackBsm = detectedNodes.getRandomBSM();
                    addTargetNode(nextAttackBsm.getSenderAddress());
                }
                if (!myAttackType.compare("DataReplay")) {
                    nextAttackBsm = detectedNodes.getNextAttackedBsm(
                            curPosition, nextAttackBsm.getSenderAddress(),
                            nextAttackBsm.getArrivalTime().dbl());
                    addTargetNode(nextAttackBsm.getSenderAddress());
                }
            }
        }

    } else {
        if (isTargetNode(myId)) {
            TraCIColor color = TraCIColor(255, 255, 0, 0);
            traciVehicle->setColor(color);
        } else {
            TraCIColor color = TraCIColor(0, 255, 0, 0);
            traciVehicle->setColor(color);
        }
        if (isAccusedNode(myId)) {
            TraCIColor color = TraCIColor(0, 0, 255, 0);
            traciVehicle->setColor(color);
        }
    }

    if ((simTime().dbl() - targetClearTime) > MAXTARGETTIME) {
        targetClearTime = simTime().dbl();
        clearTargetNodes();
    }

    if ((simTime().dbl() - accusedClearTime) > MAXACCUSEDTTIME) {
        accusedClearTime = simTime().dbl();
        clearAccusedNodes();
    }

}

static double deltaTV1 = 0;
static double deltaTV2 = 0;

static bool initV1 = false;
static bool initV2 = false;
void JosephVeinsApp::LocalMisbehaviorDetection(BasicSafetyMessage* bsm,
        int version) {
    int senderID = bsm->getSenderAddress();

    switch (version) {
    case 1: {
        LegacyChecks mdm(myId, curPosition, curSpeed, Coord(myWidth, myLength),
                curHeading);
        bsmCheckV1 = mdm.CheckBSM(*bsm, detectedNodes);
        bool result = AppV1.CheckNodeForReport(myId, *bsm,
                bsmCheckV1, detectedNodes);
        if (result) {
            MDReport reportBase;
                reportBase.setGenerationTime(simTime().dbl());
                reportBase.setSenderId(myId);
                reportBase.setReportedId(senderID);
                reportBase.setMbType(bsm->getSenderMbTypeStr());
                reportBase.setAttackType(bsm->getSenderAttackTypeStr());
                std::pair<double, double> currLonLat = traci->getLonLat(curPosition);
                reportBase.setSenderGps(Coord(currLonLat.first,currLonLat.second));
                reportBase.setReportedGps(bsm->getSenderGpsCoordinates());

            mdm.SendReport(&mdAuthority, reportBase);

            if (!myReportType.compare("BasicCheckReport")) {
                BasicCheckReport bcr = BasicCheckReport(reportBase);
                bcr.setReportedCheck(bsmCheckV1);
                bcr.writeStrToFile(savePath, serialNumber, "V1",
                        bcr.getReportPrintableJson());
//                std::cout << bcr.getReportPrintableJson();
            }

            if (!myReportType.compare("OneMessageReport")) {
                OneMessageReport omr = OneMessageReport(reportBase);
                omr.setReportedBsm(*bsm);
                omr.setReportedCheck(bsmCheckV1);
                omr.writeStrToFile(savePath, serialNumber, "V1",
                        omr.getReportPrintableJson());
//                std::cout << omr.getReportPrintableJson();
            }
            if (!myReportType.compare("EvidenceReport")) {
                EvidenceReport evr = EvidenceReport(reportBase);
                evr.addEvidence(myBsm, bsmCheckV1, *bsm, detectedNodes);
                evr.writeStrToFile(savePath, serialNumber, "V1",
                        evr.getReportPrintableJson());
             //   std::cout << evr.getReportPrintable();
            }

        }
        if (!initV1) {
            AppV1.resetAllFlags();
            //mdAuthority.resetAll();
            initV1 = true;
        }
        if (PrintTPFP) {
            double minFactor = AppV1.getMinFactor();
            varThrePrintableV1.registerMessage(bsm->getSenderMbTypeStr(),
                    minFactor);
        }

        if ((simTime().dbl() - deltaTV1) > SAVE_PERIOD) {
            deltaTV1 = simTime().dbl();
            if (PrintTPFP) {
                varThrePrintableV1.saveFile(savePath, serialNumber);
            }

            if (simTime().dbl() > START_SAVE) {
                AppV1.saveLine(savePath, serialNumber,
                        mobility->getManager()->getManagedHosts().size(),
                        deltaTV1);

                mdAuthority.saveLine(savePath, serialNumber, deltaTV1);
            }
            AppV1.resetInstFlags();
        }
        if (result) {
            addAccusedNode(senderID);
        }

        break;
    }
    case 2: {
        CaTChChecks mdmV2(myId, curPosition, curPositionConfidence, curHeading,
                curHeadingConfidence, Coord(myWidth, myLength));
        BsmCheck bsmCheckV2 = mdmV2.CheckBSM(*bsm, detectedNodes);
        bool result = AppV2.CheckNodeForReport(myId, *bsm,
                bsmCheckV2, detectedNodes);
        if (result) {

            MDReport reportBase;
                reportBase.setGenerationTime(simTime().dbl());
                reportBase.setSenderId(myId);
                reportBase.setReportedId(senderID);
                reportBase.setMbType(bsm->getSenderMbTypeStr());
                reportBase.setAttackType(bsm->getSenderAttackTypeStr());
                std::pair<double, double> currLonLat = traci->getLonLat(curPosition);
                reportBase.setSenderGps(Coord(currLonLat.first,currLonLat.second));
                reportBase.setReportedGps(bsm->getSenderGpsCoordinates());

            mdmV2.SendReport(&mdAuthority, reportBase);

            if (!myReportType.compare("BasicCheckReport")) {
                BasicCheckReport bcr = BasicCheckReport(reportBase);
                bcr.setReportedCheck(bsmCheckV2);
                bcr.writeStrToFile(savePath, serialNumber, "V2",
                        bcr.getReportPrintableJson());
            //    std::cout << bcr.getReportPrintableJson();
            }

            if (!myReportType.compare("OneMessageReport")) {

                OneMessageReport omr = OneMessageReport(reportBase);
                omr.setReportedBsm(*bsm);
                omr.setReportedCheck(bsmCheckV2);
                omr.writeStrToFile(savePath, serialNumber, "V2",
                        omr.getReportPrintableJson());
             //   std::cout << omr.getReportPrintableJson();

            }
            if (!myReportType.compare("EvidenceReport")) {
                EvidenceReport evr = EvidenceReport(reportBase);
                evr.addEvidence(myBsm, bsmCheckV2, *bsm, detectedNodes);
                evr.writeStrToFile(savePath, serialNumber, "V2",
                        evr.getReportPrintableJson());
           //    std::cout << evr.getReportPrintableJson();

            }

        }
        if (!initV2) {
            AppV2.resetAllFlags();
            //mdAuthority.resetAll();
            initV2 = true;
        }
        if (PrintTPFP) {
            double minFactor = AppV2.getMinFactor();
            varThrePrintableV2.registerMessage(bsm->getSenderMbTypeStr(),
                    minFactor);
        }

        if ((simTime().dbl() - deltaTV2) > SAVE_PERIOD) {
            deltaTV2 = simTime().dbl();
            if (PrintTPFP) {
                varThrePrintableV2.saveFile(savePath, serialNumber);
            }

            if (simTime().dbl() > START_SAVE) {
                AppV2.saveLine(savePath, serialNumber,
                        mobility->getManager()->getManagedHosts().size(),
                        deltaTV2);

                mdAuthority.saveLine(savePath, serialNumber, deltaTV2);
            }
            AppV2.resetInstFlags();
        }
        if (result) {
            addAccusedNode(senderID);
        }

        break;
    }

    default:
        break;
    }

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

void JosephVeinsApp::addTargetNode(int id) {
    bool found = false;
    for (int var = 0; var < targetNodesLength; ++var) {
        if (targetNodes[var] == id) {
            found = true;
        }
    }

    if (!found) {
        targetNodes[targetNodesLength] = id;
        targetNodesLength++;
    }
}
void JosephVeinsApp::removeTargetNode(int id) {
    int index = -1;
    for (int var = 0; var < targetNodesLength; ++var) {
        if (targetNodes[var] == id) {
            index = var;
            break;
        }
    }

    for (int var = index; var < targetNodesLength - 1; ++var) {
        targetNodes[var] = targetNodes[var + 1];
    }
    targetNodesLength--;
}
void JosephVeinsApp::clearTargetNodes() {
    targetNodesLength = 0;
}
bool JosephVeinsApp::isTargetNode(int id) {
    for (int var = 0; var < targetNodesLength; ++var) {
        if (targetNodes[var] == id) {
            return true;
        }
    }
    return false;
}

void JosephVeinsApp::addAccusedNode(int id) {
    bool found = false;
    for (int var = 0; var < accusedNodesLength; ++var) {
        if (accusedNodes[var] == id) {
            found = true;
        }
    }

    if (!found) {
        accusedNodes[accusedNodesLength] = id;
        accusedNodesLength++;
    }
}
void JosephVeinsApp::removeAccusedNode(int id) {
    int index = -1;
    for (int var = 0; var < accusedNodesLength; ++var) {
        if (accusedNodes[var] == id) {
            index = var;
            break;
        }
    }

    for (int var = index; var < accusedNodesLength - 1; ++var) {
        accusedNodes[var] = accusedNodes[var + 1];
    }
    accusedNodesLength--;
}
void JosephVeinsApp::clearAccusedNodes() {
    accusedNodesLength = 0;
}
bool JosephVeinsApp::isAccusedNode(int id) {
    for (int var = 0; var < accusedNodesLength; ++var) {
        if (accusedNodes[var] == id) {
            return true;
        }
    }
    return false;
}

