//
//  Slab.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 4/9/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__Slab__
#define __Etabs2Steel__Slab__

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

extern int debugLevel;

class Slab
{
    public:
        //Parse Functions
        void ParsePrebuildDatabse(std::vector<std::string>::iterator listBegin, std::vector<std::string>::iterator listEnd);
    
        //Get Functions
        std::vector<std::string>::iterator GetSlabDefinitionStart(){return slabDefinition.begin();};
        std::vector<std::string>::iterator GetSlabDefinitionEnd(){return slabDefinition.end();};
    
        std::vector<std::string>::iterator GetSlabListStart(){return slabList.begin();};
        std::vector<std::string>::iterator GetSlabListEnd(){return slabList.end();};
    
        unsigned long GetConversion(std::string sectionName){return slabConversion[sectionName];};
    
        unsigned long GetNumberSlabs(){return slabList.size();};
    
        //Set Functions
        void AddSlabDefinition(const std::string label, const float adeck, const float ddeck, const float aslab, const float dslab);
        void AddSlabDefinition(const std::string def){slabDefinition.push_back(def);};
        
        void AddSlabList(std::string newSlab){slabList.push_back(newSlab);};
    
        void AddConversion(std::string section);
    private:
        std::vector<std::string> slabDefinition;
        std::vector<std::string> slabList;
        std::map<std::string, unsigned long> slabConversion;
};
#endif /* defined(__Etabs2Steel__Slab__) */
