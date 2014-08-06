//
//  LoadPattern.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 3/6/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__LoadPattern__
#define __Etabs2Steel__LoadPattern__

#include <iostream>
#include <string>
#include <map>
#include <regex>

#include "FloatCompare.h"
#include "Debug.h"

extern int debugLevel;

extern std::map <std::string, std::string> PARSESTRINGS;

class LoadPattern
{
    public:
        LoadPattern(){};
    
        //Get Functions
        std::string PatternName(){return patternName;};
        std::string PatternType(){return patternType;};
    
        const std::string ParserString(){return parseString;};
    
        //Set Functions
        void PatternName(const std::string nPatternName){patternName = nPatternName;};
        void PatternType(const std::string nPatternType){patternType = nPatternType;};
    
    private:
        std::string patternName;
        std::string patternType;
    
        std::string parseString = PARSESTRINGS["LoadPatterns"];
};

//Operator Overloads
std::istream& operator>>(std::istream& is, LoadPattern& pattern);

#endif /* defined(__Etabs2Steel__LoadPattern__) */
