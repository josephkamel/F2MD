/*******************************************************************************
* @author  Joseph Kamel
* @date    11/04/2014
* @version 1.0
*
* SCA (Secure Cooperative Autonomous systems)
* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
* All rights reserved.
*******************************************************************************/

#ifndef __VEINS_JOSEPHVEINSAPP_H_
#define __VEINS_JOSEPHVEINSAPP_H_

#include <math.h>
#include "MDModule.h"
#include "MDModuleV2.h"
#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

using namespace omnetpp;
using namespace Veins;

#include "veins/modules/obstacle/ObstacleControl.h"
#include "veins/modules/obstacle/Obstacle.h"


#include "supportClasses/GeneralLib.h"
#include "mdAuthority/MDAuthority.h"

/**
 * @brief
 * This is a stub for a typical Veins application layer.
 * Most common functions are overloaded.
 * See MyVeinsApp.cc for hints
 *
 * @author David Eckhoff
 *
 */

class JosephVeinsApp : public BaseWaveApplLayer {
    private:
        NodeTable detectedNodes;
        GeneralLib genLib = GeneralLib();
        MDAuthority mdAuthority = MDAuthority();

    public:
        virtual void initialize(int stage);
        virtual void finish();
    protected:

        virtual void onBSM(BasicSafetyMessage* bsm);
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void onWSA(WaveServiceAdvertisment* wsa);

        virtual void handleSelfMsg(cMessage* msg);
        virtual void handlePositionUpdate(cObject* obj);

        double induceMisbehavior(double faulty, double attackers);

        typedef std::list<Obstacle*> ObstacleGridCell;
        typedef std::vector<ObstacleGridCell> ObstacleGridRow;
        typedef std::vector<ObstacleGridRow> Obstacles;
    };

#endif
