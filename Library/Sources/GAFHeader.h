#pragma once

class GAFHeader
{
public:

    enum Compression
    {
        __CompressionDefault = 0, // Internal
        CompressedNone = 0x00474146, // GAF
        CompressedZip = 0x00474143,  // GAC
    };

public:
    Compression     compression;
    unsigned short  version;
    unsigned int    fileLenght;
    unsigned short  framesCount;
    CCRect          frameSize;
    CCPoint         pivot;
};