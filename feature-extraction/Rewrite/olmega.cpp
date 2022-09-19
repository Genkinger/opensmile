#include "olmega.hpp"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace Olmega
{
    using namespace Util;
    Feat::Feat(const char* Path)
    {
        FILE *File = fopen(Path,"rb");
        fseek(File,0,SEEK_END);
        size_t Size = ftell(File);
        rewind(File);
        uint8_t *Buffer = new uint8_t[Size];
        fread(Buffer,Size,1,File);
        fclose(File);

        Version = Peek<int32_t>(Buffer,0,true);
        switch(Version)
        {
            case 2:
                LoadV2(Buffer);
                break;
            case 3:
                LoadV3(Buffer);
                break;
            case 4:
                LoadV4(Buffer);
                break;
            default:
                std::cerr << "Protocol Version '" << Version << "' not supported!\n";
                //TODO(L): Completely ABORT here!
                return;
                break;
        }

        delete[] Buffer;
    }

    void Feat::LoadV2(uint8_t *Data)
    {
        size_t Offset = 0;
        Version = Pop<int32_t>(Data,Offset,true);
        BlockCount = Pop<int32_t>(Data,Offset,true);
        FeatureDimensions = Pop<int32_t>(Data,Offset,true);
        BlockSize = Pop<int32_t>(Data,Offset,true);
        HopSize = Pop<int32_t>(Data,Offset,true);
        SamplingRate = Pop<int32_t>(Data,Offset,true);
        memcpy(SampleTimestamp,Data,sizeof(SampleTimestamp));
        Offset += sizeof(SampleTimestamp);
        CalibrationChannel1 = Pop<float>(Data,Offset,true);
        CalibrationChannel2 = Pop<float>(Data,Offset,true);
        
        ExtractData(Data,Offset);
    }

    void Feat::LoadV3(uint8_t *Data)
    {
        
        size_t Offset = 0;
        Version = Pop<int32_t>(Data,Offset,true);
        BlockCount = Pop<int32_t>(Data,Offset,true);
        FeatureDimensions = Pop<int32_t>(Data,Offset,true);
        BlockSize = Pop<int32_t>(Data,Offset,true);
        HopSize = Pop<int32_t>(Data,Offset,true);
        SamplingRate = Pop<int32_t>(Data,Offset,true);
        memcpy(SampleTimestamp,Data,sizeof(SampleTimestamp));
        Offset += sizeof(SampleTimestamp);
        memcpy(SystemTimestamp,Data,sizeof(SystemTimestamp));
        Offset += sizeof(SystemTimestamp);
        CalibrationChannel1 = Pop<float>(Data,Offset,true);
        CalibrationChannel2 = Pop<float>(Data,Offset,true);

        
        ExtractData(Data,Offset);
    }
    
    void Feat::LoadV4(uint8_t *Data)
    {
        size_t Offset = 0;
        Version = Pop<int32_t>(Data,Offset,true);
        BlockCount = Pop<int32_t>(Data,Offset,true);
        FeatureDimensions = Pop<int32_t>(Data,Offset,true);
        BlockSize = Pop<int32_t>(Data,Offset,true);
        HopSize = Pop<int32_t>(Data,Offset,true);
        SamplingRate = Pop<int32_t>(Data,Offset,true);
        memcpy(SampleTimestamp,Data,sizeof(SampleTimestamp));
        Offset += sizeof(SampleTimestamp);
        memcpy(SystemTimestamp,Data,sizeof(SystemTimestamp));
        Offset += sizeof(SystemTimestamp);
        CalibrationChannel1 = Pop<float>(Data,Offset,true);
        CalibrationChannel2 = Pop<float>(Data,Offset,true);
        memcpy(AndroidIdentifier,Data,sizeof(AndroidIdentifier));
        Offset += sizeof(AndroidIdentifier);
        memcpy(BluetoothTransmitterMAC,Data,sizeof(BluetoothTransmitterMAC));
        Offset += sizeof(BluetoothTransmitterMAC);

        
        ExtractData(Data,Offset);
        
    }
    
    void Feat::ExtractData(uint8_t *Data, size_t Offset)
    {
        //TODO(L): Extract all the Feature Data
        
        for(int32_t Feature = 0; Feature < BlockCount; Feature++)
        {
            Features.push_back(std::vector<float>());
            for(int32_t Dimension = 0; Dimension < FeatureDimensions; Dimension++)
            {
                Features[Feature].push_back(Pop<float>(Data,Offset,true));
            }
        }
    }

    void Feat::Print()
    {        
        std::cout << Version << "\n";
        std::cout << BlockCount << "\n";
        std::cout << FeatureDimensions << "\n";
        std::cout << BlockSize << "\n";
        std::cout << HopSize << "\n";
        std::cout << SamplingRate << "\n";
        std::cout << SampleTimestamp << "\n";
        std::cout << SystemTimestamp << "\n";
        std::cout << CalibrationChannel1 << "\n";
        std::cout << CalibrationChannel2 << "\n";
        std::cout << AndroidIdentifier << "\n";
        std::cout << BluetoothTransmitterMAC << "\n";

        for( auto& Feature : Features)
        {
            for( auto& Value: Feature)
            {
                std::cout << Value << " ";
            }
            std::cout << "\n";
        }
    }

    std::string Feat::CSV()
    {
        std::stringstream Stream;
        Stream << "Start,Stop";
        for(size_t i = 0; i < FeatureDimensions - 2; i++)
        {
            Stream << ",";
        }
        Stream << "\n";
        
        for( auto& Feature : Features)
        {
            
            for( size_t i = 0; i < Feature.size(); i++)
            {
                Stream << Feature[i];
                if(i != Feature.size()-1)
                {
                    Stream << ",";
                }
            }
                Stream << "\n";
        }
        std::string Result = Stream.str();
//        std::replace(std::begin(Result), std::end(Result), '.',',');
        return Result;
    }
}
