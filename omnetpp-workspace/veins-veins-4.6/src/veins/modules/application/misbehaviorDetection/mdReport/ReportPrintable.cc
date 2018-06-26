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

#include "ReportPrintable.h"

std::string ReportPrintable::getCheckXml(BsmCheck Check){

    std::string tempStr = "";

    XmlWriter xml;
    xml.init();
    xml.writeOpenTag("BsmCheck");

     xml.writeStartElementTag("rP");
     xml.writeString(std::to_string(Check.getRangePlausibility()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("pP");
     xml.writeString(std::to_string(Check.getPositionPlausibility()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("sP");
     xml.writeString(std::to_string(Check.getSpeedPlausibility()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("pC");
     xml.writeString(std::to_string(Check.getPositionConsistancy()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("sC");
     xml.writeString(std::to_string(Check.getSpeedConsistancy()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("psC");
     xml.writeString(std::to_string(Check.getPositionSpeedConsistancy()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("phC");
     xml.writeString(std::to_string(Check.getPositionHeadingConsistancy()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("sA");
     xml.writeString(std::to_string(Check.getSuddenAppearence()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("bF");
     xml.writeString(std::to_string(Check.getBeaconFrequency()));
     xml.writeEndElementTag();

     tempStr = "n=\"";
     tempStr = tempStr + std::to_string(Check.getIntersection().getInterNum());
     tempStr = tempStr + "\"";

     xml.writeOpenTagWithAttribute("inT",tempStr);
     for (int var = 0; var < Check.getIntersection().getInterNum(); ++var) {
         tempStr = "id=\"";
         tempStr = tempStr + std::to_string(Check.getIntersection().getInterId(var));
         tempStr = tempStr + "\"";
         xml.writeStartElementTag("veh");
         xml.writeAttribute(tempStr);
         xml.writeString(std::to_string(Check.getIntersection().getInterValue(var)));
         xml.writeEndElementTag();
    }
     xml.writeCloseTag();
     xml.writeCloseTag();

     return xml.getOutString();

}

std::string ReportPrintable::getBsmXml(BasicSafetyMessage bsm){

    std::string tempStr = "";

    XmlWriter xml;
    xml.init();

    tempStr = "id=\"";
    tempStr = tempStr + std::to_string(bsm.getSenderAddress());
    tempStr = tempStr + "\"";

    tempStr = tempStr + "ts=\"";
    tempStr = tempStr + bsm.getArrivalTime().str();
    tempStr = tempStr + "\"";

    xml.writeOpenTagWithAttribute("Bsm", tempStr);

     xml.writeStartElementTag("Pos");
     tempStr = "(";
     tempStr = tempStr + std::to_string(bsm.getSenderPos().x);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderPos().y);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderPos().z);
     tempStr = tempStr + ")";
     xml.writeString(tempStr);
     xml.writeEndElementTag();

     xml.writeStartElementTag("PosConfidence");
     tempStr = "(";
     tempStr = tempStr + std::to_string(bsm.getSenderPosConfidence().x);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderPosConfidence().y);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderPosConfidence().z);
     tempStr = tempStr + ")";
     xml.writeString(tempStr);
     xml.writeEndElementTag();

     xml.writeStartElementTag("Speed");
     tempStr = "(";
     tempStr = tempStr + std::to_string(bsm.getSenderSpeed().x);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderSpeed().y);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderSpeed().z);
     tempStr = tempStr + ")";
     xml.writeString(tempStr);
     xml.writeEndElementTag();

     xml.writeStartElementTag("SpeedConfidence");
     tempStr = "(";
     tempStr = tempStr + std::to_string(bsm.getSenderSpeedConfidence().x);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderSpeedConfidence().y);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderSpeedConfidence().z);
     tempStr = tempStr + ")";
     xml.writeString(tempStr);
     xml.writeEndElementTag();

     xml.writeStartElementTag("Heading");
     tempStr = "(";
     tempStr = tempStr + std::to_string(bsm.getSenderHeading().x);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderHeading().y);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderHeading().z);
     tempStr = tempStr + ")";
     xml.writeString(tempStr);
     xml.writeEndElementTag();

     xml.writeStartElementTag("HeadingConfidence");
     tempStr = "(";
     tempStr = tempStr + std::to_string(bsm.getSenderHeadingConfidence().x);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderHeadingConfidence().y);
     tempStr = tempStr +",";
     tempStr = tempStr + std::to_string(bsm.getSenderHeadingConfidence().z);
     tempStr = tempStr + ")";
     xml.writeString(tempStr);
     xml.writeEndElementTag();

     xml.writeStartElementTag("Width");
     xml.writeString(std::to_string(bsm.getSenderWidth()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("Length");
     xml.writeString(std::to_string(bsm.getSenderLength()));
     xml.writeEndElementTag();

     xml.writeStartElementTag("MbType");
     xml.writeString(bsm.getSenderMbTypeStr());
     xml.writeEndElementTag();

     xml.writeCloseTag();

     return xml.getOutString();

}


