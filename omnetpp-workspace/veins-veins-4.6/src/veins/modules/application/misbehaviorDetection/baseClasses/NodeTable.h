/*******************************************************************************
* @author  Joseph Kamel 
* @email   josephekamel@gmail.com
* @date    28/11/2018
* @version 1.1
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
    NodeHistory nodeHistoryList[MAX_NODES_LENGTH];
    MDMHistory mdmHistoryList[MAX_NODES_LENGTH];
    unsigned long nodePseudos[MAX_NODES_LENGTH];

    NodeHistory nullNode;
    MDMHistory nullMDMNode;

    int getOldestNode();

public:
    NodeTable();
    int getNodesNum();
    NodeHistory* getNodeHistoryList();
    unsigned long getNodePseudo(int);
    void put(unsigned long, NodeHistory, MDMHistory);
    NodeHistory* getNodeHistoryAddr(int);
    MDMHistory* getMDMHistoryAddr(unsigned long nodePseudonym);

    void setNodeHistory(int index,NodeHistory);
    void setMDMHistory(int index,MDMHistory);

    bool includes(unsigned long nodePseudonym);

    double getDeltaTime(unsigned long nodePseudo1, unsigned long nodePseudo2);

    BasicSafetyMessage* getRandomBSM();
    BasicSafetyMessage* getNextAttackedBsm(Coord myPosition, int bsmNode, double bsmTime);
};

#endif
