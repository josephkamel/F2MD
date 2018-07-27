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

#include "MDAuthority.h"

MDAuthority::MDAuthority() {
    char nameV1[32] = "mdaV1";
    char nameV2[32] = "mdaV2";
    registerNewBase(nameV1);
    registerNewBase(nameV2);
}

void MDAuthority::registerNewBase(char* baseName) {
    baseList[baseListNum].setName(baseName);
    baseListNum++;
}

void MDAuthority::addNewNode(unsigned long pseudo, std::string mbType, double time) {
    if (!mbType.compare("genuine")) {
        for (int var = 0; var < baseListNum; ++var) {
            baseList[var].addTotalGenuine(pseudo, time);
        }
    }
    if (!mbType.compare("attacker")) {
        for (int var = 0; var < baseListNum; ++var) {
            baseList[var].addTotalAttacker(pseudo, time);
        }
    }
}

void MDAuthority::addReportedNode(unsigned long pseudo, std::string mbType, double time) {
    if (!mbType.compare("genuine")) {
        for (int var = 0; var < baseListNum; ++var) {
            baseList[var].addReportedGenuine(pseudo, time);
        }
    }
    if (!mbType.compare("attacker")) {
        for (int var = 0; var < baseListNum; ++var) {
            baseList[var].addReportedAttacker(pseudo, time);
        }
    }
}

void MDAuthority::sendReport(char* baseName, MDReport report) {
    int index = -1;
    for (int var = 0; var < baseListNum; ++var) {
        if (strcmp(baseList[var].getName(), baseName) == 0) {
            index = var;
            break;
        }
    }
    if (index != -1) {
        treatReport(baseList, index, report);
    } else {
        baseList[baseListNum].setName(baseName);
        treatReport(baseList, baseListNum, report);
        baseListNum++;
    }
}

void MDAuthority::treatReport(MDABase *base, int index, MDReport report) {
    if (!report.getMbType().compare("genuine")) {
        if (!base[index].alreadyReportedGenuine(report.getReportedPseudo())) {
            base[index].addReportedGenuine(report.getReportedPseudo(),
                    report.getGenerationTime());
        }
    }

    if (!report.getMbType().compare("attacker")) {
        if (!base[index].alreadyReportedAttacker(report.getReportedPseudo())) {
            base[index].addReportedAttacker(report.getReportedPseudo(),
                    report.getGenerationTime());
        }
    }

}

void MDAuthority::saveLine(std::string path, std::string serial, double time) {

    char outChar[1024];
    char directoryPathGen[1024] = "";
    char filePathGen[1024] = "";
    const char * pathChar = path.c_str();
    const char * serialChar = serial.c_str();
    strcat(directoryPathGen, pathChar);
    strcat(directoryPathGen, serialChar);

    struct stat info;

    if (stat(directoryPathGen, &info) != 0) {
        mkdir(directoryPathGen, 0777);
    } else if (info.st_mode & S_IFDIR) {
    } else {
        mkdir(directoryPathGen, 0777);
    }

    for (int var = 0; var < baseListNum; ++var) {
        char fileName[64];
        strcpy(fileName, baseList[var].getName());
        strcat(fileName, ".dat");

        strcpy(filePathGen, directoryPathGen);
        strcat(filePathGen, "/");
        strcat(filePathGen, fileName);

        baseList[var].getPrintable(outChar, time);
        baseList[var].writeFile(filePathGen, outChar);
    }
}

void MDAuthority::resetAll() {
    for (int var = 0; var < baseListNum; ++var) {
        baseList[var].resetAll();
    }
}
