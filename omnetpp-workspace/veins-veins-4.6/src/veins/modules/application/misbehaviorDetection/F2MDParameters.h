/*******************************************************************************
 * @author  Joseph Kamel
 * @email   josephekamel@gmail.com
 * @date    28/11/2018
 * @version 2.0
 *
 * SCA (Secure Cooperative Autonomous systems)
 * Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
 * All rights reserved.
 *******************************************************************************/

#ifndef __VEINS_MDParameters_H_
#define __VEINS_MDParameters_H_
#include "enumTypes/AttackTypes.h"
#include "enumTypes/PseudoChangeTypes.h"
#include "enumTypes/ReportTypes.h"
#include "enumTypes/MbTypes.h"
#include "enumTypes/MdAppTypes.h"





//Detection Parameters
#define MAX_CONFIDENCE_RANGE 10 //3
#define MAX_PLAUSIBLE_RANGE 417 // 415
#define MIN_INT_DIST 0.9 // 3.292091
#define MIN_PSS -0.712046 //   -0.712046 for TB || -4.07843 for LUST
#define MAX_PSS 8.37598 // 8.37598 for TB || 6.08384 for LUST
#define MAX_TIME_DELTA 1.1
#define MAX_DELTA_INTER 0.4 //1.001
#define MAX_SA_RANGE 210 // 420/2
#define MAX_CONSISTANT_DISTANCE 25 // 25 for TB || 47.22 for LUST
#define MAX_PLAUSIBLE_SPEED 25 // 25 for TB || 47.22 for LUST
#define MAX_PLAUSIBLE_ACCEL 3.0 // 3.0
#define MAX_PLAUSIBLE_DECEL 4.501 // 4.5
#define MAX_BEACON_FREQUENCY 0.99 //0.99998
#define MAX_DISTANCE_FROM_ROUTE 2 //4.5
#define MAX_NON_ROUTE_SPEED -1 //3
#define MAX_HEADING_CHANGE 90 //90
#define DELTA_BSM_TIME 5 // application
#define DELTA_REPORT_TIME 5 // history report
#define POS_HEADING_TIME 1.1
//Detection Parameters


//Storage Parameters
#define MAX_BSM_LENGTH 20
#define MAX_MDM_LENGTH 20
#define MAX_NODES_LENGTH 100
#define MAX_TARGET_LENGTH 1000
#define MAX_TARGET_TIME 2
#define MAX_ACCUSED_LENGTH 1000
#define MAX_ACCUSED_TIME 2
//Storage Parameters

//Attacks Parameters
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
#define MAX_SYBIL_NUM 10
//Attacks Parameters

//pseudonym Parameters
#define Period_Change_Time 240 //seconds
#define Tolerance_Buffer 10 // nbr of messages
#define Period_Change_Distance 80 //meters
#define Random_Change_Chance 0.1 // 0.01 over 1 = 1% chance
//pseudonym Parameters



#endif
