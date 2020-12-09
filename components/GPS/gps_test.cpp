/**
 * @license    BSD 3-Clause
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 30-04-2019
 *
 * @copyright Copyright (c) 2019, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cmath>
#include "bsp.h"
#include "doctest.h"
#include "gps.h"

using namespace microhal;
using namespace std::literals;

TEST_CASE("GPS GNRMC check") {
    GPS gps;
    static auto message = "$GNRMC,110332.000,A,3150.7822,N,11711.9278,E,0.01,37.48,111214,,,D*43\n\r"sv;
    auto [status, rmc] = gps.decodeRMC(message);
    CHECK(status);
    CHECK(rmc.time.hour == 11);
    CHECK(rmc.time.min == 03);
    CHECK(rmc.time.sec == 32);
    CHECK(rmc.time.millisec == 0);
    CHECK(rmc.dateValid == true);
    // CHECK(rmc.latitude == );
    // CHECK(rmc.longitude)
    CHECK(rmc.groundSpeedKnots == 0.01f);
    CHECK(rmc.courseOverGroundDegree == 37.48f);
    CHECK(rmc.date.dd == 11);
    CHECK(rmc.date.mm == 12);
    CHECK(rmc.date.yyyy == 2014);
    CHECK(std::isnan(rmc.magneticVariation));
    CHECK(rmc.positioningMode == GPS::PositioningMode::DifferentialGNSSfix);
}

TEST_CASE("GPS GNRMC for NMEAV4.10 check") {
    GPS gps;
    static auto message = "$GNRMC,072432.000,A,3150.7790,N,11711.9289,E,0.00,0.00,130717,,,A,V*0A\n\r"sv;
    auto [status, rmc] = gps.decodeRMC(message);
    CHECK(status);
    CHECK(rmc.time.hour == 07);
    CHECK(rmc.time.min == 24);
    CHECK(rmc.time.sec == 32);
    CHECK(rmc.time.millisec == 0);
    CHECK(rmc.dateValid == true);
    // CHECK(rmc.latitude == );
    // CHECK(rmc.longitude)
    CHECK(rmc.groundSpeedKnots == 0.00f);
    CHECK(rmc.courseOverGroundDegree == 00.00f);
    CHECK(rmc.date.dd == 13);
    CHECK(rmc.date.mm == 07);
    CHECK(rmc.date.yyyy == 2017);
    CHECK(std::isnan(rmc.magneticVariation));
    CHECK(rmc.positioningMode == GPS::PositioningMode::AutonomousGNSSFix);
}

TEST_CASE("GPS GNVTG check") {
    GPS gps;
    static auto message = "$GNVTG,19.11,T,,M,0.16,N,0.30,K,A*1F\n\r"sv;
    auto [status, vtg] = gps.decodeVTG(message);
    CHECK(vtg.courseOverGroundDegree == 19.11f);
    CHECK(std::isnan(vtg.courseOverGroundMagnetic));
    CHECK(vtg.groundSpeedKnots == 0.16f);
    CHECK(vtg.groundSpeedKm == 0.3f);
    CHECK(vtg.positioningMode == GPS::PositioningMode::AutonomousGNSSFix);
}

TEST_CASE("GPS GNGGA check") {
    GPS gps;
    static auto message = "$GNGGA,083354.000,3150.7790,N,11711.9289,E,1,8,2.85,53.2,M,0.0,M,,*4B"sv;
    auto [status, gga] = gps.decodeGGA(message);
    CHECK(status);
    CHECK(gga.time.hour == 8);
    CHECK(gga.time.min == 33);
    CHECK(gga.time.sec == 54);
    CHECK(gga.time.millisec == 0);
    // CHECK(rmc.latitude == );
    // CHECK(rmc.longitude)
    CHECK(gga.fixStatus == GPS::FixStatus::GNSS);
    CHECK(gga.numberOfSatelitesUsed == 8);
    CHECK(gga.hdop == 2.85f);
    CHECK(gga.altitude == 53.2f);
    CHECK(gga.geoidSeparation == 0.0);
    CHECK(std::isnan(gga.dgpsAge));
    CHECK(std::isnan(gga.dgpsStationId));
}

TEST_CASE("GPS GNGSA check") {
    GPS gps;
    static auto message = "$GNGSA,A,3,22,14,12,15,31,25,24,04,193,,,,1.28,0.75,1.03*29"sv;

    auto [status, gsa] = gps.decodeGSA(message);
    CHECK(status);
    CHECK(gsa.mode == GPS::FixMode::Automatically);
    CHECK(gsa.fixStatus == GPS::FixStatus::Fix3D);
    CHECK(gsa.satelliteUsedOnChannel[0] == 22);
    CHECK(gsa.satelliteUsedOnChannel[1] == 14);
    CHECK(gsa.satelliteUsedOnChannel[2] == 12);
    CHECK(gsa.satelliteUsedOnChannel[3] == 15);
    CHECK(gsa.satelliteUsedOnChannel[4] == 31);
    CHECK(gsa.satelliteUsedOnChannel[5] == 25);
    CHECK(gsa.satelliteUsedOnChannel[6] == 24);
    CHECK(gsa.satelliteUsedOnChannel[7] == 4);
    CHECK(gsa.satelliteUsedOnChannel[8] == 193);
    CHECK(gsa.satelliteUsedOnChannel[9] == 0);
    CHECK(gsa.satelliteUsedOnChannel[10] == 0);
    CHECK(gsa.satelliteUsedOnChannel[11] == 0);
    CHECK(gsa.pdop == 1.28f);
    CHECK(gsa.hdop == 0.75f);
    CHECK(gsa.vdop == 1.03f);
    CHECK(gsa.gnssSystemId == GPS::GNSSSystemID::Unknown);
}

TEST_CASE("GPS GLL check") {
    GPS gps;
    static auto message = "$GNGLL,3150.7822,N,11711.9278,E,110332.000,A,D*4B\n\r"sv;

    auto [status, gll] = gps.decodeGLL(message);
    CHECK(status);
    // CHECK(gll.latitude ==);
    // CHECK(gll.longitude ==)
    CHECK(gll.time.hour == 11);
    CHECK(gll.time.min == 3);
    CHECK(gll.time.sec == 32);
    CHECK(gll.time.millisec == 0);
    CHECK(gll.dataValid);
    CHECK(gll.positioningMode == GPS::PositioningMode::DifferentialGNSSfix);
}

TEST_CASE("GPS TXT check") {
    GPS gps;
    static auto message = "$GPTXT,01,01,02,ANTSTATUS=OK*3B\n\r"sv;

    auto [status, txt] = gps.decodeTXT(message);
    CHECK(status);
    CHECK(txt.totalNumberOfMessages == 1);
    CHECK(txt.messageNumber == 1);
    CHECK(txt.messageType == GPS::MessageType::Notice);
    CHECK(txt.messageText == "ANTSTATUS=OK");
}
