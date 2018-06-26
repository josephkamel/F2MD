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

#include "Printable.h"

Printable::Printable(const char* name) {
    strcpy(this->name, name);

    flagsRangePlausibility_1 = 0;
    flagsPositionPlausibility_1 = 0;
    flagsSpeedPlausibility_1 = 0;
    flagsPositionConsistancy_1 = 0;
    flagsPositionSpeedConsistancy_1 = 0;
    flagsSpeedConsistancy_1 = 0;
    flagsBeaconFrequency_1 = 0;
    flagsIntersection_1 = 0;
    flagsSuddenAppearence_1 = 0;
    flagsSuddenAppearence_1 = 0;
    flagsPositionHeadingConsistancy_1 = 0;


    flagsRangePlausibility_2 = 0;
    flagsPositionPlausibility_2 = 0;
    flagsSpeedPlausibility_2 = 0;
    flagsPositionConsistancy_2 = 0;
    flagsPositionSpeedConsistancy_2 = 0;
    flagsSpeedConsistancy_2 = 0;
    flagsBeaconFrequency_2 = 0;
    flagsIntersection_2 = 0;
    flagsSuddenAppearence_2 = 0;
    flagsPositionHeadingConsistancy_2 = 0;

    cumulFlags_1 = 0;
    cumulFlags_2 = 0;

    allTests_1 = 0;
    allTests_2 = 0;
}

Printable::Printable() {

    flagsRangePlausibility_1 = 0;
    flagsPositionPlausibility_1 = 0;
    flagsSpeedPlausibility_1 = 0;
    flagsPositionConsistancy_1 = 0;
    flagsPositionSpeedConsistancy_1 = 0;
    flagsSpeedConsistancy_1 = 0;
    flagsBeaconFrequency_1 = 0;
    flagsIntersection_1 = 0;
    flagsSuddenAppearence_1 = 0;
    flagsSuddenAppearence_1 = 0;
    flagsPositionHeadingConsistancy_1 = 0;


    flagsRangePlausibility_2 = 0;
    flagsPositionPlausibility_2 = 0;
    flagsSpeedPlausibility_2 = 0;
    flagsPositionConsistancy_2 = 0;
    flagsPositionSpeedConsistancy_2 = 0;
    flagsSpeedConsistancy_2 = 0;
    flagsBeaconFrequency_2 = 0;
    flagsIntersection_2 = 0;
    flagsSuddenAppearence_2 = 0;
    flagsPositionHeadingConsistancy_2 = 0;

    cumulFlags_1 = 0;
    cumulFlags_2 = 0;

    allTests_1 = 0;
    allTests_2 = 0;
}

void Printable::setName(const char* name){
    strcpy(this->name, name);
}

void Printable::incAll(std::string mbType) {

    if (!mbType.compare("genuine")) {
        allTests_1++;

    }
    if (!mbType.compare("attacker")) {
        allTests_2++;
    }

}

void Printable::incCumulFlags(std::string mbType) {
    if (!mbType.compare("genuine")) {
        cumulFlags_1++;
    }
    if (!mbType.compare("attacker")) {
        cumulFlags_2++;
    }
}

void Printable::incFlags(std::string inString, std::string mbType) {
    if (!mbType.compare("genuine")) {
        if (inString == "RangePlausibility")
            flagsRangePlausibility_1++;
        if (inString == "PositionPlausibility")
            flagsPositionPlausibility_1++;
        if (inString == "SpeedPlausibility")
            flagsSpeedPlausibility_1++;
        if (inString == "PositionConsistancy")
            flagsPositionConsistancy_1++;
        if (inString == "PositionSpeedConsistancy")
            flagsPositionSpeedConsistancy_1++;
        if (inString == "SpeedConsistancy")
            flagsSpeedConsistancy_1++;
        if (inString == "BeaconFrequency")
            flagsBeaconFrequency_1++;
        if (inString == "Intersection")
            flagsIntersection_1++;
        if (inString == "SuddenAppearence")
            flagsSuddenAppearence_1++;
        if (inString == "PositionHeadingConsistancy")
            flagsPositionHeadingConsistancy_1++;
    }

    if (!mbType.compare("attacker")) {
        if (inString == "RangePlausibility")
            flagsRangePlausibility_2++;
        if (inString == "PositionPlausibility")
            flagsPositionPlausibility_2++;
        if (inString == "SpeedPlausibility")
            flagsSpeedPlausibility_2++;
        if (inString == "PositionConsistancy")
            flagsPositionConsistancy_2++;
        if (inString == "PositionSpeedConsistancy")
            flagsPositionSpeedConsistancy_2++;
        if (inString == "SpeedConsistancy")
            flagsSpeedConsistancy_2++;
        if (inString == "BeaconFrequency")
            flagsBeaconFrequency_2++;
        if (inString == "Intersection")
            flagsIntersection_2++;
        if (inString == "SuddenAppearence")
            flagsSuddenAppearence_2++;
        if (inString == "PositionHeadingConsistancy")
            flagsPositionHeadingConsistancy_2++;
    }


}

void Printable::resetAll() {
    flagsRangePlausibility_1 = 0;
    flagsPositionPlausibility_1 = 0;
    flagsSpeedPlausibility_1 = 0;
    flagsPositionConsistancy_1 = 0;
    flagsPositionSpeedConsistancy_1 = 0;
    flagsSpeedConsistancy_1 = 0;
    flagsBeaconFrequency_1 = 0;
    flagsIntersection_1 = 0;
    flagsSuddenAppearence_1 = 0;
    flagsPositionHeadingConsistancy_1 = 0;

    flagsRangePlausibility_2 = 0;
    flagsPositionPlausibility_2 = 0;
    flagsSpeedPlausibility_2 = 0;
    flagsPositionConsistancy_2 = 0;
    flagsPositionSpeedConsistancy_2 = 0;
    flagsSpeedConsistancy_2 = 0;
    flagsBeaconFrequency_2 = 0;
    flagsIntersection_2 = 0;
    flagsSuddenAppearence_2 = 0;
    flagsPositionHeadingConsistancy_2 = 0;

    cumulFlags_1 = 0;
    allTests_1 = 0;

    cumulFlags_2 = 0;
    allTests_2 = 0;
}

void Printable::writeFile(std::string path, char* printStr, bool init) {
    ofstream outFile;
    if(init){
        outFile.open(path, std::ofstream::out);
    }else{
        outFile.open(path, std::ofstream::out | std::ofstream::app | std::ofstream::ate);
    }

    outFile.seekp(0,std::ios::end);
    outFile << printStr << "\n";
    outFile.close();
}

void Printable::getPrintable(char* outStr, double density, double deltaT) {
    char line[1024] = "";
    char data[64] = "";

    strcat(line, name);//1
    strcat(line, " ");
    sprintf(data, "%f", deltaT);
    strcat(line, data);//2
    strcat(line, " ");
    sprintf(data, "%f", density);
    strcat(line, data);//3
    strcat(line, " ");
    sprintf(data, "%f", flagsRangePlausibility_1);//4
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsPositionPlausibility_1);//5
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsSpeedPlausibility_1);//6
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsPositionConsistancy_1);//7
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsPositionSpeedConsistancy_1);//8
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsSpeedConsistancy_1);//9
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsBeaconFrequency_1);//10
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsIntersection_1);//11
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsSuddenAppearence_1);//12
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsPositionHeadingConsistancy_1);//13
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", cumulFlags_1);//14
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", allTests_1);//15
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsRangePlausibility_2);//16
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsPositionPlausibility_2);//17
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsSpeedPlausibility_2);//18
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsPositionConsistancy_2);//19
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsPositionSpeedConsistancy_2);//20
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsSpeedConsistancy_2);//21
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsBeaconFrequency_2);//22
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsIntersection_2);//23
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsSuddenAppearence_2);//24
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", flagsPositionHeadingConsistancy_2);//25
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", cumulFlags_2);//26
    strcat(line, data);
    strcat(line, " ");
    sprintf(data, "%f", allTests_2);//27
    strcat(line, data);
    for (int i = 0; i < 1024; ++i) {
        outStr[i] = line[i];
    }

//    std::cout << "+++++++++++ " << name << " +++++++++++" << " Factor_1:"
//            << cumulFlags_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsRangePlausibility_1:"
//            << flagsRangePlausibility_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsPositionPlausibility_1:"
//            << flagsPositionPlausibility_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsSpeedPlausibility_1:"
//            << flagsSpeedPlausibility_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsPositionConsistancy_1:"
//            << flagsPositionConsistancy_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsPositionSpeedConsistancy_1:"
//            << flagsPositionSpeedConsistancy_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsPositionHeadingConsistancy_1:"
//            << flagsPositionHeadingConsistancy_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsSpeedConsistancy_1:"
//            << flagsSpeedConsistancy_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsBeaconFrequency_1:" << flagsBeaconFrequency_1 / allTests_1 * 100
//            << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++" << " flagsIntersection_1:"
//            << flagsIntersection_1 / allTests_1 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++" << " flagsSuddenAppearence_1:"
//            << flagsSuddenAppearence_1 / allTests_1 * 100 << "%" << '\n';


//    std::cout << "+++++++++++ " << name << " +++++++++++" << " Factor_2:"
//            << cumulFlags_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsRangePlausibility_2:"
//            << flagsRangePlausibility_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsPositionPlausibility_2:"
//            << flagsPositionPlausibility_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsSpeedPlausibility_2:"
//            << flagsSpeedPlausibility_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsPositionConsistancy_2:"
//            << flagsPositionConsistancy_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsPositionSpeedConsistancy_2:"
//            << flagsPositionSpeedConsistancy_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsSpeedConsistancy_2:"
//            << flagsSpeedConsistancy_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsPositionHeadingConsistancy_2:"
//            << flagsPositionHeadingConsistancy_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++"
//            << " flagsBeaconFrequency_2:" << flagsBeaconFrequency_2 / allTests_2 * 100
//            << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++" << " flagsIntersection_2:"
//            << flagsIntersection_2 / allTests_2 * 100 << "%" << '\n';
//    std::cout << "++++++++++ " << name << " ++++++++++" << " flagsSuddenAppearence_2:"
//            << flagsSuddenAppearence_2 / allTests_2 * 100 << "%" << '\n';

}

