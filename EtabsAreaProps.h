//
//  EtabsAreaProps.h
//  Etabs2Steel
//
//  Created by Christopher Janover on 5/22/14.
//  Copyright (c) 2014 Christopher Janover. All rights reserved.
//

#ifndef __Etabs2Steel__EtabsAreaProps__
#define __Etabs2Steel__EtabsAreaProps__

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <map>

#include "Parse.h"
#include "debug.h"

#include "FloatCompare.h"

extern std::map <std::string, std::string> PARSESTRINGS;
extern int debugLevel;
class EtabsAreaProps
{
    public:
        EtabsAreaProps(){};
    
        //Get Functions
        std::string Name(){return name;};
        std::string Type(){return type;};
        std::string Material(){return material;};
        float Thickness(){return thickness;};
    
        //Set Functions
        void Name(const std::string nName){name = nName;};
        void Type(const std::string nType){type = nType;};
        void Material(const std::string nMaterial){material = nMaterial;};
        void Thickness(const float nThickness){thickness = nThickness;};
    
    
        const std::string ParserStringWall(){return parseStringWall;};
        const std::string ParserStringSlab(){return parseStringSlab;};
    
    private:
        std::string name;
        std::string type;
        std::string material;
        float thickness;
    
    
        std::string parseStringWall = PARSESTRINGS["EtabsWallProp"];
        std::string parseStringSlab = PARSESTRINGS["EtabsSlabProp"];

};

//Operator Overloads
std::istream& operator>>(std::istream& is, EtabsAreaProps& props);

#endif /* defined(__Etabs2Steel__EtabsAreaProps__) */
