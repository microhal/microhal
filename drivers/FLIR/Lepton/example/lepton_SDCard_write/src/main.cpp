/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief      FLIR Lepton IR camera example main file
 *
 * @authors    Pawel Okas
 * created on: 2014
 * last modification: <DD-MM-YYYY>
 *
 * @copyright Copyright (c) 2016, Pawel Okas
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

#include <cstring>

#include "bsp.h"
#include "diagnostic/diagnostic.h"
#include "fatFsDisks.h"  // for FatFs::addDisk
#include "microhal.h"
#include "sd.h"          // for Sd
#include "sdCardDisk.h"  // for SDCardDisk
#include "utils/packed.h"

#include "lepton.h"

using namespace microhal;
using namespace microhal::diagnostic;
using namespace std::literals::chrono_literals;

class BMP {
    using LONG = int;
    using WORD = uint16_t;
    using DWORD = uint32_t;

    struct BITMAPFILEHEADER {
        WORD bfType;
        DWORD bfSize;
        WORD bfReserved1;
        WORD bfReserved2;
        DWORD bfOffBits;
    } MICROHAL_PACKED;

    struct BITMAPINFOHEADER {
        DWORD biSize;
        LONG biWidth;
        LONG biHeight;
        WORD biPlanes;
        WORD biBitCount;
        DWORD biCompression;
        DWORD biSizeImage;
        LONG biXPelsPerMeter;
        LONG biYPelsPerMeter;
        DWORD biClrUsed;
        DWORD biClrImportant;
    };

    static_assert(sizeof(BITMAPFILEHEADER) == 14, "");
    static_assert(sizeof(BITMAPINFOHEADER) == 40, "");

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER fileInfo;

    void prepareBITMAPFILEHEADER(size_t fileSize) {
        union {
            WORD bfTYPE;
            uint8_t type[2] = {'B', 'M'};
        };

        fileHeader.bfType = bfTYPE;
        fileHeader.bfSize = fileSize;
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;
        fileHeader.bfOffBits = 54;
    }

    void prepareBITMAPINFOHEADER(size_t width, size_t height) {
        fileInfo.biSize = sizeof(fileInfo);
        fileInfo.biWidth = width;
        fileInfo.biHeight = height;
        fileInfo.biPlanes = 1;
        fileInfo.biBitCount = 24;
        fileInfo.biCompression = 0;
        fileInfo.biSizeImage = 0;
        fileInfo.biXPelsPerMeter = 0;
        fileInfo.biYPelsPerMeter = 0;
        fileInfo.biClrUsed = 0;
        fileInfo.biClrImportant = 0;
    }

    void alocatePictureData(size_t width, size_t height) {
        // bmp line have to be divided by 4, so lets check if padding is required
        size_t lineWidth = width * sizeof(Pixel);
        size_t padding = lineWidth % 4;
        size_t requiredSize = (lineWidth + padding) * height;
    }

 public:
    struct Pixel {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };
    static_assert(sizeof(Pixel) == 3, "");

    struct Picture {
        const size_t width = 0;
        const size_t height = 0;
        Pixel *data = nullptr;
        bool allocated = false;

        constexpr static size_t calculatePadding(size_t width) {
            const size_t lineWidth = width * sizeof(Pixel);
            const size_t padding = lineWidth % 4;
            return padding;
        }

        Picture(size_t width, size_t height) : width(width), height(height), allocated(true) {
            data = static_cast<Pixel *>(malloc(width * height * sizeof(Pixel)));
#if __cpp_exceptions
            if (data == nullptr) {
                std::bad_alloc exception;
                throw exception;
            }
#else
            if (data == nullptr) std::terminate();
#endif
        }
        constexpr Picture(size_t width, size_t height, void *data) noexcept : width(width), height(height), data(static_cast<Pixel *>(data)) {}
        Picture(const Picture &) = delete;

        ~Picture() {
            if (allocated) free(data);
        }

        Pixel &pixel(size_t x, size_t y) {
            const size_t pixelPos = y * width + x;
            return data[pixelPos];
        }

        const Pixel &pixel(size_t x, size_t y) const {
            const size_t pixelPos = y * width + x;
            return data[pixelPos];
        }

        Pixel *getLinePtr(size_t y) { return &data[y * width]; }

        const Pixel *getLinePtr(size_t y) const { return &data[y * width]; }
    };

    bool save(const char *filename, const Picture &picture) {
        int filesize = 54 + 3 * picture.width * picture.height;
        prepareBITMAPFILEHEADER(filesize);
        prepareBITMAPINFOHEADER(picture.width, picture.height);

        if (FILE *f = fopen(filename, "wb")) {
            fwrite(&fileHeader, 1, 14, f);
            fwrite(&fileInfo, 1, 40, f);
            uint8_t padding[] = {0, 0, 0};
            for (size_t i = 0; i < picture.height; i++) {
                auto size = fwrite(picture.getLinePtr(i), 1, sizeof(Pixel) * picture.width, f);
                if (size != sizeof(Pixel) * picture.width) {
                    diagChannel << lock << MICROHAL_EMERGENCY << "Unable to write file, written: " << uint32_t(size)
                                << ", requested: " << uint32_t(sizeof(Pixel) * picture.width) << unlock;
                }
                if (fwrite(padding, 1, picture.calculatePadding(picture.width), f) != picture.calculatePadding(picture.width)) {
                    diagChannel << lock << MICROHAL_EMERGENCY << "Unable to write file." << unlock;
                }
            }
            fclose(f);
            return true;
        }
        return false;
    }

    bool save(const char *filename, size_t width, size_t height, uint8_t *pixels) {
        Picture picture(width, height);

        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                picture.pixel(x, y) = Pixel{pixels[y * width + x], 0, 0};
            }
        }

        return save(filename, picture);
    }
};

class Picture {
 public:
    uint16_t data[80 * 60];

    constexpr size_t width() const { return 80; }

    constexpr size_t hight() const { return 60; }
};  // MICROHAL_PACKED;
// static_assert(sizeof(Picture) == 80 * 60 * 2, "Wrong size");

void saveColoredImage(const Picture &picture, uint32_t pictureNumber) {
    // Rainbow:
    // const int colormap[] = {1, 3, 74, 0, 3, 74, 0, 3, 75, 0, 3, 75, 0, 3, 76, 0, 3, 76, 0, 3, 77, 0, 3, 79, 0, 3, 82, 0, 5, 85, 0, 7, 88, 0, 10,
    // 91, 0, 14, 94, 0, 19, 98, 0, 22, 100, 0, 25, 103, 0, 28, 106, 0, 32, 109, 0, 35, 112, 0, 38, 116, 0, 40, 119, 0, 42, 123, 0, 45, 128, 0, 49,
    // 133, 0, 50, 134, 0, 51, 136, 0, 52, 137, 0, 53, 139, 0, 54, 142, 0, 55, 144, 0, 56, 145, 0, 58, 149, 0, 61, 154, 0, 63, 156, 0, 65, 159, 0, 66,
    // 161, 0, 68, 164, 0, 69, 167, 0, 71, 170, 0, 73, 174, 0, 75, 179, 0, 76, 181, 0, 78, 184, 0, 79, 187, 0, 80, 188, 0, 81, 190, 0, 84, 194, 0, 87,
    // 198, 0, 88, 200, 0, 90, 203, 0, 92, 205, 0, 94, 207, 0, 94, 208, 0, 95, 209, 0, 96, 210, 0, 97, 211, 0, 99, 214, 0, 102, 217, 0, 103, 218, 0,
    // 104, 219, 0, 105, 220, 0, 107, 221, 0, 109, 223, 0, 111, 223, 0, 113, 223, 0, 115, 222, 0, 117, 221, 0, 118, 220, 1, 120, 219, 1, 122, 217, 2,
    // 124, 216, 2, 126, 214, 3, 129, 212, 3, 131, 207, 4, 132, 205, 4, 133, 202, 4, 134, 197, 5, 136, 192, 6, 138, 185, 7, 141, 178, 8, 142, 172, 10,
    // 144, 166, 10, 144, 162, 11, 145, 158, 12, 146, 153, 13, 147, 149, 15, 149, 140, 17, 151, 132, 22, 153, 120, 25, 154, 115, 28, 156, 109, 34,
    // 158, 101, 40, 160, 94, 45, 162, 86, 51, 164, 79, 59, 167, 69, 67, 171, 60, 72, 173, 54, 78, 175, 48, 83, 177, 43, 89, 179, 39, 93, 181, 35, 98,
    // 183, 31, 105, 185, 26, 109, 187, 23, 113, 188, 21, 118, 189, 19, 123, 191, 17, 128, 193, 14, 134, 195, 12, 138, 196, 10, 142, 197, 8, 146, 198,
    // 6, 151, 200, 5, 155, 201, 4, 160, 203, 3, 164, 204, 2, 169, 205, 2, 173, 206, 1, 175, 207, 1, 178, 207, 1, 184, 208, 0, 190, 210, 0, 193, 211,
    // 0, 196, 212, 0, 199, 212, 0, 202, 213, 1, 207, 214, 2, 212, 215, 3, 215, 214, 3, 218, 214, 3, 220, 213, 3, 222, 213, 4, 224, 212, 4, 225, 212,
    // 5, 226, 212, 5, 229, 211, 5, 232, 211, 6, 232, 211, 6, 233, 211, 6, 234, 210, 6, 235, 210, 7, 236, 209, 7, 237, 208, 8, 239, 206, 8, 241, 204,
    // 9, 242, 203, 9, 244, 202, 10, 244, 201, 10, 245, 200, 10, 245, 199, 11, 246, 198, 11, 247, 197, 12, 248, 194, 13, 249, 191, 14, 250, 189, 14,
    // 251, 187, 15, 251, 185, 16, 252, 183, 17, 252, 178, 18, 253, 174, 19, 253, 171, 19, 254, 168, 20, 254, 165, 21, 254, 164, 21, 255, 163, 22,
    // 255, 161, 22, 255, 159, 23, 255, 157, 23, 255, 155, 24, 255, 149, 25, 255, 143, 27, 255, 139, 28, 255, 135, 30, 255, 131, 31, 255, 127, 32,
    // 255, 118, 34, 255, 110, 36, 255, 104, 37, 255, 101, 38, 255, 99, 39, 255, 93, 40, 255, 88, 42, 254, 82, 43, 254, 77, 45, 254, 69, 47, 254, 62,
    // 49, 253, 57, 50, 253, 53, 52, 252, 49, 53, 252, 45, 55, 251, 39, 57, 251, 33, 59, 251, 32, 60, 251, 31, 60, 251, 30, 61, 251, 29, 61, 251, 28,
    // 62, 250, 27, 63, 250, 27, 65, 249, 26, 66, 249, 26, 68, 248, 25, 70, 248, 24, 73, 247, 24, 75, 247, 25, 77, 247, 25, 79, 247, 26, 81, 247, 32,
    // 83, 247, 35, 85, 247, 38, 86, 247, 42, 88, 247, 46, 90, 247, 50, 92, 248, 55, 94, 248, 59, 96, 248, 64, 98, 248, 72, 101, 249, 81, 104, 249,
    // 87, 106, 250, 93, 108, 250, 95, 109, 250, 98, 110, 250, 100, 111, 251, 101, 112, 251, 102, 113, 251, 109, 117, 252, 116, 121, 252, 121, 123,
    // 253, 126, 126, 253, 130, 128, 254, 135, 131, 254, 139, 133, 254, 144, 136, 254, 151, 140, 255, 158, 144, 255, 163, 146, 255, 168, 149, 255,
    // 173, 152, 255, 176, 153, 255, 178, 155, 255, 184, 160, 255, 191, 165, 255, 195, 168, 255, 199, 172, 255, 203, 175, 255, 207, 179, 255, 211,
    // 182, 255, 216, 185, 255, 218, 190, 255, 220, 196, 255, 222, 200, 255, 225, 202, 255, 227, 204, 255, 230, 206, 255, 233, 208};
    static const uint8_t colormap[] = {
        255, 255, 255, 253, 253, 253, 251, 251, 251, 249, 249, 249, 247, 247, 247, 245, 245, 245, 243, 243, 243, 241, 241, 241, 239, 239, 239, 237,
        237, 237, 235, 235, 235, 233, 233, 233, 231, 231, 231, 229, 229, 229, 227, 227, 227, 225, 225, 225, 223, 223, 223, 221, 221, 221, 219, 219,
        219, 217, 217, 217, 215, 215, 215, 213, 213, 213, 211, 211, 211, 209, 209, 209, 207, 207, 207, 205, 205, 205, 203, 203, 203, 201, 201, 201,
        199, 199, 199, 197, 197, 197, 195, 195, 195, 193, 193, 193, 191, 191, 191, 189, 189, 189, 187, 187, 187, 185, 185, 185, 183, 183, 183, 181,
        181, 181, 179, 179, 179, 177, 177, 177, 175, 175, 175, 173, 173, 173, 171, 171, 171, 169, 169, 169, 167, 167, 167, 165, 165, 165, 163, 163,
        163, 161, 161, 161, 159, 159, 159, 157, 157, 157, 155, 155, 155, 153, 153, 153, 151, 151, 151, 149, 149, 149, 147, 147, 147, 145, 145, 145,
        143, 143, 143, 141, 141, 141, 139, 139, 139, 137, 137, 137, 135, 135, 135, 133, 133, 133, 131, 131, 131, 129, 129, 129, 126, 126, 126, 124,
        124, 124, 122, 122, 122, 120, 120, 120, 118, 118, 118, 116, 116, 116, 114, 114, 114, 112, 112, 112, 110, 110, 110, 108, 108, 108, 106, 106,
        106, 104, 104, 104, 102, 102, 102, 100, 100, 100, 98,  98,  98,  96,  96,  96,  94,  94,  94,  92,  92,  92,  90,  90,  90,  88,  88,  88,
        86,  86,  86,  84,  84,  84,  82,  82,  82,  80,  80,  80,  78,  78,  78,  76,  76,  76,  74,  74,  74,  72,  72,  72,  70,  70,  70,  68,
        68,  68,  66,  66,  66,  64,  64,  64,  62,  62,  62,  60,  60,  60,  58,  58,  58,  56,  56,  56,  54,  54,  54,  52,  52,  52,  50,  50,
        50,  48,  48,  48,  46,  46,  46,  44,  44,  44,  42,  42,  42,  40,  40,  40,  38,  38,  38,  36,  36,  36,  34,  34,  34,  32,  32,  32,
        30,  30,  30,  28,  28,  28,  26,  26,  26,  24,  24,  24,  22,  22,  22,  20,  20,  20,  18,  18,  18,  16,  16,  16,  14,  14,  14,  12,
        12,  12,  10,  10,  10,  8,   8,   8,   6,   6,   6,   4,   4,   4,   2,   2,   2,   0,   0,   0,   0,   0,   9,   2,   0,   16,  4,   0,
        24,  6,   0,   31,  8,   0,   38,  10,  0,   45,  12,  0,   53,  14,  0,   60,  17,  0,   67,  19,  0,   74,  21,  0,   82,  23,  0,   89,
        25,  0,   96,  27,  0,   103, 29,  0,   111, 31,  0,   118, 36,  0,   120, 41,  0,   121, 46,  0,   122, 51,  0,   123, 56,  0,   124, 61,
        0,   125, 66,  0,   126, 71,  0,   127, 76,  1,   128, 81,  1,   129, 86,  1,   130, 91,  1,   131, 96,  1,   132, 101, 1,   133, 106, 1,
        134, 111, 1,   135, 116, 1,   136, 121, 1,   136, 125, 2,   137, 130, 2,   137, 135, 3,   137, 139, 3,   138, 144, 3,   138, 149, 4,   138,
        153, 4,   139, 158, 5,   139, 163, 5,   139, 167, 5,   140, 172, 6,   140, 177, 6,   140, 181, 7,   141, 186, 7,   141, 189, 10,  137, 191,
        13,  132, 194, 16,  127, 196, 19,  121, 198, 22,  116, 200, 25,  111, 203, 28,  106, 205, 31,  101, 207, 34,  95,  209, 37,  90,  212, 40,
        85,  214, 43,  80,  216, 46,  75,  218, 49,  69,  221, 52,  64,  223, 55,  59,  224, 57,  49,  225, 60,  47,  226, 64,  44,  227, 67,  42,
        228, 71,  39,  229, 74,  37,  230, 78,  34,  231, 81,  32,  231, 85,  29,  232, 88,  27,  233, 92,  24,  234, 95,  22,  235, 99,  19,  236,
        102, 17,  237, 106, 14,  238, 109, 12,  239, 112, 12,  240, 116, 12,  240, 119, 12,  241, 123, 12,  241, 127, 12,  242, 130, 12,  242, 134,
        12,  243, 138, 12,  243, 141, 13,  244, 145, 13,  244, 149, 13,  245, 152, 13,  245, 156, 13,  246, 160, 13,  246, 163, 13,  247, 167, 13,
        247, 171, 13,  248, 175, 14,  248, 178, 15,  249, 182, 16,  249, 185, 18,  250, 189, 19,  250, 192, 20,  251, 196, 21,  251, 199, 22,  252,
        203, 23,  252, 206, 24,  253, 210, 25,  253, 213, 27,  254, 217, 28,  254, 220, 29,  255, 224, 30,  255, 227, 39,  255, 229, 53,  255, 231,
        67,  255, 233, 81,  255, 234, 95,  255, 236, 109, 255, 238, 123, 255, 240, 137, 255, 242, 151, 255, 244, 165, 255, 246, 179, 255, 248, 193,
        255, 249, 207, 255, 251, 221, 255, 253, 235, 255, 255, 24};

    const auto [min, max] = std::minmax_element(std::begin(picture.data), std::end(picture.data));
    int diff = *max - *min + 1;

    BMP::Picture colourImage(picture.width(), picture.hight());
    //	BMP::Picture grayImage(picture.width(), picture.hight());

    for (size_t y = 0; y < picture.hight(); ++y) {
        for (size_t x = 0; x < picture.width(); ++x) {
            auto baseValue = picture.data[picture.width() * y + x];  // take input value in [0, 65536)
            int scaledValue = 256 * (baseValue - *min) / diff;       // map value to interval [0, 256), and set the pixel to its color value above
            colourImage.pixel(x, y) = BMP::Pixel{colormap[3 * scaledValue + 2], colormap[3 * scaledValue + 1], colormap[3 * scaledValue]};
            //	grayImage.pixel(x, y) = BMP::Pixel {baseValue>>8, baseValue>>8, baseValue>>8};
        }
    }

    static char pictureName[100];
    BMP pic;
    sprintf(pictureName, "colour%d.bmp", pictureNumber);
    pic.save(pictureName, colourImage);
    //	sprintf(pictureName, "gray%d.bmp", pictureNumber);
    //	pic.save(pictureName, grayImage);
}

Sd sdCard(bsp::sdCardSpi, bsp::sdCardCs);
FatFs::SDCardDisk sdDisk(sdCard);

int main(void) {
    bsp::debugPort.write("\n\r------------------- FLIR Lepton SD Card picture capture -------------------------\n\r");

    diagChannel.setOutputDevice(bsp::debugPort);
    // lets check if diagChannal is working
    diagChannel << lock << MICROHAL_EMERGENCY << "Information from diagnostic channel." << unlock;

    FatFs::addDisk(sdDisk, "0");

    static Lepton lepton(bsp::leptonSPI, bsp::leptonI2C, bsp::leptonCS, bsp::leptonPower, bsp::leptonReset);
    lepton.startup();

    uint32_t pictureNumber = 0;
    uint32_t pictureFailureCounter = 0;
    uint8_t iter = 0;
    while (1) {
        auto beginTime = std::chrono::system_clock::now();
        while (1) {
            if (lepton.isNewPictureAvailable()) {
                pictureNumber++;
                static Picture picture;
                memcpy(picture.data, lepton.getPicture(), lepton.getPictureSize());
                diagChannel << lock << MICROHAL_INFORMATIONAL << "Saving picture " << unlock;
                saveColoredImage(picture, pictureNumber);
                break;
            }
            if (beginTime + 150ms < std::chrono::system_clock::now()) {
                pictureFailureCounter++;
                diagChannel << lock << MICROHAL_INFORMATIONAL << "Timeout while receiving Lepton picture. Failures: " << pictureFailureCounter
                            << unlock;
                break;
            }
            lepton.timeProc();
            std::this_thread::sleep_for(1ms);
        }
        std::this_thread::sleep_until(beginTime + 500ms);
        if (iter++ >= 40) break;
    }

    FatFs::removeDisk(sdDisk, "0");
    diagChannel << lock << MICROHAL_INFORMATIONAL << "Fat device unmounted." << unlock;
    while (1)
        ;
    return 0;
}
