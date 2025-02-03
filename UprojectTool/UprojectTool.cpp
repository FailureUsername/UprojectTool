#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "json.hpp"
using json = nlohmann::json;
int main(int argc, char* argv[])
{
    if (strcmp(argv[2], "show-infos") == 0 )
        {
            std::cout << "infos\n";
            std::ifstream uproject_file(argv[1]);
            json data = json::parse(uproject_file);
            //std::cout << data;
            
    
            for (const auto& item : data.items())
            {
                if (item.key()== "EngineAssociation" || item.key()=="Plugins")
                {
                    std::cout << item.key() ;
                    for (const auto& val : item.value().items())
                    {
                        std::cout << "  " << val.key() << ": " << val.value() << "\n";
                    }
                }
                if (item.key()== "Modules")
                {
                    std::cout << item.key() ;
                    for (const auto& val : item.value().items())
                    {
                        if (val.key() == "0")
                        {
                            for (const auto& val2 : val.value().items())
                            {
                                if (val2.key() == "Name")
                                std::cout << "  " << val2.key() << ": " << val2.value() << "\n";
                            }
                        }
                    }
                }
            }
            
        }
        if (strcmp(argv[1], "build") == 0 )
        {
            std::cout << "build function unavailable\n";
        }
        if (strcmp(argv[1], "package") == 0 )
        {
            std::cout << "package function unavailable\n";
        }
    return 0;
}
