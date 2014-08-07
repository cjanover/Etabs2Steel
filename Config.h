//
//  Config.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/10/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Config__
#define __Etabs2Steel__Config__

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <regex>

#include "FloatCompare.h"
#include "Debug.h"


extern std::map<unsigned int, unsigned int> nodeNumberLookup;
extern std::map <std::string, std::string> PARSESTRINGS;
extern std::string workingDirectory;
extern int debugLevel;

using namespace std;

class Config
{
    public:
    
    
        Config();
        void ParseConfig();
        void SaveConfig();

        std::string RemoveComment(std::string line);
    
        //Set Functions

    
        void AddConfigList(std::string configName){configList.push_back(configName);};
    
    
        void AddRawConfigVal(std::string key, std::string val){(configRaw[key]).push_back(val);};
        void AddSaveConfigVal(std::string key, std::string val){(configSave[key]).push_back(val);};
    
    
    
        //Get Functions
        const std::string ParserStringUnits(){return parseStringUnits;};
    
        //Config List
        std::vector<std::string>::iterator GetConfigListStart(){return configList.begin();};
        std::vector<std::string>::iterator GetConfigListEnd(){return configList.end();};
    
    
        //Raw Config
        std::map<std::string, std::vector<std::string> >::iterator GetTotRawConfigStart(){return configRaw.begin();};
        std::map<std::string, std::vector<std::string> >::iterator GetTotRawConfigEnd(){return configRaw.end();};
        std::vector<std::string>::iterator GetRawConfigStart(std::string key){return (configRaw[key]).begin();};
        std::vector<std::string>::iterator GetRawConfigEnd(std::string key){return (configRaw[key]).end();};
        void RemoveRaw(std::string key){configRaw.erase(key);};
        unsigned long GetRawConfigSize(std::string key){return (configRaw[key]).size();};
    
        //Save Config
        std::map<std::string, std::vector<std::string> >::iterator GetTotConfigStart(){return configSave.begin();};
        std::map<std::string, std::vector<std::string> >::iterator GetTotConfigEnd(){return configSave.end();};
        std::vector<std::string>::iterator GetConfigStart(std::string key){return (configSave[key]).begin();};
        std::vector<std::string>::iterator GetConfigEnd(std::string key){return (configSave[key]).end();};
        void RemoveSave(std::string key){configSave.erase(key);};
        unsigned long GetConfigSize(std::string key){return (configSave[key]).size();};

    private:
        std::string parseStringUnits = PARSESTRINGS["Units"];
    
        std::vector<std::string> configList;
        std::map<std::string, std::vector<std::string> > configRaw;
        std::map<std::string, std::vector<std::string> > configSave;
};

//Operator Overloads
std::ostream& operator<<(std::ostream& os, Config& config);
std::istream& operator>>(std::istream& is, Config& config);

#endif /* defined(__Etabs2Steel__Config__) */
