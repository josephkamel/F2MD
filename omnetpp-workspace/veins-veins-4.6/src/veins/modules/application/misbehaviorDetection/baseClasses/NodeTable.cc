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

#include "NodeTable.h"

NodeTable::NodeTable() {
    nodesNum = 0;
}

int NodeTable::getNodesNum() {
    return nodesNum;
}

NodeHistory* NodeTable::getNodeHistoryList() {
    return nodeHistoryList;
}

unsigned long NodeTable::getNodePseudo(int index) {
    return nodePseudos[index];
}

void NodeTable::put(unsigned long pseudo, NodeHistory nodeHistory,
        MDMHistory mdmHistory) {

    bool included = false;
    int nodeKey;

    for (int var = 0; var < nodesNum; ++var) {
        if (pseudo == nodePseudos[var]) {
            included = true;
            nodeKey = var;
            break;
        }
    }

    if (included) {
        nodeHistoryList[nodeKey] = nodeHistory;
        mdmHistoryList[nodeKey] = mdmHistory;
    } else {
        if (nodesNum < MAXNODESLENGTH) {
            nodePseudos[nodesNum] = pseudo;
            nodeHistoryList[nodesNum] = nodeHistory;
            mdmHistoryList[nodesNum] = mdmHistory;
            nodesNum++;
        } else {
            nodeKey = getOldestNode();
            nodePseudos[nodeKey] = pseudo;
            nodeHistoryList[nodeKey] = nodeHistory;
            mdmHistoryList[nodeKey] = mdmHistory;
        }
    }
}

int NodeTable::getOldestNode() {
    int oldestNodeIndex = 0;
    double oldestNodeTime =
            nodeHistoryList[0].getLatestBSM().getSendingTime().dbl();
    double currentNodeTime = 0;

    for (int var = 0; var < nodesNum; ++var) {
        currentNodeTime =
                nodeHistoryList[var].getLatestBSM().getSendingTime().dbl();
        if (currentNodeTime < oldestNodeTime) {
            oldestNodeTime = currentNodeTime;
            oldestNodeIndex = var;
        }
    }
    return oldestNodeIndex;
}

NodeHistory NodeTable::getNodeHistory(int nodeId) {
    int totalNodes = sizeof(nodePseudos) / sizeof(nodePseudos[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodeId == nodePseudos[var]) {
            return nodeHistoryList[var];
        }
    }
    NodeHistory nullNode;
    return nullNode;
}

void NodeTable::setNodeHistory(int nodeId ,NodeHistory nodeHistory){
    int totalNodes = sizeof(nodePseudos) / sizeof(nodePseudos[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodeId == nodePseudos[var]) {
            nodeHistoryList[var] = nodeHistory;
        }
    }
}

void NodeTable::setMDMHistory(int nodeId ,MDMHistory mdmHistory){
    int totalNodes = sizeof(nodePseudos) / sizeof(nodePseudos[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodeId == nodePseudos[var]) {
            mdmHistoryList[var] = nodeId;
        }
    }
}

MDMHistory NodeTable::getMDMHistory(unsigned long nodePseudonym) {
    int totalNodes = sizeof(nodePseudos) / sizeof(nodePseudos[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodePseudonym == nodePseudos[var]) {
            return mdmHistoryList[var];
        }
    }
    MDMHistory nullNode;
    return nullNode;
}

bool NodeTable::includes(unsigned long nodePseudonym) {
    int totalNodes = sizeof(nodePseudos) / sizeof(nodePseudos[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodePseudonym == nodePseudos[var]) {
            return true;
        }
    }
    return false;
}

double NodeTable::getDeltaTime(unsigned long nodePseudo1, unsigned long nodePseudo2) {
    return fabs(
            getNodeHistory(nodePseudo1).getArrivalTime(0)
                    - getNodeHistory(nodePseudo2).getArrivalTime(0));
}

BasicSafetyMessage NodeTable::getRandomBSM() {
    GeneralLib genLib = GeneralLib();
    int randNode = genLib.RandomInt(0, nodesNum-1);
    int randBSM = genLib.RandomInt(0, nodeHistoryList[randNode].getBSMNum()-1);
    return nodeHistoryList[randNode].getBSM(randBSM);
}

BasicSafetyMessage NodeTable::getNextAttackedBsm(Coord myPosition, int bsmNode, double bsmTime) {
    if(bsmNode==0 || (simTime().dbl() - bsmTime) > 1.1){
        double minDistance = 10000000;
        int index = -1;
        MDMLib mdmLib = MDMLib();
        for (int var = 0; var < nodesNum; ++var) {
            double distance = mdmLib.calculateDistance(myPosition,nodeHistoryList[var].getLatestBSM().getSenderPos());
            if(minDistance > distance){
                minDistance = distance;
                index = var;
            }
        }
        return nodeHistoryList[index].getLatestBSM();
    }else{
        BasicSafetyMessage latestbsm = getNodeHistory(bsmNode).getLatestBSM();
        return latestbsm;
    }
}


