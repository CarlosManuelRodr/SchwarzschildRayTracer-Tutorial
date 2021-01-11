/** 
* @file BmpWriter.h 
* @brief A simple BMP writer.
*
* @author Carlos Manuel Rodriguez y Martinez
*
* @date 01/05/2020
*/

#pragma once
#ifndef __bmpWriter
#define __bmpWriter

#include <fstream>
#include <cstdint>

/***************************
*                          *
*   Operaciones binarias   *
*                          *
***************************/

char* ToByte(void* ptr)
{
    return static_cast<char*>(ptr);
}

uint16_t SwitchEndianess16(uint16_t data)
{
    uint16_t temp;
    uint16_t out = 0x0000;
    uint16_t pos = 0x00FF;

    temp = data & pos;
    temp <<= 8;
    out |= temp;
    pos <<= 8;

    temp = data & pos;
    temp >>= 8;
    out |= temp;

    return out;
}

uint32_t SwitchEndianess32(uint32_t data)
{
    uint32_t temp;
    uint32_t out = 0x00000000;
    uint32_t pos = 0x000000FF;

    temp = data & pos;
    temp <<= 24;
    out |= temp;
    pos <<= 8;
    temp = data & pos;
    temp <<= 8;
    out |= temp;
    pos <<= 8;
    temp = data & pos;
    temp >>= 8;
    out |= temp;
    pos <<= 8;
    temp = data & pos;
    temp >>= 24;
    out |= temp;

    return out;
}

// Conversion operations.
void BoolToChar(char& target, bool in)
{
    if(in)
        target = 0x01;
    else
        target = 0x00;
}

void CharToBool(bool& target, char in)
{
    if(in == 0x00)
        target = false;
    else
        target = true;
}

void UInt32ToChar(char* target, uint32_t in)
{
    in = SwitchEndianess32(in);
    uint32_t temp = 0x000000FF;
    for(unsigned int i=0; i<sizeof(uint32_t); i++)
    {
        target[i] = in & temp;
        in >>= 8;
    }
}

void Int32ToChar(char* target, int32_t in)
{
    in = SwitchEndianess32(in);
    int32_t temp = 0x000000FF;
    for(unsigned int i=0; i<sizeof(int32_t); i++)
    {
        target[i] = in & temp;
        in >>= 8;
    }
}

void CharToUInt32(uint32_t& target, char* in)
{
    target = 0x00000000;
    uint32_t temp;
    for(unsigned int i=0; i<sizeof(uint32_t); i++)
    {
        temp = in[i] & 0x000000FF;
        target |= temp;
        if(i != 3)
            target <<= 8;
    }
}

void CharToInt32(int32_t& target, char* in)
{
    target = 0x00000000;
    int32_t temp;
    for(unsigned int i=0; i<sizeof(int32_t); i++)
    {
        temp = in[i] & 0x000000FF;
        target |= temp;
        if(i != 3) target <<= 8;
    }
}

/**
* @struct BMPHeader
* @brief Header of the BMP file.
*/
struct BMPHeader
{
    uint16_t identifier;
    uint32_t size;
    uint16_t appSpecific1;
    uint16_t appSpecific2;
    uint32_t bitmapData;
};

/**
* @struct DIBHeader
* @brief Another header used by the BMP file that contains parameters.
*/
struct DIBHeader
{
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t nPlanes;
    uint16_t colorDepth;
    uint32_t compression;
    uint32_t bmpBytes;
    int32_t hRes;
    int32_t vRes;
    uint32_t nColors;
    uint32_t nImpColors;
};

/**
* @struct BMPPixel
* @brief A simple rgb pixel.
*/
struct BMPPixel
{
    uint8_t r, g, b;
};

/**
* @class BMPWriter
* @brief Class to write BMP files.
*
* Although SFML has it's own methods to save image files these have memory allocation problems if the
* user tries to save a oversized image (10000x10000 pixels for example). This class it's designed to avoid
* these problems by writing the BMP line by line.
*/
class BMPWriter
{
    BMPHeader* myBmpHdr;
    DIBHeader* myDibHdr;
    std::ofstream file;
    unsigned int imageWidth;
    unsigned int imageHeight;
    unsigned int paddingBytes;
    int dataSize;
    unsigned int indexHeight;

public:
    ///@brief Constructor.
    ///@param filepath Path of file to save.
    ///@param width Width of the image.
    ///@param height Height of the image.
    BMPWriter(const char* filepath, unsigned int width, unsigned int height)
    {
        // Creates header.
        indexHeight = 0;
        imageWidth = width;
        imageHeight = height;
        dataSize = width * height;
        myBmpHdr = new BMPHeader;
        myDibHdr = new DIBHeader;

        myBmpHdr->identifier = SwitchEndianess16(0x424d);    // Windows
        myBmpHdr->appSpecific1 = 0x0000;
        myBmpHdr->appSpecific2 = 0x0000;

        myDibHdr->width = width;
        myDibHdr->height = height;
        myDibHdr->nPlanes = SwitchEndianess16(0x0100);
        myDibHdr->colorDepth = SwitchEndianess16(0x1800);
        myDibHdr->compression = 0x00000000;
        myDibHdr->bmpBytes = SwitchEndianess32(0x10000000);
        myDibHdr->hRes = SwitchEndianess32(0x130B0000);
        myDibHdr->vRes = SwitchEndianess32(0x130B0000);
        myDibHdr->nColors = 0x00000000;
        myDibHdr->nImpColors = 0x00000000;

        unsigned int bmpSize = 0;
        unsigned int offsetData = 54;
        paddingBytes = imageWidth % 4;
        // Calculates file size.
        bmpSize += 14;        // BMPHeader size. sizeof(BMPHeader) it's not safe due to struct padding.
        bmpSize += 40;        // DIBHeader size
        bmpSize += 3 * width * height;
        bmpSize += imageHeight * paddingBytes;
        myBmpHdr->size = bmpSize;
        myBmpHdr->bitmapData = offsetData;
        myDibHdr->headerSize = 40;        // DIBHeader size

        // Writes header.
        file.open(filepath, std::ios::out | std::ios::binary);
        file.write(ToByte(&myBmpHdr->identifier), 2);
        file.write(ToByte(&myBmpHdr->size), 4);
        file.write(ToByte(&myBmpHdr->appSpecific1), 2);
        file.write(ToByte(&myBmpHdr->appSpecific2), 2);
        file.write(ToByte(&myBmpHdr->bitmapData), 4);
        file.write(ToByte(&myDibHdr->headerSize), 4);
        file.write(ToByte(&myDibHdr->width), 4);
        file.write(ToByte(&myDibHdr->height), 4);
        file.write(ToByte(&myDibHdr->nPlanes), 2);
        file.write(ToByte(&myDibHdr->colorDepth), 2);
        file.write(ToByte(&myDibHdr->compression), 4);
        file.write(ToByte(&myDibHdr->bmpBytes), 4);
        file.write(ToByte(&myDibHdr->hRes), 4);
        file.write(ToByte(&myDibHdr->vRes), 4);
        file.write(ToByte(&myDibHdr->nColors), 4);
        file.write(ToByte(&myDibHdr->nImpColors), 4);
    }
    ~BMPWriter()
    {
        delete myBmpHdr;
        delete myDibHdr;
    }

    ///@brief Writes BMP line.
    ///@param data Array of pixels to write.
    void WriteLine(BMPPixel* data)
    {
        // Writes BMP line.
        if(indexHeight < imageHeight)
        {
            for(unsigned int i=0; i<imageWidth; i++)
            {
                file.write(ToByte(&data[i].b), 1);
                file.write(ToByte(&data[i].g), 1);
                file.write(ToByte(&data[i].r), 1);
            }
            if(paddingBytes == 1)
            {
                uint8_t padding = 0x00;
                file.write(ToByte(&padding), 1);
            }
            if(paddingBytes == 2)
            {
                uint16_t padding = 0x0000;
                file.write(ToByte(&padding), 2);
            }
            if(paddingBytes == 3)
            {
                uint32_t padding = 0x00000000;
                file.write(ToByte(&padding), 3);
            }
        }
        indexHeight++;
    }

    void CloseBMP()
    {
        file.close();
    }
};

#endif