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

#define serialNumber "IRT-DEMO"
#define savePath "../../../../../mdmSave/"

#define confPos 5
#define confSpd 0
#define confHea 0

#define SAVE_PERIOD 1 //60 seconds

#define START_SAVE 0 //60 seconds
#define START_ATTACK 10 //60 seconds

// attack 1 OneMessageReport, 2 EvidenceReport, 3 BasicCheckReport
#define REPORT_VERSION "EvidenceReport"

#define ATTACKER_PROB 0.1
#define ATTACK_TYPE "Disruptive"
// attacks
// ConstPos ,ConstPosOffset, RandomPos, RandomPosOffset
// ConstSpeed, ConstSpeedOffet, RandomSpeed, RansomSpeedOffet
// EventualStop, Disruptive, DataReplay, StaleMessages
// DoS

#define ConstPosX 1200
#define ConstPosY 1200

#define ConstPosOffsetX 50
#define ConstPosOffsetY 50

#define RandomPosX 3900
#define RandomPosY 1700

#define RandomPosOffsetX 50
#define RandomPosOffsetY 50

#define StopProb 0.025

#define StaleMessages_Buffer 5 //less than 10

static bool EnablePC = false;
#define PC_TYPE "DistanceBased"
// PC_TYPE 1 Periodical, 2 Disposable, 3 DistanceBased, 3 Random
#define Tolerance_Buffer 10 // nbr of messages
#define Period_Change_Time 10 //seconds
#define Period_Change_Distance 1000 //meters
#define Random_Change_Chance 0.01 // 0.01 over 1 = 1% chance

//Detection Application
static bool EnableV1 = true;
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

        //pseudonym-------------------------------
        pseudoNum = 0;
        myPseudonym = getNextPseudonym();

        messageToleranceBuffer = 0;
        lastChangeTime = simTime().dbl();
        cumulativeDistance = 0;
        lastPos = Coord(0, 0, 0);
        //pseudonym-------------------------------

        mdAuthority.addNewNode(myPseudonym, myMdType, simTime().dbl());

        std::string stringId = std::to_string(myPseudonym);

        curPositionConfidence = Coord(confPos, confPos, 0);
        curSpeedConfidence = Coord(confSpd, confSpd, 0);
        curHeadingConfidence = Coord(confHea, confHea, 0);

        myReportType = REPORT_VERSION;

        if (!myMdType.compare("genuine")) {
            TraCIColor color = TraCIColor(0, 255, 0, 0);
            traciVehicle->setColor(color);
            myAttackType = "genuine";
        } else if (!myMdType.compare("attacker")) {

            //attack-------------------------------
            myAttackType = ATTACK_TYPE;
            StopInitiated = false;
            //attack-------------------------------


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

void JosephVeinsApp::checkPseudonymChange() {
    std::string pcType = PC_TYPE;
    if (!pcType.compare("Periodical")) {
        periodicalPCP();
    } else if (!pcType.compare("Disposable")) {
        disposablePCP();
    } else if (!pcType.compare("DistanceBased")) {
        distanceBasedPCP();
    } else if (!pcType.compare("Random")) {
        distanceBasedPCP();
    }
}

double JosephVeinsApp::periodicalPCP() {
    if ((simTime().dbl() - lastChangeTime) > Period_Change_Time) {
        lastChangeTime = simTime().dbl();
        myPseudonym = getNextPseudonym();
    }
}

double JosephVeinsApp::disposablePCP() {
    if (messageToleranceBuffer > Tolerance_Buffer) {
        messageToleranceBuffer = 0;
        myPseudonym = getNextPseudonym();
    } else {
        messageToleranceBuffer++;
    }
}

double JosephVeinsApp::distanceBasedPCP() {
    MDMLib mdmLib = MDMLib();
    double stepDistance = mdmLib.calculateDistance(lastPos, curPosition);
    lastPos = curPosition;

    cumulativeDistance = cumulativeDistance + stepDistance;
    if (cumulativeDistance > Period_Change_Distance) {
        myPseudonym = getNextPseudonym();
        cumulativeDistance = 0;
    }
}

double JosephVeinsApp::randomPCP() {
    double rnd = genLib.RandomDouble(0, 1);
    if (rnd < Random_Change_Chance) {
        myPseudonym = getNextPseudonym();
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

BsmCheck bsmCheckV1;
BsmCheck bsmCheckV2;
void JosephVeinsApp::onBSM(BasicSafetyMessage* bsm) {

    int senderID = bsm->getSenderPseudonym();

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

    if(EnablePC){
        checkPseudonymChange();
    }

//Your application has received a beacon message from another car or RSU
//code for handling the message goes here

}
void JosephVeinsApp::treatAttackFlags() {
    if (!myMdType.compare("attacker")) {

        if (!myAttackType.compare("StaleMessages")) {
            if(myBsmNum >= StaleMessages_Buffer){
                attackBsm = myBsm[StaleMessages_Buffer];
            }else{
                if(myBsmNum>0){
                    attackBsm = myBsm[0];
                }else{
                    attackBsm.setSenderPos(Coord(curPosition.x,curPosition.y,curPosition.z));
                    attackBsm.setSenderPosConfidence(curPositionConfidence);

                    attackBsm.setSenderSpeed(curSpeed);
                    attackBsm.setSenderSpeedConfidence(curSpeedConfidence);

                    attackBsm.setSenderHeading(curHeading);
                    attackBsm.setSenderHeadingConfidence(curHeadingConfidence);

                    attackBsm.setSenderWidth(myWidth);
                    attackBsm.setSenderLength(myLength);
                }
            }
        }

        if (!myAttackType.compare("ConstPos")) {
                attackBsm = myBsm[0];

                attackBsm.setSenderPos(Coord(ConstPosX,ConstPosY,curPosition.z));
                attackBsm.setSenderPosConfidence(curPositionConfidence);

                attackBsm.setSenderSpeed(curSpeed);
                attackBsm.setSenderSpeedConfidence(curSpeedConfidence);

                attackBsm.setSenderHeading(curHeading);
                attackBsm.setSenderHeadingConfidence(curHeadingConfidence);

                attackBsm.setSenderWidth(myWidth);
                attackBsm.setSenderLength(myLength);
        }

        if (!myAttackType.compare("ConstPosOffset")) {
                attackBsm = myBsm[0];

                attackBsm.setSenderPos(Coord(curPosition.x+ConstPosOffsetX,curPosition.y+ConstPosOffsetY,curPosition.z));
                attackBsm.setSenderPosConfidence(curPositionConfidence);

                attackBsm.setSenderSpeed(curSpeed);
                attackBsm.setSenderSpeedConfidence(curSpeedConfidence);

                attackBsm.setSenderHeading(curHeading);
                attackBsm.setSenderHeadingConfidence(curHeadingConfidence);

                attackBsm.setSenderWidth(myWidth);
                attackBsm.setSenderLength(myLength);
        }

        if (!myAttackType.compare("RandomPos")) {
                attackBsm = myBsm[0];

                double x = genLib.RandomInt(0, RandomPosX);
                double y = genLib.RandomInt(0, RandomPosY);

                attackBsm.setSenderPos(Coord(x,y,curPosition.z));
                attackBsm.setSenderPosConfidence(curPositionConfidence);

                attackBsm.setSenderSpeed(curSpeed);
                attackBsm.setSenderSpeedConfidence(curSpeedConfidence);

                attackBsm.setSenderHeading(curHeading);
                attackBsm.setSenderHeadingConfidence(curHeadingConfidence);

                attackBsm.setSenderWidth(myWidth);
                attackBsm.setSenderLength(myLength);
        }

        if (!myAttackType.compare("RandomPosOffset")) {
                attackBsm = myBsm[0];

                double x = genLib.RandomInt(0, RandomPosOffsetX);
                double y = genLib.RandomInt(0, RandomPosOffsetY);

                attackBsm.setSenderPos(Coord(curPosition.x+x,curPosition.y+y,curPosition.z));
                attackBsm.setSenderPosConfidence(curPositionConfidence);

                attackBsm.setSenderSpeed(curSpeed);
                attackBsm.setSenderSpeedConfidence(curSpeedConfidence);

                attackBsm.setSenderHeading(curHeading);
                attackBsm.setSenderHeadingConfidence(curHeadingConfidence);

                attackBsm.setSenderWidth(myWidth);
                attackBsm.setSenderLength(myLength);
        }

        if (!myAttackType.compare("EventualStop")) {
            if(StopInitiated){
                attackBsm = StopBsm;
            }else{
                double prob = genLib.RandomDouble(0, 1);
                if(prob<StopProb){
                    StopBsm = myBsm[0];
                    StopBsm.setSenderPos(curPosition);
                    StopBsm.setSenderPosConfidence(curPositionConfidence);

                    StopBsm.setSenderSpeed(Coord(0,0,0));
                    StopBsm.setSenderSpeedConfidence(curSpeedConfidence);

                    StopBsm.setSenderHeading(curHeading);
                    StopBsm.setSenderHeadingConfidence(curHeadingConfidence);

                    StopBsm.setSenderWidth(myWidth);
                    StopBsm.setSenderLength(myLength);
                    StopInitiated = true;
                }
                attackBsm = myBsm[0];
                attackBsm.setSenderPos(curPosition);
                attackBsm.setSenderPosConfidence(curPositionConfidence);

                attackBsm.setSenderSpeed(curSpeed);
                attackBsm.setSenderSpeedConfidence(curSpeedConfidence);

                attackBsm.setSenderHeading(curHeading);
                attackBsm.setSenderHeadingConfidence(curHeadingConfidence);

                attackBsm.setSenderWidth(myWidth);
                attackBsm.setSenderLength(myLength);
            }
        }

        if (!myAttackType.compare("Disruptive")) {
            if (detectedNodes.getNodesNum() > 0) {
                attackBsm = nextAttackBsm;
                nextAttackBsm = detectedNodes.getRandomBSM();
                addTargetNode(nextAttackBsm.getSenderPseudonym());
            }
        }

        if (!myAttackType.compare("DataReplay")) {
            if (detectedNodes.getNodesNum() > 0) {
                attackBsm = nextAttackBsm;
                nextAttackBsm = detectedNodes.getNextAttackedBsm(curPosition,
                        nextAttackBsm.getSenderPseudonym(),
                        nextAttackBsm.getArrivalTime().dbl());
                addTargetNode(nextAttackBsm.getSenderPseudonym());
            }
        }

    } else {
        if (isTargetNode(myPseudonym)) {
            TraCIColor color = TraCIColor(255, 255, 0, 0);
            traciVehicle->setColor(color);
        } else {
            TraCIColor color = TraCIColor(0, 255, 0, 0);
            traciVehicle->setColor(color);
        }
        if (isAccusedNode(myPseudonym)) {
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
    int senderID = bsm->getSenderPseudonym();

    switch (version) {
    case 1: {
        LegacyChecks mdm(myPseudonym, curPosition, curSpeed,
                Coord(myWidth, myLength), curHeading);
        bsmCheckV1 = mdm.CheckBSM(*bsm, detectedNodes);
        bool result = AppV1.CheckNodeForReport(myPseudonym, *bsm, bsmCheckV1,
                detectedNodes);
        if (result) {
            MDReport reportBase;
            reportBase.setGenerationTime(simTime().dbl());
            reportBase.setSenderId(myPseudonym);
            reportBase.setReportedId(senderID);
            reportBase.setMbType(bsm->getSenderMbType());
            reportBase.setAttackType(bsm->getSenderAttackType());
            std::pair<double, double> currLonLat = traci->getLonLat(
                    curPosition);
            reportBase.setSenderGps(Coord(currLonLat.first, currLonLat.second));
            reportBase.setReportedGps(bsm->getSenderGpsCoordinates());

            char nameV1[32] = "mdaV1";
            mdAuthority.sendReport(nameV1, reportBase);

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
                evr.addEvidence(myBsm[0], bsmCheckV1, *bsm, detectedNodes);
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
            varThrePrintableV1.registerMessage(bsm->getSenderMbType(),
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
        CaTChChecks mdmV2(myPseudonym, curPosition, curPositionConfidence,
                curHeading, curHeadingConfidence, Coord(myWidth, myLength));
        BsmCheck bsmCheckV2 = mdmV2.CheckBSM(*bsm, detectedNodes);
        bool result = AppV2.CheckNodeForReport(myPseudonym, *bsm, bsmCheckV2,
                detectedNodes);
        if (result) {

            MDReport reportBase;
            reportBase.setGenerationTime(simTime().dbl());
            reportBase.setSenderId(myPseudonym);
            reportBase.setReportedId(senderID);
            reportBase.setMbType(bsm->getSenderMbType());
            reportBase.setAttackType(bsm->getSenderAttackType());
            std::pair<double, double> currLonLat = traci->getLonLat(
                    curPosition);
            reportBase.setSenderGps(Coord(currLonLat.first, currLonLat.second));
            reportBase.setReportedGps(bsm->getSenderGpsCoordinates());

            char nameV2[32] = "mdaV2";
            mdAuthority.sendReport(nameV2, reportBase);

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
                evr.addEvidence(myBsm[0], bsmCheckV2, *bsm, detectedNodes);
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
            varThrePrintableV2.registerMessage(bsm->getSenderMbType(),
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

//    MDModuleV2 mdmV2(myPseudonym, curPosition, curPositionConfidence);
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

