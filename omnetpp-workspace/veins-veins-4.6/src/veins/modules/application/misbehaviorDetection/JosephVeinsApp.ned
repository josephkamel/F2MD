///*******************************************************************************
//* @author  Joseph Kamel
//* @date    11/04/2014
//* @version 1.0
//*
//* SCA (Secure Cooperative Autonomous systems)
//* Copyright (c) 2013, 2018 Institut de Recherche Technologique SystemX
//* All rights reserved.
//*******************************************************************************/

package org.car2x.veins.modules.application.misbehaviorDetection;
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;

//
// network description file for your Veins Application. Add parameters here
//
simple JosephVeinsApp extends BaseWaveApplLayer
{
    @class(JosephVeinsApp);
    string appName = default("Joseph Mb Veins App");
    string helloWorld = default("testInput");
}
