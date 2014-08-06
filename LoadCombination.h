//
//  LoadCombination.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__LoadCombination__
#define __Etabs2Steel__LoadCombination__

#include <iostream>
#include <string>
#include <map>
#include <regex>

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;

extern std::map <std::string, std::string> PARSESTRINGS;

class LoadCombination
{
    public:
        static std::string MassComboName;
        static std::string LoadComboName;
    
        LoadCombination(){};
    
        //Get Functions
        std::string Name(){return name;};
        bool CaseExists(std::string caseName);
        float LoadSF(std::string caseName){return loadSF[caseName];};
    
        std::string GetFirstName(){return loadSF.begin()->first;};
        std::map<std::string, float>::iterator GetStartIterator(){return loadSF.begin();};
        std::map<std::string, float>::iterator GetEndIterator(){return loadSF.end();};
    
        const std::string ParserString(){return parseString;};
    
        //Set Functions
        void Name(const std::string nName){name = nName;};
        void LoadSF(std::string caseName, float SF){loadSF[caseName] = SF;};
    
    private:
        std::string name;
        std::map<std::string, float> loadSF;
    
        std::string parseString = PARSESTRINGS["LoadCombinations"];
    


};

//Operator Overloads
std::istream& operator>>(std::istream& is, LoadCombination& load);



#endif /* defined(__Etabs2Steel__LoadCombination__) */
