#pragma once

NS_GAF_BEGIN

class GAFSoundInfo
{
public:
    const static std::string SoundEvent;

    enum class Format : uint8_t
    {
        WAV = 0,
        MP3 = 1,
        OGG = 2,
    };

    enum class Rate : uint8_t
    {
        kHz_5_5 = 0, // 5.5
        kHz_11  = 1,
        kHz_22  = 2,
        kHz_44  = 3,
    };

    enum class SampleSize : uint8_t
    {
        Bit_8   = 0,
        Bit_16  = 1,
    };

    enum class SyncEvent : uint8_t
    {
        Stop        = 1,
        Start       = 2,
        Continue    = 3,
    };

    uint16_t id;
    std::string linkage;
    std::string source;
    Format format;
    Rate rate;
    SampleSize sampleSize;
    bool stereo;
    uint32_t sampleCount;

    void setFormat(uint8_t value) { format = static_cast<Format>(value); }
    void setRate(uint8_t value) { rate = static_cast<Rate>(value); }
    void setSampleSize(uint8_t value) { sampleSize = static_cast<SampleSize>(value); }
};

NS_GAF_END