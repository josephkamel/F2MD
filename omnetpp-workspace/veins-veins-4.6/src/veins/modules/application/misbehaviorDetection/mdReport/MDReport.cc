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

#include "MDReport.h"


MDReport::MDReport() {
//    generationTime=0;
//    senderId=0;
//    reportedId=0;
//    mbType = 0;
}

double MDReport::getGenerationTime() {
    return generationTime;
}

int MDReport::getSenderId() {
    return senderId;
}
int MDReport::getReportedId() {
    return reportedId;
}

std::string MDReport::getMbType() {
    return mbType;
}


std::string MDReport::getAttackType() {
    return attackType;
}

void MDReport::setGenerationTime(double time) {
    generationTime = time;
}

void MDReport::setSenderId(int id) {
    senderId = id;
}
void MDReport::setReportedId(int id) {
    reportedId = id;
}
void MDReport::setMbType(std::string type) {
    mbType = type;
}

void MDReport::setAttackType(std::string type) {
    attackType = type;
}

std::string MDReport::getBaseReportXml() {
    std::string tempStr = "";

    XmlWriter xml;
    xml.init();

    xml.writeOpenTag("Metadata");

    xml.writeStartElementTag("senderId");
    xml.writeString(std::to_string(senderId));
    xml.writeEndElementTag();

    xml.writeStartElementTag("reportedId");
    xml.writeString(std::to_string(reportedId));
    xml.writeEndElementTag();

    xml.writeStartElementTag("generationTime");
    xml.writeString(std::to_string(generationTime));
    xml.writeEndElementTag();

    xml.writeStartElementTag("mbType");
    xml.writeString(mbType);
    xml.writeEndElementTag();

    xml.writeStartElementTag("attackType");
    xml.writeString(attackType);
    xml.writeEndElementTag();

    xml.writeCloseTag();

    return xml.getOutString();
}


bool MDReport::writeStrToFile(const std::string strFileCnst,
        const std::string serial, const std::string version,
        const std::string outStr) {
    int gentime = generationTime;
    int gentime00 = (generationTime - gentime) * 100;

    std::string strFile = strFileCnst + serial + "/MDReports/MDReport_"
            + version + "_" + std::to_string(gentime) + "-"
            + std::to_string(gentime00) + "_" + std::to_string(senderId) + "_"
            + std::to_string(reportedId) + ".xml";

    std::fstream checkFile(strFile);

    std::cout << strFile << "\n";

    if (checkFile.is_open()) {

        std::cout << strFile << "\n";
        std::cout << "Error: File alread exists.\n";
        exit(0);
        return false;
    }

    std::ofstream outFile;

    outFile.open(strFile);
    if (outFile.is_open()) {

        outFile << outStr;

        outFile.close();
    }

    return true;

}

