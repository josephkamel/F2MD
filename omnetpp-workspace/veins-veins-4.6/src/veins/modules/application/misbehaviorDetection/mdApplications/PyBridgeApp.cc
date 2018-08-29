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



#include "PyBridgeApp.h"
#include "../supportClasses/HTTPRequest.h"

using namespace std;
using namespace boost;

PyBridgeApp::PyBridgeApp(const char* name, int port, std::string host) :
        MDApplication(name) {
    this->port = port;
    this->host = host;
}

bool PyBridgeApp::CheckNodeForReport(unsigned long myPseudonym, BasicSafetyMessage bsm,
        BsmCheck bsmCheck, NodeTable detectedNodes) {

    minFactor = 1;

    HTTPRequest httpr = HTTPRequest(9998, "localhost");

    BsmPrintable bsmPrint = BsmPrintable();
    bsmPrint.setReceiverPseudo(myPseudonym);
    bsmPrint.setBsm(bsm);
    bsmPrint.setBsmCheck(bsmCheck);

    std::string bsmJsonStr = bsmPrint.getBsmPrintableJson();

    std::string response = httpr.Request(bsmJsonStr);

    std::cout<<"response:" << response << "\n";

    return true;
}

double PyBridgeApp::getMinFactor() {
    return minFactor;
}
