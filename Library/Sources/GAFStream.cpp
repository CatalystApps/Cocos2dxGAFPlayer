#include "GAFPrecompiled.h"
#include "GAFStream.h"
#include "GAFFile.h"

NS_GAF_BEGIN

GAFStream::GAFStream(GAFFile* input):
m_input(input),
m_currentByte(0),
m_unusedBits(0)
{
    assert(input);
}

GAFStream::~GAFStream()
{
}

void GAFStream::readNBytesOfT(void* dest, unsigned int n)
{
    align();

    m_input->readBytes(dest, n);
}

bool GAFStream::readBool()
{
    return readUint(1) ? true : false;
}

// Reads a bit-packed unsigned integer from the stream
// and returns it.  The given bitcount determines the
// number of bits to read.
unsigned int GAFStream::readUint(unsigned short bitcount)
{
    assert(bitcount <= 32);

    unsigned int retval = 0;

    while (bitcount)
    {
        if (m_unusedBits)
        {
            if (bitcount >= m_unusedBits)
            {
                retval |= (m_currentByte << (bitcount - m_unusedBits));
                bitcount -= m_unusedBits;

                m_currentByte = 0;
                m_unusedBits = 0;
            }
            else
            {
                retval |= (m_currentByte >> (m_unusedBits - bitcount));
                m_currentByte &= ((1 << (m_unusedBits - bitcount)) - 1);
                m_unusedBits -= bitcount;

                bitcount = 0;
            }
        }
        else
        {
            m_currentByte = m_input->read1Byte();
            m_unusedBits = 8;
        }
    }

    assert(bitcount == 0);

    return retval;
}

// Reads a bit-packed little-endian signed integer
// from the stream.  The given bitcount determines the
// number of bits to read.
int GAFStream::readSint(unsigned short bitcount)
{
    assert(bitcount <= 32);

    int retval = (int)readUint(bitcount);

    if (retval & (1 << (bitcount - 1)))
    {
        retval |= -1 << bitcount;
    }

    return retval;
}

float GAFStream::readFixed()
{
    int retval = m_input->read4Bytes();
    return static_cast<float>(retval / 65536.0f);
}

float GAFStream::readFixed8()
{
    short retval = m_input->read2Bytes();
    return static_cast<float>(retval / 255.0f);
}

unsigned char GAFStream::readUByte()
{
    align();
    return m_input->read1Byte();
}

char GAFStream::readSByte()
{
    align();
    return m_input->read1Byte();
}

unsigned short GAFStream::readU16()
{
    unsigned short retval = 0;
    readNBytesOfT(&retval, sizeof(unsigned short));
    return retval;
}

unsigned int GAFStream::readU32()
{
    align();
    return m_input->read4Bytes();
}

int GAFStream::readS32()
{
    align();
    return static_cast<int>(m_input->read4Bytes());
}

void GAFStream::readString(std::string* out)
{
    m_input->readString(out);
}

GAFFile* GAFStream::getInput() const
{
    return m_input;
}

Tags::Enum GAFStream::openTag()
{
    align();

    /*int tagHeader = readU16();
    int tagType = tagHeader >> 6;
    unsigned int tagLenght = tagHeader & 0x3F;*/

    unsigned short tagType = readU16();
    unsigned int tagLenght = readU32();

    assert(m_unusedBits == 0);

    //BFORMATTED_GLOG(DLOG(INFO) << boost::format("[%d]: Opening tag: [%s] with len: [%d] expected stream position: [%d]") % getPosition() % Tags::toString((Tags::Enum)tagType) % tagLenght % (getPosition() + tagLenght));

    TagRecord record = { m_input->getPosition() + tagLenght, tagLenght, (Tags::Enum)tagType };

    //CCLOG("[%d]: Opening tag: [%s] with len: [%d] expected stream position: [%d]", getPosition(), Tags::toString(record.tagType).c_str(), record.tagSize, record.expectedStreamPos);

    m_tagStack.push(record);

    return (Tags::Enum)tagType;
}

void GAFStream::closeTag()
{
    assert(!m_tagStack.empty());

    TagRecord record = m_tagStack.top();

    m_tagStack.pop();

    const uint32_t inputPosition = m_input->getPosition();

    if (record.expectedStreamPos != inputPosition)
    {
        CCLOGERROR("Tag [%s] hasn't been correctly read, tag length is not respected. Expected [%d] but actually [%d]", Tags::toString(record.tagType).c_str(), record.expectedStreamPos, inputPosition);

        //assert("Tag hasn't been correctly read, tag length was not respected" && false);
    }

    m_input->rewind(record.expectedStreamPos);

    m_unusedBits = 0;
}

unsigned int GAFStream::getTagLenghtOnStackTop() const
{
    assert(!m_tagStack.empty());

    return m_tagStack.top().tagSize;
}

unsigned int GAFStream::getTagExpectedPosition() const
{
    assert(!m_tagStack.empty());

    return m_tagStack.top().expectedStreamPos;
}

unsigned int GAFStream::getPosition() const
{
    return m_input->getPosition();
}

void GAFStream::align()
{
    m_currentByte = 0;
    m_unusedBits = 0;
}

bool GAFStream::isEndOfStream() const
{
    return m_input->isEOF();
}

float GAFStream::readFloat()
{
    return m_input->readFloat();
}

NS_GAF_END