#include "GAFPrecompiled.h"
#include "GAFFile.h"
#include "platform/CCFileUtils.h"

#define USE_ZLIB 1

#if USE_ZLIB
    #ifdef WIN32
        #include <zlib/zlib.h>
    #else
        #include <zlib.h>
    #endif
#endif

void GAFFile::_readHeaderBegin(GAFHeader& out)
{
    readBytes(&out.compression, 4);
    out.version = read2Bytes();
    out.fileLenght = read4Bytes();
}

GAFFile::GAFFile() : 
m_data(NULL)
{

}

GAFFile::~GAFFile()
{
    close();
}

unsigned char GAFFile::read1Byte()
{
    unsigned char retval = 0;

    readBytes(&retval, sizeof(unsigned char));

    return retval;
}

unsigned short GAFFile::read2Bytes()
{
    unsigned short retval = 0;

    readBytes(&retval, sizeof(unsigned short));

    return retval;
}

unsigned int GAFFile::read4Bytes()
{
    unsigned int retval = 0;

    readBytes(&retval, sizeof(unsigned int));

    return retval;
}

unsigned long long GAFFile::read8Bytes()
{
    unsigned long long retval = 0;

    readBytes(&retval, sizeof(unsigned long long));

    return retval;
}

float GAFFile::readFloat()
{
    float retval = 0;

    readBytes(&retval, sizeof(float));

    return retval;
}

double GAFFile::readDouble()
{
    double retval = 0;

    readBytes(&retval, sizeof(double));

    return retval;
}

bool GAFFile::isEOF() const
{
    // Actually the data position should never be greater than the data len
    // But this is better safe
    return m_dataPosition >= m_dataLen;
}

unsigned int GAFFile::readString(std::string* str)
{
    assert(m_dataPosition + sizeof(short) <= m_dataLen);

    unsigned short len = read2Bytes();

    assert(m_dataPosition + len <= m_dataLen);

    char* data = new char[len];
    readBytes(data, len); // WARN. Possible optimization here. We are able to read from m_data directly

    str->assign(data, len);

    delete[] data;

    return str->length() + sizeof(unsigned short);
}

void GAFFile::readBytes(void* dst, unsigned int len)
{
    assert(dst && m_data);
    assert(m_dataPosition + len <= m_dataLen);

    memcpy(dst, m_data + m_dataPosition, len);
    m_dataPosition += len;
}

void GAFFile::close()
{
    delete[] m_data;
    m_data = NULL;
    m_dataLen = 0;
    m_dataPosition = 0;
}

bool GAFFile::open(const std::string& filePath, const char* openMode)
{
    close();

    m_data = CCFileUtils::sharedFileUtils()->getFileData(filePath.c_str(), openMode, &m_dataLen);

    if (m_data)
    {
        _readHeaderBegin(m_header);

        // 10 - is size of sizeof(Footprint) + sizeof(FileLen) + sizeof(Version)
        static const uint32_t UncompressedDataSize = 10;

        assert(m_dataPosition == UncompressedDataSize); // Paranoid mode

        if (m_header.compression == GAFHeader::CompressedNone)
        {
            // Loader will complete reading
        }
        else if (m_header.compression == GAFHeader::CompressedZip)
        {
#if USE_ZLIB
            unsigned long uncompressedSize = m_header.fileLenght;
            char* uncompressedBuffer = new char[uncompressedSize];

            int retStatus = uncompress((Bytef*)uncompressedBuffer, &uncompressedSize, (Bytef*)(m_data + m_dataPosition), m_dataLen - m_dataPosition); // Decompress rest

            if (retStatus != Z_OK)
            {
                return false;
            }

            assert("Paranoid mode" && uncompressedSize == m_header.fileLenght);

            delete[] m_data;

            m_data = new unsigned char[uncompressedSize];

            memcpy(m_data, uncompressedBuffer, uncompressedSize);
            m_dataLen = uncompressedSize;
            m_dataPosition = 0;

            delete[] uncompressedBuffer;
#else
            assert("ZLIB is disabled" && false);
#endif
        }
        else
        {
            // This is not a GAF file or a header is corrupted
            return false;
        }

        return true;
    }

    return false;
}

bool GAFFile::isOpened() const
{
    return m_data != NULL;
}

const GAFHeader& GAFFile::getHeader() const
{
    return m_header;
}

GAFHeader& GAFFile::getHeader()
{
    return m_header;
}

unsigned int GAFFile::getPosition() const
{
    return m_dataPosition;
}

void GAFFile::rewind(unsigned int newPos)
{
    m_dataPosition = newPos;
}
