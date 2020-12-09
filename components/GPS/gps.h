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

#ifndef _MICROHAL_GPS_H_
#define _MICROHAL_GPS_H_

#include <array>
#include <cstdint>
#include <string_view>

namespace microhal {

class GPS {
 public:
    using string_view = std::string_view;
    enum class PositioningMode : uint8_t { NoFix, AutonomousGNSSFix, DifferentialGNSSfix, Error };
    enum class FixStatus : uint8_t { Invalid, GNSS, DGPS, Estimated, NoFix, Fix2D, Fix3D, Error };
    enum class FixMode : uint8_t { Manulal, Automatically };
    enum class GNSSSystemID : uint8_t { GPS, GLONASS, Galileo, BeiDou, Unknown };
    enum class MessageType : uint8_t { Error, Warning, Notice, User, Unknown };
    struct Time {
        uint8_t hour;
        uint8_t min;
        uint8_t sec;
        uint16_t millisec;
    };

    struct Date {
        uint8_t dd;
        uint8_t mm;
        uint16_t yyyy;
    };

    struct RMC {
        float latitude;
        float longitude;
        float groundSpeedKnots;
        float courseOverGroundDegree;
        float magneticVariation;
        Time time;
        Date date;
        PositioningMode positioningMode;
        bool dateValid;
    };

    struct VTG {
        float courseOverGroundDegree;
        float courseOverGroundMagnetic;
        float groundSpeedKnots;
        float groundSpeedKm;
        PositioningMode positioningMode;
    };

    struct GGA {
        float latitude;
        float longitude;
        float hdop;
        float altitude;
        float geoidSeparation;
        float dgpsAge;
        float dgpsStationId;
        Time time;
        FixStatus fixStatus;
        uint8_t numberOfSatelitesUsed;
    };

    struct GSA {
        float pdop;
        float hdop;
        float vdop;
        FixMode mode;
        FixStatus fixStatus;
        std::array<uint8_t, 12> satelliteUsedOnChannel;  //!< sateliteUsedOnChannel[0] represent number of satellites used on channel 1
        GNSSSystemID gnssSystemId;
    };
    struct GSV {};

    struct GLL {
        float latitude;
        float longitude;
        Time time;
        bool dataValid;
        PositioningMode positioningMode;
    };

    struct TXT {
        string_view messageText;        // non allocating string, it is user responsibility to be sure that pointer to data is valid
        uint8_t totalNumberOfMessages;  // from 1 to 99
        uint8_t messageNumber;          // from 1 to 99
        MessageType messageType;
    };

    template <typename T>
    struct Result {
        uint8_t status;
        T data;
    };

    Result<RMC> decodeRMC(string_view message);
    Result<VTG> decodeVTG(string_view message);
    Result<GGA> decodeGGA(string_view message);
    Result<GSA> decodeGSA(string_view message);
    Result<GSV> decodeGSV(string_view message);
    Result<GLL> decodeGLL(string_view message);
    Result<TXT> decodeTXT(string_view message);

    void update(string_view messages);

    float getLatitude() const { return latitude; }
    float getLongitude() const { return longitude; }
    float getAltitude() const { return altitude; }
    FixStatus getFixStatus() const { return fixStatus; }

 protected:
    float latitude;
    float longitude;
    float altitude;
    float pdop;
    float hdop;
    float vdop;
    Time time;
    Date date;
    FixStatus fixStatus;

 private:
    Time decodeTime(string_view timestr);
    Date decodeDate(string_view datestr);
    float decodeLatitude(string_view, string_view);
    float decodeLongitude(string_view longitudeString, string_view eastOrWest);
    float decodeMagneticVariation(string_view magneticVariation, string_view eastOrWest);
    PositioningMode decodePositioningMode(string_view positioningMode);
    FixStatus decodeFixStatus(string_view fixStatus);
    FixStatus decode2DOr3DFixStatus(string_view fixStatus);
    GNSSSystemID decodeGNSSSystemId(string_view gnssSystemId);
    MessageType decodeMessageType(string_view messageType);
    uint_fast8_t checksum(string_view message);
    bool isChecksumValid(string_view message);

    string_view removeChecksum(string_view);

    float stringToFloat(string_view);
};  // namespace microhal

}  // namespace microhal

#endif /* _MICROHAL_GPS_H_ */
