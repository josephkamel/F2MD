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

#include <veins/modules/application/misbehaviorDetection/mdReport/EvidenceReport.h>

EvidenceReport::EvidenceReport(MDReport baseReport) {
    generationTime = baseReport.getGenerationTime();
    senderId = baseReport.getSenderId();
    reportedId = baseReport.getReportedId();
    mbType = baseReport.getMbType();
    bsmListNum = 0;
}

void EvidenceReport::setReportedCheck(BsmCheck reportedCheck) {
    this->reportedCheck = reportedCheck;
}

void EvidenceReport::addBsmToList(BasicSafetyMessage bsm) {
    this->bsmList[bsmListNum] = bsm;
    bsmListNum++;
}

void EvidenceReport::addEvidence(BasicSafetyMessage myBsm,
        BsmCheck reportedCheck, BasicSafetyMessage receivedBsm,
        NodeTable detectedNodes) {
    int eviNum = 0;
    bool myBsmAdded = false;
    setReportedCheck(reportedCheck);

    if (reportedCheck.getRangePlausibility() < 1) {
        addBsmToList(myBsm);
        myBsmAdded=true;
        if (eviNum < 1) {
            eviNum = 1;
        }
    }

    if (reportedCheck.getPositionPlausibility() < 1) {
        if (eviNum < 1) {
            eviNum = 1;
        }
    }

    if (reportedCheck.getSpeedPlausibility() < 1) {
        if (eviNum < 1) {
            eviNum = 1;
        }
    }

    if (reportedCheck.getPositionConsistancy() < 1) {
        if (eviNum < 2) {
            eviNum = 2;
        }
    }

    if (reportedCheck.getSpeedConsistancy() < 1) {
        if (eviNum < 2) {
            eviNum = 2;
        }
    }

    if (reportedCheck.getPositionSpeedConsistancy() < 1) {
        if (eviNum < 2) {
            eviNum = 2;
        }
    }

    if (reportedCheck.getPositionHeadingConsistancy() < 1) {
        if (eviNum < 2) {
            eviNum = 2;
        }
    }

    if (reportedCheck.getSuddenAppearence() < 1) {
        if (eviNum < 1) {
            eviNum = 1;
        }
    }

    if (reportedCheck.getBeaconFrequency() < 1) {
        if (eviNum < 2) {
            eviNum = 2;
        }
    }
    if (reportedCheck.getIntersection().getInterNum() > 0) {
        if (eviNum < 1) {
            eviNum = 1;
        }
    }

    if (eviNum > 0) {
        addBsmToList(receivedBsm);
    }

    for (int var = 0; var < eviNum - 1; ++var) {
        addBsmToList(detectedNodes.getNodeHistory(reportedId).getBSM(var));
    }

    for (int var = 0; var < reportedCheck.getIntersection().getInterNum();
            ++var) {
        if (reportedCheck.getIntersection().getInterValue(var) < 1) {

            if (senderId == reportedCheck.getIntersection().getInterId(var)) {
                if(!myBsmAdded){
                    addBsmToList(myBsm);
                }

            } else {
                addBsmToList(
                        detectedNodes.getNodeHistory(
                                reportedCheck.getIntersection().getInterId(var)).getLatestBSM());
            }
        }
    }
}

std::string EvidenceReport::getReportPrintable() {

    ReportPrintable rp;

    XmlWriter xml;
    xml.init();
    xml.writeHeader();

    std::string tempStr = "Type=\"";
    tempStr = tempStr + "EvidenceReport";
    tempStr = tempStr + "\"";

    xml.writeOpenTagWithAttribute("Report", tempStr);

    xml.writeWholeElement(getBaseReportXml());

    xml.writeWholeElement(rp.getCheckXml(reportedCheck));

    for (int var = 0; var < bsmListNum; ++var) {
        xml.writeWholeElement(rp.getBsmXml(bsmList[var]));
    }

    xml.writeCloseTag();

    return xml.getOutString();
}
