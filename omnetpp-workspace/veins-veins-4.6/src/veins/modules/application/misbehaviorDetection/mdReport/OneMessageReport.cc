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

#include <veins/modules/application/misbehaviorDetection/mdReport/OneMessageReport.h>

OneMessageReport::OneMessageReport(MDReport baseReport) {
    generationTime = baseReport.getGenerationTime();
    senderId = baseReport.getSenderId();
    reportedId = baseReport.getReportedId();
    mbType = baseReport.getMbType();
}
void OneMessageReport::setReportedCheck(BsmCheck reportedCheck) {
    this->reportedCheck = reportedCheck;
}

void OneMessageReport::setReportedBsm(BasicSafetyMessage reportedBsm) {
    this->reportedBsm = reportedBsm;
}

std::string OneMessageReport::getReportPrintable() {

    ReportPrintable rp;

    XmlWriter xml;
    xml.init();
    xml.writeHeader();

    std::string tempStr = "Type=\"";
    tempStr = tempStr + "OneMessageReport";
    tempStr = tempStr + "\"";
    xml.writeOpenTagWithAttribute("Report", tempStr);

    xml.writeWholeElement(getBaseReportXml());
    xml.writeWholeElement(rp.getCheckXml(reportedCheck));

    xml.writeWholeElement(rp.getBsmXml(reportedBsm));

    xml.writeCloseTag();

    return xml.getOutString();
}


