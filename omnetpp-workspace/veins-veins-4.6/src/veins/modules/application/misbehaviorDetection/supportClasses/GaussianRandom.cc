/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#include "GaussianRandom.h"

GaussianRandom::GaussianRandom(double confidenceRadius, Coord curSpeedConfidence) {
    this->confidenceRadius = confidenceRadius;
    this->curSpeedConfidence = curSpeedConfidence;
}

double GaussianRandom::getRand(double mean, double stddev) {
    std::random_device rd{};
       std::mt19937 gen{rd()};

       std::normal_distribution<> d{mean,stddev};
    return d(gen);
}

Coord GaussianRandom::OffsetPosition(Coord curPosition) {

    double r =  getRand(0, confidenceRadius/3);
    double theta = ((double) rand() / RAND_MAX) * 2 * PI;

//    if(r>confidenceRadius ){
//        r = confidenceRadius;
//    }
//    if(r<-confidenceRadius){
//        r = -confidenceRadius;
//    }

    double deltaX = r * cos(theta);
    double deltaY = r * sin(theta);
//    std::cout << "========== r:"<<r<<" theta:"<< theta << '\n';
//    std::cout<< "deltaX:" << deltaX  << '\n';
//    std::cout<< "deltaY:" << deltaY  << '\n';

    return Coord(curPosition.x + deltaX, curPosition.y + deltaY, 0);

}

Coord GaussianRandom::OffsetSpeed(Coord curSpeed) {

    double deltaVx =  getRand(0, curSpeedConfidence.x/3);
    double deltaVy =  getRand(0, curSpeedConfidence.y/3);
    double deltaVz =  getRand(0, curSpeedConfidence.z/3);

//    if(deltaVx>curSpeedConfidence.x ){
//        deltaVx = curSpeedConfidence.x;
//    }
//    if(deltaVx<-curSpeedConfidence.x ){
//        deltaVx = -curSpeedConfidence.x;
//    }
//    if(deltaVy>curSpeedConfidence.y ){
//        deltaVy = curSpeedConfidence.y;
//    }
//    if(deltaVy<-curSpeedConfidence.y ){
//        deltaVy = -curSpeedConfidence.y;
//    }
//    if(deltaVz>curSpeedConfidence.z ){
//        deltaVz = curSpeedConfidence.z;
//    }
//    if(deltaVz<-curSpeedConfidence.z ){
//        deltaVz = -curSpeedConfidence.z;
//    }

    deltaVy = deltaVx;

//    std::cout << "========== speed: "<<curSpeed.x<<" "<< curSpeed.y << '\n';
//    std::cout<< "deltaVx:" << deltaVx  << '\n';
//    std::cout<< "deltaVy:" << deltaVy  << '\n';

    return Coord(curSpeed.x + deltaVx, curSpeed.y + deltaVy, 0);

}




