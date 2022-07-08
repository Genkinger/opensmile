#pragma once
#include <vector>
#include <cstdint>
#include <string>

namespace Olmega
{

    namespace Util
    {
        template<typename T>
        T Peek(uint8_t *Data, size_t Offset, bool SwapBytes)
        {
            uint8_t Buffer[sizeof(T)];
            for(size_t i = 0; i < sizeof(T); i++)
            {
                Buffer[(SwapBytes ? sizeof(T)-1-i : i )] = *(Data+Offset+i);
            }
            return(*reinterpret_cast<T*>(Buffer));
        }
        template<typename T>
        T Pop(uint8_t *Data, size_t &Offset, bool SwapBytes)
        {
            T Value = Peek<T>(Data,Offset,SwapBytes);
            Offset += sizeof(T);
            return Value;
        }
    }

    class Feat
    {
    public:
        Feat(const char*  Path);
    public:
        void Print();
        std::string CSV();
    private:
        void LoadV2(uint8_t *Data);
        void LoadV3(uint8_t *Data);
        void LoadV4(uint8_t *Data);
        void ExtractData(uint8_t *Data, size_t Offset);
    private:
        int32_t Version = 0;
        int32_t BlockCount = 0;
        int32_t FeatureDimensions = 0;
        int32_t BlockSize = 0;
        int32_t HopSize = 0;
        int32_t SamplingRate = 0;
        char    SampleTimestamp[16] = {};
        char    SystemTimestamp[16] = {};
        float   CalibrationChannel1 = 0;
        float   CalibrationChannel2 = 0;
        uint8_t AndroidIdentifier[16] = {};
        uint8_t BluetoothTransmitterMAC[17] = {};
        std::vector<std::vector<float>> Features; //NOTE(L): This type is meh ...
    };
}
