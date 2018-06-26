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

#include "MDABase.h"

MDABase::MDABase() {
    init = true;
    totalGenuineNum = 0;
    totalAttackerNum = 0;
    reportedGenuineNum = 0;
    reportedAttackerNum = 0;
    GenuineAverageReportDelay = 0;
    attackerAverageReportDelay = 0;

}

MDABase::MDABase(const char * name) {
    strcpy(this->name, name);
    init = true;
    totalGenuineNum = 0;
    totalAttackerNum = 0;
    reportedGenuineNum = 0;
    reportedAttackerNum = 0;
    GenuineAverageReportDelay = 0;
    attackerAverageReportDelay = 0;
}

char* MDABase::getName(){
    return this->name;
}

void MDABase::setName(const char * name){
    strcpy(this->name, name);
}

void MDABase::resetAll(){
    totalGenuineNum = 0;
    totalAttackerNum = 0;
    reportedGenuineNum = 0;
    reportedAttackerNum = 0;
    GenuineAverageReportDelay = 0;
    attackerAverageReportDelay = 0;
}

void MDABase::addTotalGenuine(int id, double time) {
    totalGenuineIdList[totalGenuineNum] = id;
    totalGenuineTimeList[totalGenuineNum] = time;
    totalGenuineNum++;
}

void MDABase::addTotalAttacker(int id, double time) {
    totalAttackerIdList[totalAttackerNum] = id;
    totalAttackerTimeList[totalAttackerNum] = time;
    totalAttackerNum++;
}

void MDABase::addReportedGenuine(int id, double time) {
    reportedGenuineIdList[reportedGenuineNum] = id;
    reportedGenuineTimeList[reportedGenuineNum] = time;

    int totalIndex = totalGenuineIndex(id);
    double deltaTime = time - totalGenuineTimeList[totalIndex];

    GenuineAverageReportDelay = (GenuineAverageReportDelay * (double)reportedGenuineNum
            + deltaTime) / ((double)reportedGenuineNum + 1);

    reportedGenuineNum++;
}

void MDABase::addReportedAttacker(int id, double time) {
    reportedAttackerIdList[reportedAttackerNum] = id;
    reportedAttackerTimeList[reportedAttackerNum] = time;

    int totalIndex = totalAttackerIndex(id);
    double deltaTime = time - totalAttackerTimeList[totalIndex];

    attackerAverageReportDelay = (attackerAverageReportDelay * (double)reportedAttackerNum
            + deltaTime) / ((double)reportedAttackerNum + 1);

    reportedAttackerNum++;
}

void MDABase::writeFile(std::string path, char* printStr) {
    ofstream outFile;
    if(init){
        outFile.open(path,
                std::ofstream::out);
        init = false;
    }else{
        outFile.open(path,
                std::ofstream::out | std::ofstream::app | std::ofstream::ate);
    }

    outFile.seekp(0, std::ios::end);
    outFile << printStr << "\n";
    outFile.close();
}

bool MDABase::alreadyReportedGenuine(int id) {
    for (int var = 0; var < reportedGenuineNum; ++var) {
        if (id == reportedGenuineIdList[var]) {
            return true;
        }
    }
    return false;
}

bool MDABase::alreadyReportedAttacker(int id) {
    for (int var = 0; var < reportedAttackerNum; ++var) {
        if (id == reportedAttackerIdList[var]) {
            return true;
        }
    }
    return false;
}

int MDABase::totalGenuineIndex(int id) {
    for (int var = 0; var < totalGenuineNum; ++var) {
        if (id == totalGenuineIdList[var]) {
            return var;
        }
    }
    return -1;
}

int MDABase::totalAttackerIndex(int id) {
    for (int var = 0; var < totalAttackerNum; ++var) {
        if (id == totalAttackerIdList[var]) {
            return var;
        }
    }
    return -1;
}

void MDABase::getPrintable(char* outStr, double time) {

    char line[1024] = "";
    char data[64] = "";
    strcat(line, name);
    strcat(line, " ");
    sprintf(data, "%f", time);
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%d", reportedGenuineNum);
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%d", totalGenuineNum);
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", GenuineAverageReportDelay);
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%d", reportedAttackerNum);
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%d", totalAttackerNum);
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", attackerAverageReportDelay);
    strcat(line, data);

    for (int i = 0; i < 1024; ++i) {
        outStr[i] = line[i];
    }

    std::cout << "=*-=*-=*-=*- " << name << " =*-=*-=*-=*- Genuine:"
            << reportedGenuineNum << "/" << totalGenuineNum<<" "
            << (double)reportedGenuineNum / (double)totalGenuineNum * 100 << "% "
            << GenuineAverageReportDelay <<"s |Attacker:"
            << reportedAttackerNum << "/" << totalAttackerNum<<" "
            << (double)reportedAttackerNum / (double)totalAttackerNum * 100 << "% "
            << attackerAverageReportDelay <<"s"<<'\n';
}
