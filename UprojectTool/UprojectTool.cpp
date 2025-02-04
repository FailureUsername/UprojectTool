#include <cstring>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "json.hpp"
#include <windows.h>

#define BUILD_COMMAND ".\\Engine\\Build\\BatchFiles\\Build.bat "
#define BUILD_TARGET " Win64 "
using json = nlohmann::json;

void showInfos(const std::string& uproject_path) {
    std::ifstream file(uproject_path);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier: " << uproject_path << std::endl;
        return;
    }

    
    json data = json::parse(file);

    if (data.find("Modules") == data.end()) {
        std::cerr << "Section [Modules] introuvable dans le fichier .uproject, nom impossible à afficher" << std::endl;
        return;
    }

    std::cout << "\nINFORMATIONS UPROJECT\n" << std::endl;
    std::string projectName = data["Modules"][0]["Name"];
    std::string EngineVersion = data["EngineAssociation"];

    if (EngineVersion.length() > 3) {
        EngineVersion = "Build from source";
    }
    
    std::cout << "Version Moteur: " << EngineVersion << std::endl;
    std::cout << "Nom du Projet: " << projectName << std::endl;

    if (data.find("Plugins") == data.end()) {
        std::cout << "Section [Plugins] introuvable dans le fichier .uproject" << std::endl;
        return;
    }

    std::cout << "\nPlugins installés: " << std::endl;

    json plugins = data["Plugins"];
    for (json::iterator it = plugins.begin(); it != plugins.end(); ++it) {
        std::string pluginName = it.value()["Name"];
        std::cout << "-" + pluginName + "\n" << std::endl;
    }
}

void build(const std::string& uproject_path)
{
    std::ifstream file(uproject_path);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier: " << uproject_path << std::endl;
        return;
    }

    
    json data = json::parse(file);

    if (data.find("Modules") == data.end()) {
        std::cerr << "Section [Modules] introuvable dans le fichier .uproject, nom impossible à récupérer" << std::endl;
        return;
    }

    std::string projectName = data["Modules"][0]["Name"];
    std::string target = projectName +"Editor";
    std::string command = std::string(BUILD_COMMAND) + " " + projectName + " " + target +
                         std::string(BUILD_TARGET) + " Development " + uproject_path + " -waitmutex";
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error: Build command failed with code " << result << std::endl;
    }
}

void package(const std::string& uproject_path,const std::string&  package_path)
{
    std::cout << "Project Path: " << uproject_path << std::endl;
    std::string command =
    ".\\Engine\\Build\\BatchFiles\\RunUAT.bat -ScriptsForProject="+ uproject_path +" BuildCookRun -project="+ uproject_path +" -noP4 -clientconfig=Development -serverconfig=Development -nocompileeditor -unrealexe=UnrealEditor-Cmd.exe -utf8output -platform=Win64 -build -cook -CookCultures=en -unversionedcookedcontent -stage -package -cmdline=\" -Messaging\" -addcmdline=\"-SessionId=64491F724FBC769EE969F29ADEBD68E5 -SessionOwner='fchal' -SessionName='MyFirstPackagingProfile'   \" -archive -archivedirectory="+ package_path;
    std::cout << "Executing command: " << command << std::endl;
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error: Build command failed with code " << result << std::endl;
    }
}



int main(int argc, char* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    const std::string projectPath = argv[1];
    
    if (strcmp(argv[2], "show-infos") == 0 )
        {
            showInfos(projectPath);
        }
        if (strcmp(argv[2], "build") == 0 )
        {
            build(projectPath);
        }
        if (strcmp(argv[2], "package") == 0 )
        {
            
            if(argc>3)
            {
                const std::string packagePath = argv[3]; 
                package(projectPath, packagePath);
            }
            else
                std::cerr << "Pas de path de de pour le package" << std::endl;
        }
    return 0;
}
