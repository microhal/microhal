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

#include "gps.h"
#include <charconv>
#include <cmath>
#include <system_error>
#include "diagnostic/diagnostic.h"

using namespace microhal;
using namespace diagnostic;
using namespace std::literals;

namespace microhal {

void GPS::update(string_view messages) {
    size_t msgBegin = 0;
    while (1) {
        auto msgEnd = messages.find('$', 1);
        string_view message = messages.substr(msgBegin, msgEnd);
        msgBegin = msgEnd + 1;
        string_view messageType = message.substr(2, 3);
        if (messageType == "RMC"sv) {
            if (auto [status, rmc] = decodeRMC(message); status == 1) {
                time = rmc.time;
                date = rmc.date;
                latitude = rmc.latitude;
                longitude = rmc.longitude;
            }
        } else if (messageType == "VTG"sv) {
            if (auto [status, vtg] = decodeVTG(message); status == 1) {
            }
        } else if (messageType == "GGA"sv) {
            if (auto [status, gga] = decodeGGA(message); status == 1) {
                latitude = gga.latitude;
                longitude = gga.longitude;
                altitude = gga.altitude;
                hdop = gga.hdop;
            }
        } else if (messageType == "GSA"sv) {
            if (auto [status, gsa] = decodeGSA(message); status == 1) {
                hdop = gsa.hdop;
                pdop = gsa.pdop;
                vdop = gsa.vdop;
            }
        } else if (messageType == "GLL"sv) {
            if (auto [status, gll] = decodeGLL(message); status == 1) {
                latitude = gll.latitude;
                longitude = gll.longitude;
                time = gll.time;
            }
        } else {
            diagChannel << lock << MICROHAL_DEBUG << "Unsupported GPS NMEA message: " << message.substr(0, 6) << endl << unlock;
        }
    }
}

GPS::Result<GPS::RMC> GPS::decodeRMC(string_view message) {
    RMC rmc;
    if (isChecksumValid(message)) {
        message = removeChecksum(message);
        // split message into strings using ',' as separator
        string_view strings[13];
        size_t begin = 0;
        for (auto& string : strings) {
            auto end = message.find(',', begin);
            string = message.substr(begin, end - begin);
            begin = end + 1;
            if (end == std::string_view::npos) break;
        }

        if (strings[0] == "$GNRMC"sv || strings[0] == "$GPRMC"sv) {
            rmc.time = decodeTime(strings[1]);
            rmc.dateValid = strings[2] == "A"sv;
            rmc.latitude = decodeLatitude(strings[3], strings[4]);
            rmc.longitude = decodeLongitude(strings[5], strings[6]);
            rmc.groundSpeedKnots = stringToFloat(strings[7]);
            rmc.courseOverGroundDegree = stringToFloat(strings[8]);
            rmc.date = decodeDate(strings[9]);
            rmc.magneticVariation = decodeMagneticVariation(strings[10], strings[11]);
            rmc.positioningMode = decodePositioningMode(strings[12]);
            return {1, rmc};
        }
    }
    return {0, rmc};
}

GPS::Result<GPS::VTG> GPS::decodeVTG(string_view message) {
    VTG vtg;
    if (isChecksumValid(message)) {
        message = removeChecksum(message);
        // split message into strings using ',' as separator
        string_view strings[10];
        size_t begin = 0;
        for (auto& string : strings) {
            auto end = message.find(',', begin);
            string = message.substr(begin, end - begin);
            begin = end + 1;
            if (end == std::string_view::npos) break;
        }

        if (strings[0] == "$GNVTG"sv) {
            vtg.courseOverGroundDegree = stringToFloat(strings[1]);
            vtg.courseOverGroundMagnetic = stringToFloat(strings[3]);
            vtg.groundSpeedKnots = stringToFloat(strings[5]);
            vtg.groundSpeedKm = stringToFloat(strings[7]);
            vtg.positioningMode = decodePositioningMode(strings[9]);
            return {1, vtg};
        }
    }
    return {0, vtg};
}

GPS::Result<GPS::GGA> GPS::decodeGGA(string_view message) {
    GGA gga;
    if (isChecksumValid(message)) {
        message = removeChecksum(message);
        // split message into strings using ',' as separator
        string_view strings[13];
        size_t begin = 0;
        for (auto& string : strings) {
            auto end = message.find(',', begin);
            string = message.substr(begin, end - begin);
            begin = end + 1;
            if (end == std::string_view::npos) break;
        }

        if (strings[0] == "$GNGGA"sv || strings[0] == "$GPGGA"sv) {
            gga.time = decodeTime(strings[1]);
            gga.latitude = decodeLatitude(strings[2], strings[3]);
            gga.longitude = decodeLongitude(strings[4], strings[5]);
            gga.fixStatus = decodeFixStatus(strings[6]);
            std::from_chars(strings[7].data(), strings[7].data() + strings[7].size(), gga.numberOfSatelitesUsed, 10);
            gga.hdop = stringToFloat(strings[8]);
            gga.altitude = stringToFloat(strings[9]);
            gga.geoidSeparation = stringToFloat(strings[11]);
            gga.dgpsAge = NAN;
            gga.dgpsStationId = NAN;
            return {1, gga};
        }
    }
    return {0, gga};
}

GPS::Result<GPS::GSA> GPS::decodeGSA(string_view message) {
    GSA gsa;
    if (isChecksumValid(message)) {
        message = removeChecksum(message);
        // split message into strings using ',' as separator
        string_view strings[19];
        size_t begin = 0;
        for (auto& string : strings) {
            auto end = message.find(',', begin);
            string = message.substr(begin, end - begin);
            begin = end + 1;
            if (end == std::string_view::npos) break;
        }

        if (strings[0] == "$GNGSA"sv || strings[0] == "$GPGSA"sv) {
            gsa.mode = strings[1] == "M"sv ? FixMode::Manulal : FixMode::Automatically;
            gsa.fixStatus = decode2DOr3DFixStatus(strings[2]);
            for (size_t i = 0; i < 12; i++) {
                if (strings[3 + i].empty()) {
                    gsa.satelliteUsedOnChannel[i] = 0;
                } else {
                    std::from_chars(strings[3 + i].data(), strings[3 + i].data() + strings[3 + i].size(), gsa.satelliteUsedOnChannel[i], 10);
                }
            }
            gsa.pdop = stringToFloat(strings[15]);
            gsa.hdop = stringToFloat(strings[16]);
            gsa.vdop = stringToFloat(strings[17]);
            gsa.gnssSystemId = decodeGNSSSystemId(strings[18]);
            return {1, gsa};
        }
    }
    return {0, gsa};
}

GPS::Result<GPS::GSV> GPS::decodeGSV(string_view message) {
    GSV gsv;
    return {0, gsv};
}

GPS::Result<GPS::GLL> GPS::decodeGLL(string_view message) {
    GLL gll;
    if (isChecksumValid(message)) {
        message = removeChecksum(message);
        // split message into strings using ',' as separator
        string_view strings[8];
        size_t begin = 0;
        for (auto& string : strings) {
            auto end = message.find(',', begin);
            string = message.substr(begin, end - begin);
            begin = end + 1;
            if (end == std::string_view::npos) break;
        }

        if (strings[0] == "$GNGLL" || strings[0] == "$GPGLL") {
            gll.latitude = decodeLatitude(strings[1], strings[2]);
            gll.longitude = decodeLongitude(strings[3], strings[4]);
            gll.time = decodeTime(strings[5]);
            gll.dataValid = strings[6] == "A"sv ? true : false;
            gll.positioningMode = decodePositioningMode(strings[7]);
            return {1, gll};
        }
    }
    return {0, gll};
}

GPS::Result<GPS::TXT> GPS::decodeTXT(string_view message) {
    TXT txt;
    if (isChecksumValid(message)) {
        message = removeChecksum(message);
        // split message into strings using ',' as separator
        string_view strings[5];
        size_t begin = 0;
        for (auto& string : strings) {
            auto end = message.find(',', begin);
            string = message.substr(begin, end - begin);
            begin = end + 1;
            if (end == std::string_view::npos) break;
        }

        if (strings[0] == "$GPTXT"sv) {
            std::from_chars(strings[1].data(), strings[1].data() + strings[1].size(), txt.totalNumberOfMessages, 10);
            std::from_chars(strings[2].data(), strings[2].data() + strings[2].size(), txt.messageNumber, 10);
            txt.messageType = decodeMessageType(strings[3]);
            txt.messageText = strings[4];
            return {1, txt};
        }
    }
    return {0, txt};
}

/**
 *
 * @param timestr string_view containing time in format hhmmss.sss
 * @return Time structure
 */
GPS::Time GPS::decodeTime(string_view timestr) {
    string_view hour = timestr.substr(0, 2);
    string_view minutes = timestr.substr(2, 2);
    string_view seconds = timestr.substr(4, 2);
    string_view milliseconds = timestr.substr(7, 3);
    Time time;
    std::from_chars(hour.data(), hour.data() + hour.size(), time.hour, 10);
    std::from_chars(minutes.data(), minutes.data() + minutes.size(), time.min, 10);
    std::from_chars(seconds.data(), seconds.data() + seconds.size(), time.sec, 10);
    std::from_chars(milliseconds.data(), milliseconds.data() + milliseconds.size(), time.millisec, 10);

    return time;
}

GPS::Date GPS::decodeDate(string_view datestr) {
    string_view dd = datestr.substr(0, 2);
    string_view mm = datestr.substr(2, 2);
    string_view yy = datestr.substr(4, 2);
    Date date;
    std::from_chars(dd.data(), dd.data() + dd.size(), date.dd, 10);
    std::from_chars(mm.data(), mm.data() + mm.size(), date.mm, 10);
    std::from_chars(yy.data(), yy.data() + yy.size(), date.yyyy, 10);
    date.yyyy += 2000;
    return date;
}
/**
 *
 * @param latitudeString string view in format ddmm.mmmm
 * @param northOrSouth strinv_view containing N or S
 * @return latitude as float number or NaN if an error occurred
 */
float GPS::decodeLatitude(string_view latitudeString, string_view northOrSouth) {
    if (latitudeString.length() >= 2) {
        string_view degreesStr = latitudeString.substr(0, 2);
        string_view minutesStr = latitudeString.substr(2);

        uint_fast8_t latitudeDegree;
        if (auto [ptr, ec] = std::from_chars(degreesStr.data(), degreesStr.data() + degreesStr.size(), latitudeDegree, 10); ec == std::errc()) {
            float latitude = (stringToFloat(minutesStr) / 60.0f) + latitudeDegree;
            if (northOrSouth.compare("S") == 0) latitude *= -1.0f;
            return latitude;
        }
    }
    return NAN;
}
/**
 *
 * @param longitudeString string_view in format dddmm.mmmm
 * @param eastOrWest string_view containing E or W
 * @return longitude as float or NaN if an error occurred
 */
float GPS::decodeLongitude(string_view longitudeString, string_view eastOrWest) {
    if (longitudeString.length() >= 3) {
        string_view longitudeDegreesStr = longitudeString.substr(0, 3);
        string_view longitudeMinutesStr = longitudeString.substr(3);

        uint_fast8_t longitudeDegrees;
        if (auto [ptr, ec] =
                std::from_chars(longitudeDegreesStr.data(), longitudeDegreesStr.data() + longitudeDegreesStr.size(), longitudeDegrees, 10);
            ec == std::errc()) {
            float longitude = (stringToFloat(longitudeMinutesStr) / 60.0f) + longitudeDegrees;
            if (eastOrWest.compare("W") == 0) longitude *= -1.0f;
            return longitude;
        }
    }
    return NAN;
}

float GPS::decodeMagneticVariation(string_view magneticVariation, string_view eastOrWest) {
    return NAN;  // todo implement
}

GPS::PositioningMode GPS::decodePositioningMode(string_view positioningMode) {
    if (positioningMode == "N"sv) return PositioningMode::NoFix;
    if (positioningMode == "A"sv) return PositioningMode::AutonomousGNSSFix;
    if (positioningMode == "D"sv) return PositioningMode::DifferentialGNSSfix;
    return PositioningMode::Error;
}

GPS::FixStatus GPS::decodeFixStatus(string_view fixStatus) {
    if (fixStatus == "0"sv) return FixStatus::Invalid;
    if (fixStatus == "1"sv) return FixStatus::GNSS;
    if (fixStatus == "2"sv) return FixStatus::DGPS;
    if (fixStatus == "6"sv) return FixStatus::Estimated;
    return FixStatus::Error;
}

GPS::FixStatus GPS::decode2DOr3DFixStatus(string_view fixStatus) {
    if (fixStatus == "1"sv) return FixStatus::NoFix;
    if (fixStatus == "2"sv) return FixStatus::Fix2D;
    if (fixStatus == "3"sv) return FixStatus::Fix3D;
    return FixStatus::Error;
}

GPS::GNSSSystemID GPS::decodeGNSSSystemId(string_view gnssSystemId) {
    if (gnssSystemId == "1"sv) return GNSSSystemID::GPS;
    if (gnssSystemId == "2"sv) return GNSSSystemID::GLONASS;
    if (gnssSystemId == "3"sv) return GNSSSystemID::Galileo;
    if (gnssSystemId == "4"sv) return GNSSSystemID::BeiDou;
    return GNSSSystemID::Unknown;
}
GPS::MessageType GPS::decodeMessageType(string_view messageType) {
    if (messageType == "00"sv) return MessageType::Error;
    if (messageType == "01"sv) return MessageType::Warning;
    if (messageType == "02"sv) return MessageType::Notice;
    if (messageType == "07"sv) return MessageType::User;
    return MessageType::Unknown;
}

uint_fast8_t GPS::checksum(string_view message) {
    uint_fast8_t checksum = 0;
    for (auto a : message) {
        checksum ^= a;
    }
    return checksum;
}

bool GPS::isChecksumValid(string_view message) {
    auto checksumBegin = message.find('*');
    if (checksumBegin + 2 <= message.size()) {
        string_view checksumStr = message.substr(checksumBegin + 1, 2);
        string_view checksumData = message.substr(1, checksumBegin - 1);

        uint8_t expectedChecksum;
        if (!checksumStr.empty()) {
            if (auto [ptr, ec] = std::from_chars(checksumStr.data(), checksumStr.data() + checksumStr.size(), expectedChecksum, 16);
                ec == std::errc()) {
                auto calculatedChecksum = checksum(checksumData);
                return calculatedChecksum == expectedChecksum;
            }
        }
    }
    return false;
}

GPS::string_view GPS::removeChecksum(string_view message) {
    auto checksumBegin = message.find('*');
    return message.substr(0, checksumBegin);
}

float GPS::stringToFloat(string_view string) {
    auto dotPos = string.find('.');
    if (dotPos != string_view::npos) {
        uint32_t dec, frac;
        if (auto [ptr, ec] = std::from_chars(string.data(), string.data() + dotPos, dec, 10); ec != std::errc()) {
            diagChannel << lock << MICROHAL_DEBUG << "string decode error: " << string << endl << unlock;
            return NAN;
        }
        if (auto [ptr, ec] = std::from_chars(string.data() + dotPos + 1, string.data() + string.size(), frac, 10); ec != std::errc()) {
            diagChannel << lock << MICROHAL_DEBUG << "string decode error: " << string << endl << unlock;
            return NAN;
        }
        static uint32_t divider[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
        return (static_cast<float>(frac) / divider[string.size() - dotPos - 1]) + dec;
    } else {
        uint32_t i;
        if (auto [ptr, ec] = std::from_chars(string.data(), string.data() + string.size(), i, 10); ec != std::errc()) {
            diagChannel << lock << MICROHAL_DEBUG << "string decode error: " << string << endl << unlock;
            return NAN;
        }
        return i;
    }
}

}  // namespace microhal
