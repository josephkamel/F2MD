/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#include "MBReport.h"

MBReport::MBReport() {
    generationTime=0;
    senderId=0;
    reportedId=0;
    mbType = 0;
//    failedCheckListNum=0;
//    bsmListNum=0;
}

double MBReport::getGenerationTime(){
    return generationTime;
}

int MBReport::getSenderId(){
    return senderId;
}
int MBReport::getReportedId(){
    return reportedId ;
}

double MBReport::getMbType(){
    return mbType;
}

void MBReport::setGenerationTime(double time){
    generationTime = time;
}

void MBReport::setSenderId(int id){
    senderId = id;
}
void MBReport::setReportedId(int id){
    reportedId = id;
}
void MBReport::setMbType(double type){
    mbType = type;
}

//void MBReport::addFailedCheck(std::string check){
//    failedCheckList[failedCheckListNum] = check;
//    failedCheckListNum++;
//}
//
//void MBReport::addBsmCheck(BsmCheck bsmCheck){
//    reportedBsmList[bsmListNum] = bsmCheck;
//    bsmListNum++;
//}





