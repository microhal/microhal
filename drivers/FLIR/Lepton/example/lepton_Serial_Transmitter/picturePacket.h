/*
 * picturePacket.h
 *
 *  Created on: 13 lip 2016
 *      Author: pawel
 */

#ifndef PICTUREPACKET_H_
#define PICTUREPACKET_H_

#include "hostCommPacket.h"

class Picture {
 public:
	uint32_t pictureNumber;
    uint16_t data[80 * 60];

    size_t width() const { return 80; }

    size_t hight() const { return 60; }
} MICROHAL_PACKED;
static_assert(sizeof(Picture) == 80*60*2+4,"Wrong size");

class PicturePacket : public microhal::HostCommDataPacket<Picture, 0xC4> {
 public:
    PicturePacket() : HostCommDataPacket<Picture, 0xC4>() {}
    virtual ~PicturePacket();
};

#endif /* PICTUREPACKET_H_ */
