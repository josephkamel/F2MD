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

#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>

#include "MDApplication.h"

using namespace std;
using namespace boost;



MDApplication::MDApplication(const char* name) {
    strcpy(appName,name);

    prntApp.setName(name);
    char nameInst[32];
    strcpy(nameInst,name);
    strcat(nameInst,"Inst");
    prntAppInst.setName(nameInst);
}



void MDApplication::SendReport(MDAuthority* mdAuthority, MBReport mbReport) {
    mdAuthority->sendReport(appName, mbReport);
}


static bool init = true;
void MDApplication::saveLine(std::string path, std::string serial, double density,
        double deltaT){

    char fileNameApp[64];
    char fileNameAppInst[64];
    strcpy(fileNameApp,appName);
    strcat(fileNameApp, ".dat");
    strcpy(fileNameAppInst,appName);
    strcat(fileNameAppInst, "Inst.dat");

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

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameApp);

    prntApp.getPrintable(outChar, density, deltaT);
    prntApp.writeFile(filePathGen, outChar, init);

    strcpy(filePathGen, directoryPathGen);
    strcat(filePathGen, "/");
    strcat(filePathGen, fileNameAppInst);

    prntAppInst.getPrintable(outChar, density, deltaT);
    prntAppInst.writeFile(filePathGen, outChar, init);

    if (init) {
        init = false;
    }
}


void MDApplication::resetInstFlags(){
    prntAppInst.resetAll();
}

void MDApplication::resetAllFlags(){
    prntApp.resetAll();
    prntAppInst.resetAll();
}
