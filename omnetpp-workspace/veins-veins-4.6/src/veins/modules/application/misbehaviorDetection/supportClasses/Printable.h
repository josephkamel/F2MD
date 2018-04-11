#ifndef __VEINS_Printable_H_
#define __VEINS_Printable_H_

#include <stdio.h>
#include <stdlib.h>     /* atof */
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
using namespace boost;

class Printable {

private:
    char name[32];

    double flagsRangePlausibility_1 = 0;
    double flagsPositionPlausibility_1 = 0;
    double flagsSpeedPlausibility_1 = 0;
    double flagsPositionConsistancy_1 = 0;
    double flagsPositionSpeedConsistancy_1 = 0;
    double flagsSpeedConsistancy_1 = 0;
    double flagsBeaconFrequency_1 = 0;
    double flagsIntersection_1 = 0;
    double flagsSuddenAppearence_1 = 0;

    double flagsRangePlausibility_2 = 0;
    double flagsPositionPlausibility_2 = 0;
    double flagsSpeedPlausibility_2 = 0;
    double flagsPositionConsistancy_2 = 0;
    double flagsPositionSpeedConsistancy_2 = 0;
    double flagsSpeedConsistancy_2 = 0;
    double flagsBeaconFrequency_2 = 0;
    double flagsIntersection_2 = 0;
    double flagsSuddenAppearence_2 = 0;

    double cumulFlags_1 = 0;
    double cumulFlags_2 = 0;

    double allTests_1 = 0;
    double allTests_2 = 0;

public:
    Printable(const char *);

    void incAll(double mbType);
    void incCumulFlags(double mbType);
    void incFlags(std::string inString, double mbType);
    void resetAll();
    void getPrintable(char* outStr, double density,double deltaT);

    void writeFile(std::string path, char* printStr, bool init);
};

#endif
