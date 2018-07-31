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

#ifndef __VEINS_PseudoChangeTypes_H_
#define __VEINS_PseudoChangeTypes_H_

#include <iostream>

namespace pseudoChangeTypes {

#define Tolerance_Buffer 10 // nbr of messages
#define Period_Change_Time 1 //seconds
#define Period_Change_Distance 1000 //meters
#define Random_Change_Chance 0.01 // 0.01 over 1 = 1% chance

enum PseudoChange {
    NoChange = 0,
    Periodical,
    Disposable,
    DistanceBased,
    Random,
    SIZE_OF_ENUM
};

static const char* AttackNames[] = { "NoChange", "Periodical", "Disposable",
        "DistanceBased", "Random" };

static_assert(sizeof(attackTypes::AttackNames)/sizeof(char*) == attackTypes::SIZE_OF_ENUM
        , "sizes dont match");
}

#endif
