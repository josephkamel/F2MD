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

#ifndef __VEINS_ReportTypes_H_
#define __VEINS_ReportTypes_H_

#include <iostream>

namespace reportTypes {

enum Report {
    OneMessageReport = 0,
    EvidenceReport,
    BasicCheckReport,
    SIZE_OF_ENUM
};

static const char* AttackNames[] = { "OneMessageReport", "EvidenceReport", "BasicCheckReport"};

static_assert(sizeof(attackTypes::AttackNames)/sizeof(char*) == attackTypes::SIZE_OF_ENUM
        , "sizes dont match");
}

#endif
