#pragma once

NS_GAF_BEGIN

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
    // Common
    Compression     compression;
    unsigned short  version;
    unsigned int    fileLenght;

    // Version 3
    unsigned short  framesCount;
    cocos2d::Rect   frameSize;
    cocos2d::Vect   pivot;
	
    // Version 4
    std::vector<float>          scaleValues;
    std::vector<float>          csfValues;

    inline uint16_t getMajorVersion() const
    {
        return version & 0x00FF;
    }

    inline uint16_t getMinorVersion() const
    {
        return version >> 1;
    }
};

NS_GAF_END