//
// Copyright (C) 2016 David Eckhoff <david.eckhoff@fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef __VEINS_MBReport_H_
#define __VEINS_MBReport_H_

#include <omnetpp.h>
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"

#include "../baseClasses/BsmCheck.h"

using namespace omnetpp;

class MBReport {

    private:
        double generationTime;
        int senderId;
        int reportedId;
        double mbType;
//        std::string failedCheckList[];
//        int failedCheckListNum;
//        BsmCheck reportedBsmList[];
//        int bsmListNum;

    public:
        MBReport();

        void setGenerationTime(double time);
        void setSenderId(int id);
        void setReportedId(int id);
        void setMbType(double type);
//        void addFailedCheck(std::string check);
//        void addBsmCheck(BsmCheck bsmCheck);

        double getGenerationTime();
        int getSenderId();
        int getReportedId();
        double getMbType();

    };

#endif
