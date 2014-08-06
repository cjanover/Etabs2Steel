//
//  SteelSection.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/8/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__SteelSection__
#define __Etabs2Steel__SteelSection__

#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <regex>

#include "FloatCompare.h"
#include "Debug.h"

extern std::string workingDirectory;
extern int debugLevel;

class SteelSection
{
    public:
        SteelSection(){};
    
        //Parse Functions
        void ParsePrebuildDatabse(std::vector<std::string>::iterator listBegin, std::vector<std::string>::iterator listEnd, float SF);
    
        //Get Functions
        std::vector<std::string>::iterator GetSectionDefinitionStart(){return sectionDefinition.begin();};
        std::vector<std::string>::iterator GetSectionDefinitionEnd(){return sectionDefinition.end();};
    
        std::vector<std::string>::iterator GetSectionListStart(){return sectionList.begin();};
        std::vector<std::string>::iterator GetSectionListEnd(){return sectionList.end();};
    
        unsigned long GetConversion(std::string sectionName){return sectionConversion[sectionName];};
        unsigned int GetMaterialConversion(std::string sectionName){return materialConversion[sectionName];};
    
        std::map<std::string, unsigned long>::iterator GetConversionStart(){return sectionConversion.begin();};
        std::map<std::string, unsigned long>::iterator GetConversionEnd(){return sectionConversion.end();};
    
        std::map<std::string, unsigned int>::iterator GetMaterialConversionStart(){return materialConversion.begin();};
        std::map<std::string, unsigned int>::iterator GetMaterialConversionEnd(){return materialConversion.end();};
    
        unsigned long GetNumberSections(){return sectionList.size();};
    
        //Set Functions
        void AddSectionDefinition(const std::string label, const float d, const float tw, const float b, const float tf);
        void AddSectionDefinition(const std::string def){sectionDefinition.push_back(def);};
        
        void AddSectionList(std::string newSection){sectionList.push_back(newSection);};
    
        void AddConversion(std::string section);
        void AddMaterialConversion(const std::string section, const unsigned int conv){materialConversion[section] = conv;};
    
    private:
        std::vector<std::string> sectionDefinition;
        std::vector<std::string> sectionList;
        std::map<std::string, unsigned long> sectionConversion;
        std::map<std::string, unsigned int> materialConversion;

};

//Operator Overloads
std::ostream& operator<<(std::ostream& os, SteelSection& steelSection);

#endif /* defined(__Etabs2Steel__SteelSection__) */
