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

#ifndef __VEINS_AttackTypes_H_
#define __VEINS_AttackTypes_H_

#include <iostream>

namespace attackTypes {

#define RandomPosX 3900
#define RandomPosY 1700

#define RandomPosOffsetX 70
#define RandomPosOffsetY 70

#define RandomSpeedX 40
#define RandomSpeedY 40

#define RandomSpeedOffsetX 7
#define RandomSpeedOffsetY 7

#define StopProb 0.01

#define StaleMessages_Buffer 5 //less than 10

#define DosMultipleFreq 4 // times faster

#define SybilVehNumber 5 // max 10
#define SelfSybil false // max 10
#define SybilDistanceX 5 // meters
#define SybilDistanceY 2 // meters

enum Attacks {
    Genuine = 0,
    ConstPos,
    ConstPosOffset,
    RandomPos,
    RandomPosOffset,
    ConstSpeed,
    ConstSpeedOffset,
    RandomSpeed,
    RandomSpeedOffset,
    EventualStop,
    Disruptive,
    DataReplay,
    StaleMessages,
    DoS,
    DoSRandom,
    DoSDisruptive,
    Sybil,
    MAStress,
    SIZE_OF_ENUM
};

static const char* AttackNames[] = { "Genuine", "ConstPos", "ConstPosOffset",
        "RandomPos", "RandomPosOffset", "ConstSpeed", "ConstSpeedOffset",
        "RandomSpeed", "RandomSpeedOffset", "EventualStop", "Disruptive",
        "DataReplay", "StaleMessages", "DoS", "DoSRandom", "DoSDisruptive",
        "Sybil","MAStress" };

static_assert(sizeof(attackTypes::AttackNames)/sizeof(char*) == attackTypes::SIZE_OF_ENUM
        , "sizes dont match");

static const attackTypes::Attacks intAttacks[] = { Genuine, ConstPos,
        ConstPosOffset, RandomPos, RandomPosOffset, ConstSpeed,
        ConstSpeedOffset, RandomSpeed, RandomSpeedOffset, EventualStop,
        Disruptive, DataReplay, StaleMessages, DoS, DoSRandom, DoSDisruptive,
        Sybil, MAStress };

static_assert(sizeof(attackTypes::intAttacks)/sizeof(attackTypes::Attacks) == attackTypes::SIZE_OF_ENUM
        , "sizes dont match");

}

#endif
