#include "GAFPrecompiled.h"
#include "GAFFile.h"
#include "platform/CCFileUtils.h"

#define USE_ZLIB 1

#if USE_ZLIB
    #include <zlib.h>
#endif

NS_GAF_BEGIN

void GAFFile::_readHeaderBegin(GAFHeader& out)
{
    readBytes(&out.compression, 4);
    out.version = read2Bytes();
    out.fileLenght = read4Bytes();
}

GAFFile::GAFFile() : 
m_data(nullptr)
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
    m_data = nullptr;
    m_dataLen = 0;
    m_dataPosition = 0;
}

bool GAFFile::open(const unsigned char* data, size_t len)
{
    close();

    m_data = const_cast<unsigned char*>(data);
    m_dataLen = len;

    if (m_data)
    {
        return _processOpen();
    }
    
    return false;
}

bool GAFFile::open(const std::string& filePath, const char* openMode)
{
    close();

    m_data = _getData(filePath, openMode, m_dataLen);

    if (m_data)
    {
        return _processOpen();
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

unsigned char* GAFFile::_getData(const std::string& filename, const char* openMode, unsigned long& outLen)
{
    assert(!(filename.empty()));

    unsigned char* ret = nullptr;
    outLen = 0;
    ssize_t size = 0;
    const char* mode = nullptr;

#ifdef ANDROID
    ret = cocos2d::FileUtilsAndroid::getInstance()->getFileData(filename, openMode, &size);
#else
    do
    {
        // Read the file from hardware
        std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
        FILE *fp = fopen(fullPath.c_str(), openMode);
        CC_BREAK_IF(!fp);
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        ret = (unsigned char*)malloc(sizeof(unsigned char)* size);

        size = fread(ret, sizeof(unsigned char), size, fp);
        fclose(fp);
    } while (0);
#endif

    if (nullptr == ret || 0 == size)
    {
        std::string msg = "Get data from file(";
        msg.append(filename).append(") failed!");
        CCLOG("%s", msg.c_str());
    }
    else
    {
        outLen = size;
    }

    return ret;
}

bool GAFFile::_processOpen()
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

NS_GAF_END
