/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_NODETABLE_H_
#define __VEINS_NODETABLE_H_

#include <veins/modules/application/misbehaviorDetection/baseClasses/NodeMDMHistory.h>
#include "NodeHistory.h"

#include "../supportClasses/GeneralLib.h"
#include "../supportClasses/MDMLib.h"

class NodeTable {

private:
    int nodesNum;
    NodeHistory nodeHistoryList[MAXNODESLENGTH];
    MDMHistory mdmHistoryList[MAXNODESLENGTH];
    int nodeIds[MAXNODESLENGTH];

    int getOldestNode();

public:
    NodeTable();
    int getNodesNum();
    NodeHistory* getNodeHistoryList();
    int getNodeId(int);
    void put(int, NodeHistory, MDMHistory);
    NodeHistory getNodeHistory(int);
    MDMHistory getMDMHistory(int);

    void setNodeHistory(int index,NodeHistory);
    void setMDMHistory(int index,MDMHistory);

    bool includes(int);

    double getDeltaTime(int, int);

    BasicSafetyMessage getRandomBSM();
};

#endif
