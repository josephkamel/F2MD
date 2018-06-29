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

static bool setDate = false;
static std::string curDate;

MDReport::MDReport() {

    if (!setDate) {
        char dateBuffer[50];
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(dateBuffer, "%d-%d-%d_%d:%d:%d", tm.tm_year + 1900,
                tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        std::string curDateTemp(dateBuffer);
        curDate = curDateTemp;
        setDate = true;
    }

//    generationTime=0;
//    senderId=0;
//    reportedId=0;
//    mbType = 0;
}

void MDReport::setBaseReport(MDReport baseReport) {
    generationTime = baseReport.getGenerationTime();
    senderId = baseReport.getSenderId();
    reportedId = baseReport.getReportedId();
    mbType = baseReport.getMbType();
    attackType = baseReport.getAttackType();
    senderGps = baseReport.getSenderGps();
    reportedGps = baseReport.getReportedGps();
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

void MDReport::setSenderGps(Coord Gps){
    senderGps = Gps;
}

void MDReport::setReportedGps(Coord Gps){
    reportedGps = Gps;
}

Coord MDReport::getSenderGps(){
    return senderGps;
}

Coord MDReport::getReportedGps(){
    return reportedGps;
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

std::string MDReport::getBaseReportJson(std::string reportTypeStr) {

    std::string tempStr ="";

    JsonWriter jw;
    jw.openJsonElement("Metadata", false);

    tempStr = jw.getSimpleTag("senderId",std::to_string(senderId),true);
    jw.addTagToElement("Metadata", tempStr);
    tempStr = jw.getSimpleTag("reportedId",std::to_string(reportedId),true);
    jw.addTagToElement("Metadata", tempStr);
    tempStr = jw.getSimpleTag("generationTime",std::to_string(generationTime),true);
    jw.addTagToElement("Metadata", tempStr);

    jw.openJsonElementList("senderGps");
    jw.addTagToElement("senderGps", std::to_string(senderGps.x));
    jw.addFinalTagToElement("senderGps", std::to_string(senderGps.y));
    tempStr = jw.getJsonElementList("senderGps");
    jw.addTagToElement("Metadata", tempStr);

    jw.openJsonElementList("reportedGps");
    jw.addTagToElement("reportedGps", std::to_string(reportedGps.x));
    jw.addFinalTagToElement("reportedGps", std::to_string(reportedGps.y));
    tempStr = jw.getJsonElementList("reportedGps");
    jw.addTagToElement("Metadata", tempStr);

    tempStr = jw.getSimpleTag("mbType",mbType,false);
    jw.addTagToElement("Metadata", tempStr);
    tempStr = jw.getSimpleTag("attackType",attackType,false);
    jw.addTagToElement("Metadata", tempStr);
    tempStr = jw.getSimpleTag("reportType",reportTypeStr,false);
    jw.addFinalTagToElement("Metadata", tempStr);

    return jw.getJsonElement("Metadata");
}

bool MDReport::writeStrToFile(const std::string strFileCnst,
        const std::string serial, const std::string version,
        const std::string outStr) {
    int gentime = generationTime;
    int gentime00 = (generationTime - gentime) * 100;

    std::string dirnameStr = strFileCnst + serial + "/MDReports_" + curDate;
    const char* dirnameConst = dirnameStr.c_str();

    struct stat info;
    if (stat(dirnameConst, &info) != 0) {
        mkdir(dirnameConst, 0777);
    } else if (info.st_mode & S_IFDIR) {
    } else {
        mkdir(dirnameConst, 0777);
    }

    std::string strFile = strFileCnst + serial + "/MDReports_" + curDate
            + "/MDReport_" + version + "_" + std::to_string(gentime) + "-"
            + std::to_string(gentime00) + "_" + std::to_string(senderId) + "_"
            + std::to_string(reportedId) + ".rep";

    std::fstream checkFile(strFile);

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

