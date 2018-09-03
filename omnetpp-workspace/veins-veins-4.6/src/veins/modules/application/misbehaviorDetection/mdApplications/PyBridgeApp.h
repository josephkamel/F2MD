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

#ifndef __VEINS_PyBridgeApp_H_
#define __VEINS_PyBridgeApp_H_

#include <tuple>
#include <omnetpp.h>
#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string>
#include <vector>
#include "MDApplication.h"

#include "../supportClasses/BsmPrintable.h"

using namespace Veins;
using namespace omnetpp;

class PyBridgeApp: public MDApplication {
public:

    int version = 1;

    int port = 80;
    std::string host = "localhost";
    double minFactor = 1;

    PyBridgeApp(int version ,int port, std::string host);

    bool CheckNodeForReport(unsigned long myPseudonym,
            BasicSafetyMessage bsm, BsmCheck bsmCheck, NodeTable detectedNodes);


    double getMinFactor();
};

#endif
