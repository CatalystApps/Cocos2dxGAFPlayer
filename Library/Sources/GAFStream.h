#pragma once

#include "TagDefines.h"

NS_GAF_BEGIN

class GAFFile;

class GAFStream
{
private:
    GAFFile*            m_input;
    unsigned char       m_currentByte;
    unsigned char       m_unusedBits;

    struct TagRecord
    {
        unsigned int expectedStreamPos;
        unsigned int tagSize;
        Tags::Enum tagType;
    };

    typedef std::stack<TagRecord> TagStack_t;
    TagStack_t          m_tagStack;

public:
    GAFStream(GAFFile* input);
    ~GAFStream();

    void                 readNBytesOfT(void* dest, unsigned int);
    bool                 readBool();
    unsigned int         readUint(unsigned short bitcount);
    int                  readSint(unsigned short bitcount);
    float                readFixed();
    float                readFixed8();

    float                readFloat();

    unsigned char        readUByte();
    char                 readSByte();

    unsigned char        readU8() { return readUByte(); }
    char                 readS8() { return readSByte(); }
    unsigned short       readU16();
    unsigned int         readU32();
    int                  readS32();

    void                 readString(std::string* out);

    GAFFile*             getInput() const;

    Tags::Enum           openTag();
    void                 closeTag();
    unsigned int         getTagLenghtOnStackTop() const;
    unsigned int         getTagExpectedPosition() const;
    unsigned int         getPosition() const;

    void                 align();

    bool                 isEndOfStream() const;
};

NS_GAF_END