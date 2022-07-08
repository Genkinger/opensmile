#include "olmega.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

int32_t main(int32_t Count, char** Arguments)
{
/*    Olmega::Feat Feat(Arguments[1]);
    std::ofstream OutputFile(Arguments[2]);
    OutputFile << Feat.CSV();
*/
    for(auto& Entry : std::filesystem::directory_iterator(Arguments[1]))
    {
        if(Entry.is_regular_file())
        {
            Olmega::Feat Feat(Entry.path().string().c_str());
            std::ofstream OutputFile(Entry.path().string() + ".csv");
            OutputFile << Feat.CSV();
        }
    }
       
    return(0);
}
