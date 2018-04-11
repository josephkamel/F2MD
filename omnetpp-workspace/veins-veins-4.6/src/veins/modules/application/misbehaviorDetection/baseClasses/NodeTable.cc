/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
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

int NodeTable::getNodeId(int index) {
    return nodeIds[index];
}

void NodeTable::put(int nodeId, NodeHistory nodeHistory,
        MDMHistory mdmHistory) {

    bool included = false;
    int nodeKey;

    for (int var = 0; var < nodesNum; ++var) {
        if (nodeId == nodeIds[var]) {
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
            nodeIds[nodesNum] = nodeId;
            nodeHistoryList[nodesNum] = nodeHistory;
            mdmHistoryList[nodesNum] = mdmHistory;
            nodesNum++;
        } else {
            nodeKey = getOldestNode();
            nodeIds[nodeKey] = nodeId;
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
    int totalNodes = sizeof(nodeIds) / sizeof(nodeIds[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodeId == nodeIds[var]) {
            return nodeHistoryList[var];
        }
    }
    NodeHistory nullNode;
    return nullNode;
}

void NodeTable::setNodeHistory(int nodeId ,NodeHistory nodeHistory){
    int totalNodes = sizeof(nodeIds) / sizeof(nodeIds[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodeId == nodeIds[var]) {
            nodeHistoryList[var] = nodeHistory;
        }
    }
}

void NodeTable::setMDMHistory(int nodeId ,MDMHistory mdmHistory){
    int totalNodes = sizeof(nodeIds) / sizeof(nodeIds[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodeId == nodeIds[var]) {
            mdmHistoryList[var] = nodeId;
        }
    }
}

MDMHistory NodeTable::getMDMHistory(int nodeId) {
    int totalNodes = sizeof(nodeIds) / sizeof(nodeIds[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodeId == nodeIds[var]) {
            return mdmHistoryList[var];
        }
    }
    MDMHistory nullNode;
    return nullNode;
}

bool NodeTable::includes(int nodeId) {
    int totalNodes = sizeof(nodeIds) / sizeof(nodeIds[0]);
    for (int var = 0; var < totalNodes; ++var) {
        if (nodeId == nodeIds[var]) {
            return true;
        }
    }
    return false;
}

double NodeTable::getDeltaTime(int nodeId1, int nodeId2) {
    return fabs(
            getNodeHistory(nodeId1).getArrivalTime(0)
                    - getNodeHistory(nodeId2).getArrivalTime(0));
}

BasicSafetyMessage NodeTable::getRandomBSM() {
    GeneralLib genLib = GeneralLib();
    int randNode = genLib.RandomInt(0, nodesNum-1);
    int randBSM = genLib.RandomInt(0, nodeHistoryList[randNode].getBSMNum()-1);
    return nodeHistoryList[randNode].getBSM(randBSM);
}

