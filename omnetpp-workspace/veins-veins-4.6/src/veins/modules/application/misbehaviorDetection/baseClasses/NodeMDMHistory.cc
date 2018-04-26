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

#include <veins/modules/application/misbehaviorDetection/baseClasses/NodeMDMHistory.h>


MDMHistory::MDMHistory() {
    nodeId = 0;
    BSMNum = 0;
}

MDMHistory::MDMHistory(int id) {
    nodeId = id;
    BSMNum = 0;
}

BsmCheck MDMHistory::getBsmCheck(int index, int version) {
    if(version == 1){
        return bsmCheckListV1[index];
    }else{
        return bsmCheckListV2[index];
    }
}


void MDMHistory::setBsmCheck(int index, BsmCheck bsmCheckV1, BsmCheck bsmCheckV2) {
    bsmCheckListV1[index] = bsmCheckV1;
    bsmCheckListV2[index] = bsmCheckV2;
}

void MDMHistory::addBsmCheck(BsmCheck bsmCheckV1, BsmCheck bsmCheckV2) {
    if (BSMNum < MAXMDMLENGTH) {
        BSMNum++;
    }
    for (int var = BSMNum - 1; var > 0; --var) {
        bsmCheckListV1[var] = bsmCheckListV1[var - 1];
        bsmCheckListV2[var] = bsmCheckListV2[var - 1];
    }
    bsmCheckListV1[0] = bsmCheckV1;
    bsmCheckListV2[0] = bsmCheckV2;
}
