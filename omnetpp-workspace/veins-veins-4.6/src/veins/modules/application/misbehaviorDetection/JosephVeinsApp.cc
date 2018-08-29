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

static bool randomConf = false;
#define confPos 5
#define confSpd 0
#define confHea 0

#define SAVE_PERIOD 1 //60 seconds

#define START_SAVE 0 //60 seconds
#define START_ATTACK 10 //60 seconds

#define REPORT_VERSION reportTypes::EvidenceReport

static bool MixAttacks = false;
static attackTypes::Attacks MixAttacksList[] = {
        attackTypes::ConstPos, attackTypes::ConstPosOffset,
        attackTypes::RandomPos, attackTypes::RandomPosOffset,
        attackTypes::ConstSpeed, attackTypes::ConstSpeedOffset,
        attackTypes::RandomSpeed, attackTypes::RandomSpeedOffset,
        attackTypes::EventualStop, attackTypes::Disruptive,
        attackTypes::DataReplay, attackTypes::StaleMessages, attackTypes::DoS,
        attackTypes::DoSRandom, attackTypes::DoSDisruptive, attackTypes::Sybil };

#define ATTACKER_PROB 0.1
#define ATTACK_TYPE attackTypes::DataReplay
// 1 ConstPos, 2 ConstPosOffset, 3 RandomPos, 4 RandomPosOffset,
// 5 ConstSpeed, 6 ConstSpeedOffset, 7 RandomSpeed, 8 RandomSpeedOffset,
// 9 EventualStop, 10 Disruptive, 11 DataReplay, 12 StaleMessages,
// 13 DoS, 14 DoSRandom, 15 DoSDisruptive, 16 Sybil

static bool EnablePC = false;
#define PC_TYPE pseudoChangeTypes::Periodical
// Periodical, Disposable, DistanceBased, Random

//Detection Application
static bool EnableV1 = true;
static bool EnableV2 = true;

static bool writeSelfMsg = false;

//writeBsms
static bool writeBsmsV1 = false;
static bool writeBsmsV2 = false;
//writeReport
static bool writeReportsV1 = false;
static bool writeReportsV2 = false;

static MDAuthority mdAuthority = MDAuthority();
char const *AppV1Name = "AppV1";
char const *AppV2Name = "AppV2";

//ThresholdApp AppV1(AppV1Name, 0.5);
//ThresholdApp AppV2(AppV2Name, 0.5);

//AggrigationApp AppV1(AppV1Name,1,10.0,10);
//AggrigationApp AppV2(AppV2Name,2,10.0,10);

//BehavioralApp AppV1(AppV1Name,1,10.0,0.5);
//BehavioralApp AppV2(AppV2Name,2,10.0,0.8);

//ExperiApp AppV1(AppV1Name,1,10.0,10,3);
//ExperiApp AppV2(AppV2Name,2,10.0,10,3);

PyBridgeApp AppV1(AppV1Name,9998,"localhost");
PyBridgeApp AppV2(AppV2Name,9998,"localhost");

static bool PrintTPFP = false;
static VarThrePrintable varThrePrintableV1(AppV1Name);
static VarThrePrintable varThrePrintableV2(AppV2Name);

static bool setDate = false;
static std::string curDate;

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
        myPcType = PC_TYPE;
        pseudoNum = 0;

        pcPolicy = PCPolicy();

        pcPolicy.setCurPosition(&curPosition);
        pcPolicy.setMyId(&myId);
        pcPolicy.setMyPseudonym(&myPseudonym);
        pcPolicy.setPseudoNum(&pseudoNum);

        myPseudonym = pcPolicy.getNextPseudonym();

        //pseudonym-------------------------------

        mdAuthority.addNewNode(myPseudonym, myMdType, simTime().dbl());

        if(randomConf){
            double randConfPos = genLib.RandomDouble(0, confPos);
            double randConfSpeed = genLib.RandomDouble(0, randConfSpeed);
            double randConfHeading = genLib.RandomDouble(0, confHea);

            curPositionConfidence = Coord(randConfPos, randConfPos, 0);
            curSpeedConfidence = Coord(randConfSpeed, randConfSpeed, 0);
            curHeadingConfidence = Coord(randConfHeading, randConfHeading, 0);
        }else{
            curPositionConfidence = Coord(confPos, confPos, 0);
            curSpeedConfidence = Coord(confSpd, confSpd, 0);
            curHeadingConfidence = Coord(confHea, confHea, 0);
        }

        myReportType = REPORT_VERSION;

        switch (myMdType) {
        case mbTypes::Genuine: {
            TraCIColor color = TraCIColor(0, 255, 0, 0);
            traciVehicle->setColor(color);
            myAttackType = attackTypes::Genuine;
        }
            break;
        case mbTypes::Attacker: {

            //attack-------------------------------
            if(MixAttacks){
                int AtLiSize = sizeof(MixAttacksList)/sizeof(MixAttacksList[0]);

                int attackIndex = genLib.RandomInt(0, AtLiSize-1);
                myAttackType = MixAttacksList[attackIndex];
                std::cout<<"##############################################################NEW ATTACKER:"<< myPseudonym << "\n";
                std::cout<<"##############################################################AttackType:" << attackTypes::AttackNames[myAttackType] << "\n";
            }else{
                myAttackType = ATTACK_TYPE;
            }

            mdAttack = MDAttack();

            mdAttack.setBeaconInterval(&beaconInterval);
            mdAttack.setCurHeading(&curHeading);
            mdAttack.setCurHeadingConfidence(&curHeadingConfidence);
            mdAttack.setCurPosition(&curPosition);
            mdAttack.setCurPositionConfidence(&curPositionConfidence);
            mdAttack.setCurSpeed(&curSpeed) ;
            mdAttack.setCurSpeedConfidence(&curSpeedConfidence);
            mdAttack.setDetectedNodes(&detectedNodes);
            mdAttack.setMyBsm(myBsm);
            mdAttack.setMyBsmNum(&myBsmNum);
            mdAttack.setMyLength(&myLength);
            mdAttack.setMyPseudonym(&myPseudonym);
            mdAttack.setMyWidth(&myWidth);
            mdAttack.setPcPolicy(&pcPolicy);

            mdAttack.init(myAttackType);

            //attack-------------------------------

            TraCIColor color = TraCIColor(255, 0, 0, 0);
            traciVehicle->setColor(color);
        }
        break;
        default:
        TraCIColor color = TraCIColor(0, 0, 0, 0);
        traciVehicle->setColor(color);
        break;
    }

        if (!setDate) {
            char dateBuffer[50];
            time_t t = time(NULL);
            struct tm tm = *localtime(&t);
            sprintf(dateBuffer, "%d-%d-%d_%d:%d:%d", tm.tm_year + 1900,
                    tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
                    tm.tm_sec);
            std::string curDateTemp(dateBuffer);
            curDate = curDateTemp;
            setDate = true;
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
mbTypes::Mbs JosephVeinsApp::induceMisbehavior(double attacker) {

    double genuine = 1 - attacker;

    if (simTime().dbl() < START_ATTACK) {
        return mbTypes::Genuine;
    }

    if ((totalAttacker + totalGenuine) == 0) {
        totalGenuine++;
        return mbTypes::Genuine;
    }

    double realFactor = totalAttacker / (totalGenuine + totalAttacker);
    double theoFactor = attacker / (genuine + attacker);

    if (theoFactor > realFactor) {
        totalAttacker++;
        return mbTypes::Attacker;
    } else {
        totalGenuine++;
        return mbTypes::Genuine;
    }

}

BsmCheck bsmCheckV1;
BsmCheck bsmCheckV2;
void JosephVeinsApp::onBSM(BasicSafetyMessage* bsm) {

    unsigned long senderPseudonym = bsm->getSenderPseudonym();

    if (EnableV1) {
        LocalMisbehaviorDetection(bsm, 1);
    }
    if (EnableV2) {
        LocalMisbehaviorDetection(bsm, 2);
    }

    if (!detectedNodes.includes(senderPseudonym)) {
        NodeHistory newNode(senderPseudonym);
        newNode.addBSM(*bsm);
        MDMHistory newMDM(senderPseudonym);
        if (EnableV1) {
            newMDM.addBsmCheck(bsmCheckV1, 1);
        }
        if (EnableV2) {
            newMDM.addBsmCheck(bsmCheckV2, 2);
        }
        detectedNodes.put(senderPseudonym, newNode, newMDM);
    } else {
        NodeHistory existingNode = detectedNodes.getNodeHistory(
                senderPseudonym);
        existingNode.addBSM(*bsm);
        MDMHistory existingMDM = detectedNodes.getMDMHistory(senderPseudonym);
        if (EnableV1) {
            existingMDM.addBsmCheck(bsmCheckV1, 1);
        }
        if (EnableV2) {
            existingMDM.addBsmCheck(bsmCheckV2, 2);
        }
        detectedNodes.put(senderPseudonym, existingNode, existingMDM);
    }

    if (EnablePC) {
        pcPolicy.checkPseudonymChange(myPcType);
    }

//Your application has received a beacon message from another car or RSU
//code for handling the message goes here

}
void JosephVeinsApp::treatAttackFlags() {
    if (myMdType == mbTypes::Attacker) {
        updateVehicleInfo();

        attackBsm = mdAttack.launchAttack(myAttackType);

        if(mdAttack.getTargetNode()>=0){
            addTargetNode(mdAttack.getTargetNode());
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
    unsigned long senderPseudo = bsm->getSenderPseudonym();

    switch (version) {
    case 1: {
        std::string mdv = "V1";
        LegacyChecks mdm(myPseudonym, curPosition, curSpeed,
                Coord(myWidth, myLength), curHeading);
        bsmCheckV1 = mdm.CheckBSM(*bsm, detectedNodes);

        bool result = AppV1.CheckNodeForReport(myPseudonym, *bsm, bsmCheckV1,
                detectedNodes);
        if (result) {
            MDReport reportBase;
            reportBase.setGenerationTime(simTime().dbl());
            reportBase.setSenderPseudo(myPseudonym);
            reportBase.setReportedPseudo(senderPseudo);
            reportBase.setMbType(mbTypes::mbNames[bsm->getSenderMbType()]);
            reportBase.setAttackType(
                    attackTypes::AttackNames[bsm->getSenderAttackType()]);
            std::pair<double, double> currLonLat = traci->getLonLat(
                    curPosition);
            reportBase.setSenderGps(Coord(currLonLat.first, currLonLat.second));
            reportBase.setReportedGps(bsm->getSenderGpsCoordinates());

            char nameV1[32] = "mdaV1";
            mdAuthority.sendReport(nameV1, reportBase);

            if (writeReportsV1) {
                sendReport(reportBase, mdv, bsmCheckV1, bsm);
            }
        }

        if (writeBsmsV1) {
            writeMdBsm(mdv, bsmCheckV1, bsm);
        }

        if (!initV1) {
            AppV1.resetAllFlags();
            //mdAuthority.resetAll();
            initV1 = true;
        }
        if (PrintTPFP) {
            double minFactor = AppV1.getMinFactor();

            varThrePrintableV1.registerMessage(
                    mbTypes::intMbs[bsm->getSenderMbType()], minFactor);
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
            addAccusedNode(senderPseudo);
        }

        break;
    }
    case 2: {
        std::string mdv = "V2";
        CaTChChecks mdmV2(myPseudonym, curPosition, curPositionConfidence,
                curHeading, curHeadingConfidence, Coord(myWidth, myLength));
        BsmCheck bsmCheckV2 = mdmV2.CheckBSM(*bsm, detectedNodes);
        bool result = AppV2.CheckNodeForReport(myPseudonym, *bsm, bsmCheckV2,
                detectedNodes);
        if (result) {

            MDReport reportBase;
            reportBase.setGenerationTime(simTime().dbl());
            reportBase.setSenderPseudo(myPseudonym);
            reportBase.setReportedPseudo(senderPseudo);

            reportBase.setMbType(mbTypes::mbNames[bsm->getSenderMbType()]);
            reportBase.setAttackType(
                    attackTypes::AttackNames[bsm->getSenderAttackType()]);
            std::pair<double, double> currLonLat = traci->getLonLat(
                    curPosition);
            reportBase.setSenderGps(Coord(currLonLat.first, currLonLat.second));
            reportBase.setReportedGps(bsm->getSenderGpsCoordinates());

            char nameV2[32] = "mdaV2";
            mdAuthority.sendReport(nameV2, reportBase);

            if (writeReportsV2) {
                sendReport(reportBase, mdv, bsmCheckV2, bsm);
            }

        }

        if (writeBsmsV2) {
            writeMdBsm(mdv, bsmCheckV2, bsm);
        }

        if (!initV2) {
            AppV2.resetAllFlags();
            //mdAuthority.resetAll();
            initV2 = true;
        }
        if (PrintTPFP) {
            double minFactor = AppV2.getMinFactor();
            varThrePrintableV2.registerMessage(
                    mbTypes::intMbs[bsm->getSenderMbType()], minFactor);
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
            addAccusedNode(senderPseudo);
        }

        break;
    }

    default:
        break;
    }

}

void JosephVeinsApp::sendReport(MDReport reportBase, std::string version,
        BsmCheck bsmCheck, BasicSafetyMessage *bsm) {
    switch (myReportType) {
    case reportTypes::BasicCheckReport: {
        BasicCheckReport bcr = BasicCheckReport(reportBase);
        bcr.setReportedCheck(bsmCheck);
        bcr.writeStrToFile(savePath, serialNumber, version,
                bcr.getReportPrintableJson(), curDate);
    }
        break;

    case reportTypes::OneMessageReport: {
        OneMessageReport omr = OneMessageReport(reportBase);
        omr.setReportedBsm(*bsm);
        omr.setReportedCheck(bsmCheck);
        omr.writeStrToFile(savePath, serialNumber, version,
                omr.getReportPrintableJson(), curDate);
    }
        break;
    case reportTypes::EvidenceReport: {
        EvidenceReport evr = EvidenceReport(reportBase);
        evr.addEvidence(myBsm[0], bsmCheck, *bsm, detectedNodes);
        evr.writeStrToFile(savePath, serialNumber, version,
                evr.getReportPrintableJson(), curDate);
    }
        break;
    default:
        break;
    }
}

void JosephVeinsApp::writeMdBsm(std::string version, BsmCheck bsmCheck,
        BasicSafetyMessage *bsm) {
    BsmPrintable bsmPrint = BsmPrintable();
    bsmPrint.setReceiverPseudo(myPseudonym);
    bsmPrint.setBsm(*bsm);
    bsmPrint.setBsmCheck(bsmCheck);
    bsmPrint.writeStrToFile(savePath, serialNumber, version,
            bsmPrint.getBsmPrintableJson(), curDate);
}

void JosephVeinsApp::writeSelfBsm(
        BasicSafetyMessage bsm) {
    BsmPrintable bsmPrint = BsmPrintable();
    bsmPrint.setReceiverPseudo(myId);
    bsmPrint.setBsm(bsm);
    bsmPrint.writeSelfStrToFile(savePath, serialNumber,
            bsmPrint.getSelfBsmPrintableJson(), curDate);
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

    treatAttackFlags();

    BaseWaveApplLayer::handleSelfMsg(msg);

    if(writeSelfMsg){
        writeSelfBsm(myBsm[0]);
    }


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

void JosephVeinsApp::addTargetNode(unsigned long id) {
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
void JosephVeinsApp::removeTargetNode(unsigned long id) {
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
bool JosephVeinsApp::isTargetNode(unsigned long id) {
    for (int var = 0; var < targetNodesLength; ++var) {
        if (targetNodes[var] == id) {
            return true;
        }
    }
    return false;
}

void JosephVeinsApp::addAccusedNode(unsigned long id) {
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
void JosephVeinsApp::removeAccusedNode(unsigned long id) {
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
bool JosephVeinsApp::isAccusedNode(unsigned long id) {
    for (int var = 0; var < accusedNodesLength; ++var) {
        if (accusedNodes[var] == id) {
            return true;
        }
    }
    return false;
}

