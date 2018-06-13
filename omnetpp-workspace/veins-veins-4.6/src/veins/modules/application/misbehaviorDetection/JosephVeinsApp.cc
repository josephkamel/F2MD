/*******************************************************************************
 * @author  Joseph Kamel
 * @email   joseph.kamel@gmail.com
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
#define savePath "/home/joseph/Projects/ConfidenceRange/mdmSave/"

#define confPos 5
#define confSpd 0
#define confHea 0

#define FAULTY 0.9
#define ATTACKERS 0.1

#define SAVE_PERIOD 1 //60 seconds

#define START_SAVE 0 //60 seconds
#define START_ATTACK 10 //60 seconds

//#define DISRUPTIVE true

#define ATTACKTYPE 3
// attack 1 const addition
#define CONSTX 50
#define CONSTY 50
// attack 2 disruptive

// attack 3 replay

static int targetNodes[MAXTARGETLENGTH];
static int targetNodesLength = 0;
static double targetClearTime = 0;
static int accusedNodes[MAXACCUSEDLENGTH];
static int accusedNodesLength = 0;
static double accusedClearTime = 0;


static MDAuthority mdAuthority = MDAuthority();
char const *AppV1Name = "AppV1";
char const *AppV2Name = "AppV2";


static double TPV1[11]= {0,0,0,0,0,0,0,0,0,0,0};
static double TNV1[11]= {0,0,0,0,0,0,0,0,0,0,0};
static double FPV1[11]= {0,0,0,0,0,0,0,0,0,0,0};
static double FNV1[11]= {0,0,0,0,0,0,0,0,0,0,0};

static double TPV2[11]= {0,0,0,0,0,0,0,0,0,0,0};
static double TNV2[11]= {0,0,0,0,0,0,0,0,0,0,0};
static double FPV2[11]= {0,0,0,0,0,0,0,0,0,0,0};
static double FNV2[11]= {0,0,0,0,0,0,0,0,0,0,0};


//ThresholdApp AppV1(AppV1Name, 0.5);
//ThresholdApp AppV2(AppV2Name, 0.5);

AggrigationApp AppV1(AppV1Name,1,10.0,5);
AggrigationApp AppV2(AppV2Name,2,10.0,5);


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


            TraCIColor color = TraCIColor(0, 255, 0, 0);
            traciVehicle->setColor(color);
        } else if (myMdType == 2) {
            curPositionConfidence = Coord(confPos, confPos, 0);
            curSpeedConfidence = Coord(confSpd, confSpd, 0);
            curHeadingConfidence = Coord(confHea, confHea, 0);

            myAttackType = ATTACKTYPE;
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

static double totalFaulty = 0;
static double totalAttacker = 0;
double JosephVeinsApp::induceMisbehaviorFixed(double faulty, double attackers) {

    if (simTime().dbl() < START_ATTACK) {
        return 1;
    }

    if ((totalAttacker + totalFaulty) == 0) {
        totalFaulty++;
        return 1;
    }

    double realFactor = totalAttacker / (totalFaulty + totalAttacker);
    double theoFactor = attackers / (faulty + attackers);

    if (theoFactor > realFactor) {
        totalAttacker++;
        return 2;
    } else {
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

    MDModule mdm(myId, curPosition, curSpeed, Coord(myWidth, myLength),
            curHeading);
    MDModuleV2 mdmV2(myId, curPosition, curPositionConfidence, curHeading,
            curHeadingConfidence, Coord(myWidth, myLength));
    BsmCheck bsmCheckV1 = mdm.CheckBSM(*bsm, detectedNodes);
    BsmCheck bsmCheckV2 = mdmV2.CheckBSM(*bsm, detectedNodes);

    tuple<bool, MBReport> resultV1 = AppV1.CheckNodeForReport(myId, *bsm,
            bsmCheckV1, detectedNodes, bsm->getSenderMbType());
    tuple<bool, MBReport> resultV2 = AppV2.CheckNodeForReport(myId, *bsm,
            bsmCheckV2, detectedNodes, bsm->getSenderMbType());
    if (get < 0 > (resultV1)) {
        mdm.SendReport(&mdAuthority, get < 1 > (resultV1));
    }

    if (get < 0 > (resultV2)) {
        mdmV2.SendReport(&mdAuthority, get < 1 > (resultV2));
    }

    //std::cout<< "--------------------------------------"<< '\n';;

    double minFactorV1 = AppV1.getMinFactor(myId, *bsm, bsmCheckV1, detectedNodes,
            bsm->getSenderMbType());
    int indexV1 = 0;
    for (double var = 0; var <=1 ; var = var+0.1) {
        if (minFactorV1 <= var) {
            if (bsm->getSenderMbType() == 2) {
                TPV1[indexV1]++;
            }else{
                FPV1[indexV1]++;
            }
        }else{
            if (bsm->getSenderMbType() == 2) {
                FNV1[indexV1]++;
            }else{
                TNV1[indexV1]++;
            }
        }
        indexV1++;
    }

    double minFactorV2 = AppV2.getMinFactor(myId, *bsm, bsmCheckV2, detectedNodes,
            bsm->getSenderMbType());

    int indexV2 = 0;
    for (double var = 0; var <=1 ; var = var+0.1) {
        if (minFactorV2 <= var) {
            if (bsm->getSenderMbType() == 2) {
                TPV2[indexV2]++;
            }else{
                FPV2[indexV2]++;
            }
        }else{
            if (bsm->getSenderMbType() == 2) {
                FNV2[indexV2]++;
            }else{
                TNV2[indexV2]++;
            }
        }
        indexV2++;
    }


    if (!detectedNodes.includes(senderID)) {
        NodeHistory newNode(senderID);
        newNode.addBSM(*bsm);
        MDMHistory newMDM(senderID);
        newMDM.addBsmCheck(bsmCheckV1, bsmCheckV2);
        detectedNodes.put(senderID, newNode, newMDM);
    } else {
        NodeHistory existingNode = detectedNodes.getNodeHistory(senderID);
        existingNode.addBSM(*bsm);
        MDMHistory existingMDM = detectedNodes.getMDMHistory(senderID);
        existingMDM.addBsmCheck(bsmCheckV1, bsmCheckV2);
        detectedNodes.put(senderID, existingNode, existingMDM);
    }

    if (!init) {
        AppV1.resetAllFlags();
        AppV2.resetAllFlags();
        //mdAuthority.resetAll();
        init = true;
    }

    if ((simTime().dbl() - deltaT) > SAVE_PERIOD) {
        deltaT = simTime().dbl();

        if (simTime().dbl() > START_SAVE) {
            AppV1.saveLine(savePath, serialNumber,
                    mobility->getManager()->getManagedHosts().size(), deltaT);
            AppV2.saveLine(savePath, serialNumber,
                    mobility->getManager()->getManagedHosts().size(), deltaT);
            mdAuthority.saveLine(savePath, serialNumber, deltaT);
        }
        AppV1.resetInstFlags();
        AppV2.resetInstFlags();

        std::cout << "V1        TP        FP      TN      FN" << '\n';
        for (int var = 0; var < indexV1; ++var) {
            std::cout <<var<<"        " << TPV1[var] <<"       "<<FPV1[var]<< "       "<< TNV1[var]<<"     "<< FNV1[var]<< '\n';
        }
        std::cout << "V2        TP        FP      TN      FN" << '\n';
        for (int var = 0; var < indexV2; ++var) {
            std::cout <<var<<"        " << TPV2[var] <<"       "<<FPV2[var]<< "       "<< TNV2[var]<<"     "<< FNV2[var]<< '\n';
        }

    }

    if(minFactorV1 < 0.5){
        addAccusedNode(senderID);
    }

    if (myMdType == 2) {
        if(myAttackType > 1){
            if (detectedNodes.getNodesNum() > 0) {
                attackBsm = nextAttackBsm;
                if(myAttackType == 2){
                    nextAttackBsm = detectedNodes.getRandomBSM();
                    addTargetNode(nextAttackBsm.getSenderAddress());
                }
                if(myAttackType == 3){
                    nextAttackBsm = detectedNodes.getNextAttackedBsm(curPosition,
                            nextAttackBsm.getSenderAddress(),
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

        if(isAccusedNode(myId)){
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

